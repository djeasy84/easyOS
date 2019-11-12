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

#ifndef M24M02_H
#define M24M02_H

#define I2C_EEPROM_ID 0x50

#define BANK_SIZE (2UL*((1024UL*1024UL)/8UL))

class ExternalEEPROM
{
    public:
        bool setup(bool twoBank=false);

        bool read(uint32_t addr, uint8_t *data);
        bool write(uint32_t addr, uint8_t data);

        bool read(uint32_t addr, uint8_t *data, uint32_t len);
        bool write(int32_t addr, uint8_t *data, uint32_t len);

    private:
        bool getRegistry(uint8_t devAddrCmd, uint16_t regAddr, uint8_t *data);
        bool setRegistry(uint8_t devAddrCmd, uint16_t regAddr, uint8_t data);

        uint32_t maxAddr;
};

ExternalEEPROM ExEEPROM;

/****************************************************************************************/

bool ExternalEEPROM::setup(bool twoBank)
{
    maxAddr = (BANK_SIZE - 1) * ((twoBank) ? 2 : 1);
    return true;
}

bool ExternalEEPROM::read(uint32_t addr, uint8_t *data)
{
    if (addr > maxAddr)
        return false;
    uint8_t nBank = (addr >= BANK_SIZE) ? 1 : 0;
    uint32_t intAddr = (nBank == 0) ? addr : addr - BANK_SIZE;
    uint8_t dsc = I2C_EEPROM_ID | (nBank << 2) | (intAddr >> 16);
    if (!getRegistry(dsc, intAddr, data))
        return false;
    return true;
}

bool ExternalEEPROM::write(uint32_t addr, uint8_t data)
{
    if (addr > maxAddr)
        return false;
    uint8_t nBank = (addr >= BANK_SIZE) ? 1 : 0;
    uint32_t intAddr = (nBank == 0) ? addr : addr - BANK_SIZE;
    uint8_t dsc = I2C_EEPROM_ID | (nBank << 2) | (intAddr >> 16);
    if (!setRegistry(dsc, intAddr, data))
        return false;
    uint8_t value = 0;
    for (uint8_t i=0; i<0xFF; i++)
    {
        if (getRegistry(dsc, intAddr, &value))
        {
            if (value == data)
                return true;
        }
    }
    return false;
}

bool ExternalEEPROM::read(uint32_t addr, uint8_t *data, uint32_t len)
{
    for (uint32_t i=0; i<len; i++)
    {
        if (!read(addr+i, &data[i]))
            return false;
        ST.watchdog_reset();
    }
    return true;
}

bool ExternalEEPROM::write(int32_t addr, uint8_t *data, uint32_t len)
{
    for (uint32_t i=0; i<len; i++)
    {
        if (!write(addr+i, data[i]))
            return false;
        ST.watchdog_reset();
    }
    return true;
}

bool ExternalEEPROM::setRegistry(uint8_t devAddrCmd, uint16_t regAddr, uint8_t data)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddrCmd, false))
        return false;
    if (!I2C.sendData((regAddr) >> 8))
        return false;
    if (!I2C.sendData((regAddr) & 0x00FF))
        return false;
    if (!I2C.sendData(data))
        return false;
    if (!I2C.sendStop())
        return false;
    return true;
}

bool ExternalEEPROM::getRegistry(uint8_t devAddrCmd, uint16_t regAddr, uint8_t *data)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddrCmd, false))
        return false;
    if (!I2C.sendData((regAddr) >> 8))
        return false;
    if (!I2C.sendData((regAddr) & 0x00FF))
        return false;
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddrCmd, true))
        return false;
    if (!I2C.readData(data, true))
        return false;
    if (!I2C.sendStop())
        return false;
    return true;
}

#endif
