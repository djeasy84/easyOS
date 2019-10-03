/*
 * MIT License
 *
 * This file is part of easyOS.
 * You can find the software at https://github.com/djeasy84/easyOS
 *
 * Copyright (c) 2018 - Lorenzo Baglioni (djeasy84) <lbaglioni84@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef DEVICE_INFORMATION_H
#define DEVICE_INFORMATION_H

#include <string.h>

#define BLOCK_NUM 32

#define NAME_SIZE (16 + 1)
#define DATA_SIZE (110 + 1)
#define BLOCK_SIZE (NAME_SIZE + DATA_SIZE + 2 /*CRC*/)  // 128 Bytes

#define FREE_BLOCK 0
#define USED_BLOCK 1
#define BAD_BLOCK 2

struct dataBlock
{
    char name[NAME_SIZE];
    char data[DATA_SIZE];
    uint16_t crc;
};

class DeviceInformation
{
    public:
        bool setup(uint32_t addr = 0x00000000);

        bool writeBlock(char *name, char *data);
        bool readBlock(char *name, char *data, uint8_t maxLen);

        bool freeBlock(char *name);

        bool formatBlocks(bool fastCleaning = true);

    private:
        bool findBlockByFree(uint8_t *idxBlock);
        bool findBlockByName(char *name, uint8_t *idxBlock);

        uint16_t CRC16(uint8_t *data, uint8_t len);

        uint32_t startAddr;
};

DeviceInformation DI;

/****************************************************************************************/

bool DeviceInformation::setup(uint32_t addr)
{
    // OFFSET IN EEPROM
    startAddr = addr;

    // FAST CHECK INDEX
    for (uint32_t i = 0; i < BLOCK_NUM; i++)
    {
        uint8_t data = 0xFF;
        if (!ExEEPROM.read(startAddr + i, &data))
            return false;

        if (data != FREE_BLOCK && data != USED_BLOCK && data != BAD_BLOCK)
            return false;

        #if defined (TIME_M)
        ST.watchdog_reset();
        #endif
    }

    return true;
}

bool DeviceInformation::writeBlock(char *name, char *data)
{
    // CHECK NAME LEN
    if (strlen(name) > (NAME_SIZE - 1))
        return false;

    // FIND NEW OR EXISTING
    uint8_t idxBlock = 0;
    if (!findBlockByName(name, &idxBlock))
    {
        if (!findBlockByFree(&idxBlock))
            return false;
    }

    // CHECK DATA LEN
    if (strlen(data) > (DATA_SIZE - 1))
        return false;

    // ALLOCATE TEMP BLOCK
    struct dataBlock blockTmp;
    memset(&blockTmp, 0, BLOCK_SIZE);

    // NAME
    strcpy(blockTmp.name, name);
    blockTmp.name[NAME_SIZE - 1] = '\0';

    // DATA
    strcpy(blockTmp.data, data);
    blockTmp.data[DATA_SIZE - 1] = '\0';

    // CRC16
    blockTmp.crc = CRC16((uint8_t *)blockTmp.data, DATA_SIZE);

    // WRITE BLOCK
    if (!ExEEPROM.write(startAddr + (BLOCK_NUM + (idxBlock * BLOCK_SIZE)), (uint8_t *)&blockTmp, BLOCK_SIZE))
        return false;

    // FLAG AS USED BLOCK
    if (!ExEEPROM.write(startAddr + idxBlock, USED_BLOCK))
        return false;

    return true;
}

bool DeviceInformation::readBlock(char *name, char *data, uint8_t maxLen)
{
    // CHECK NAME LEN
    if (strlen(name) > (NAME_SIZE - 1))
        return false;

    // FIND BLOCK
    uint8_t idxBlock = 0;
    if (!findBlockByName(name, &idxBlock))
        return false;

    // ALLOCATE TEMP BLOCK
    struct dataBlock blockTmp;
    memset(&blockTmp, 0, BLOCK_SIZE);

    // READ BLOCK
    if (!ExEEPROM.read(startAddr + (BLOCK_NUM + (BLOCK_SIZE * idxBlock)), (uint8_t *)&blockTmp, BLOCK_SIZE))
        return false;

    // CRC16
    if (CRC16((uint8_t *)blockTmp.data, DATA_SIZE) != blockTmp.crc)
    {
        // FLAG AS BAD BLOCK
        if (!ExEEPROM.write(startAddr + idxBlock, BAD_BLOCK))
            return false;
    }

    // CHECK DATA LEN
    if ((strlen(blockTmp.data) + 1) > maxLen)
        return false;

    // COPY DATA
    strcpy(data, blockTmp.data);

    return true;
}

bool DeviceInformation::freeBlock(char *name)
{
    uint8_t idxBlock = 1;
    if (!findBlockByName(name, &idxBlock))
        return false;

    // FLAG AS FREE BLOCK
    if (!ExEEPROM.write(startAddr + idxBlock, FREE_BLOCK))
        return false;

    return true;
}

bool DeviceInformation::formatBlocks(bool fastCleaning)
{
    uint32_t maxAddr = BLOCK_NUM;
    if (!fastCleaning)
        maxAddr = (BLOCK_NUM + (BLOCK_SIZE * BLOCK_NUM));

    for (uint32_t i = 0; i < maxAddr; i++)
    {
        if (!ExEEPROM.write(startAddr + i, 0x00))
            return false;

        #if defined (TIME_M)
        ST.watchdog_reset();
        #endif
    }
}

bool DeviceInformation::findBlockByFree(uint8_t *idxBlock)
{
    // FIND FIRST FREE BLOCK
    for (uint32_t i = 0; i < (BLOCK_NUM + (BLOCK_SIZE * BLOCK_NUM)); i++)
    {
        uint8_t data = 0xFF;
        if (!ExEEPROM.read(startAddr + i, &data))
            return false;

        if (data != FREE_BLOCK && data != USED_BLOCK && data != BAD_BLOCK)
            return false;

        if (data == FREE_BLOCK)
        {
            *idxBlock = i;
            return true;
        }

        #if defined (TIME_M)
        ST.watchdog_reset();
        #endif
    }
    return false;
}

bool DeviceInformation::findBlockByName(char *name, uint8_t *idxBlock)
{
    // FIND BLOCK BY NAME
    for (uint32_t i = 0; i < (BLOCK_NUM + (BLOCK_SIZE * BLOCK_NUM)); i++)
    {
        uint8_t data = 0xFF;
        if (!ExEEPROM.read(startAddr + i, &data))
            return false;

        if (data != FREE_BLOCK && data != USED_BLOCK && data != BAD_BLOCK)
            return false;

        if (data == USED_BLOCK)
        {
            struct dataBlock blockTmp;
            if (!ExEEPROM.read(startAddr + (BLOCK_NUM + (BLOCK_SIZE * i)), (uint8_t *)&blockTmp, BLOCK_SIZE))
                return false;

            if (!strncmp(blockTmp.name, name, (NAME_SIZE - 1)))
            {
                *idxBlock = i;
                return true;
            }
        }

        #if defined (TIME_M)
        ST.watchdog_reset();
        #endif
    }
    return false;
}

uint16_t DeviceInformation::CRC16(uint8_t *data, uint8_t len)
{
    uint16_t crc = 0;
    uint16_t divisor = 0b1100101000101001;
    for (uint8_t i=0; i<len && i<DATA_SIZE; i++)
    {
        crc ^= (uint16_t)(data[i] << 8);
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((crc & 0b1000000000000000) != 0)
                crc = (uint16_t)((crc << 1) ^ divisor);
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

#endif
