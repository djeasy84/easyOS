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

#ifndef PN532_H
#define PN532_H

#define I2C_NFC_ID 0x48

class NearFieldCommunication
{
    public:
        bool setup();

        bool read(uint8_t *data);  // LEN: 7

    private:
        bool sendCommand(uint8_t devAddr, uint8_t *data, uint8_t len);
        bool readResponse(uint8_t devAddr, uint8_t *data, uint8_t len);

        uint32_t lastRead;
        uint32_t lastCheck;
};

NearFieldCommunication NFC;

/****************************************************************************************/

bool NearFieldCommunication::setup()
{
    lastRead = lastCheck = 0;

    uint8_t txData[4] = {0x14, 0x01, 0x00, 0x00};

    if (!sendCommand(I2C_NFC_ID, txData, 4))
        return false;

    uint8_t rxData[8];
    if (!readResponse(I2C_NFC_ID, rxData, 8))
        return false;

    if (rxData[6] != 0x15)
        return false;

    return true;
}

bool NearFieldCommunication::read(uint8_t *data)
{
    if (ST.time_diff(ST.millisec(), lastCheck) < 100)
        return false;

    lastCheck = ST.millisec();

    if (ST.time_diff(ST.millisec(), lastRead) < 1000)
        return false;

    uint8_t txData[4];
    uint8_t cmdBuffer[3] = {0x4A, 0x01, 0x00};
    if (!sendCommand(I2C_NFC_ID, cmdBuffer, 3))
        return false;

    ST.wait_millisec(25);

    uint8_t rxData[20];
    if (!readResponse(I2C_NFC_ID, rxData, 20))
        return false;

    if (rxData[7] != 1)
        return false;

    if (rxData[12] != 7)
        return false;

    for (uint8_t i=0; i<rxData[12]; i++)
        data[i] = rxData[13+i];

    lastRead = ST.millisec();

    return true;
}

bool NearFieldCommunication::sendCommand(uint8_t devAddr, uint8_t *data, uint8_t len)
{
    uint8_t dataCheckSum = 0;
    uint8_t dataLen = len + 1;

    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite((devAddr >> 1), false))
        return false;

    if (!I2C.sendData(0x00))
        return false;
    if (!I2C.sendData(0x00))
        return false;
    if (!I2C.sendData(0xFF))
        return false;
    dataCheckSum += 0x00 + 0x00 + 0xFF;

    if (!I2C.sendData(dataLen))
        return false;
    if (!I2C.sendData(~dataLen + 1))
        return false;

    if (!I2C.sendData(0xD4))
        return false;
    dataCheckSum += 0xD4;

    for (uint8_t i=0; i<dataLen-1; i++)
    {
        if (!I2C.sendData(data[i]))
            return false;

        dataCheckSum += data[i];
    }

    if (!I2C.sendData(~dataCheckSum))
        return false;

    if (!I2C.sendData(0x00))
        return false;

    if (!I2C.sendStop())
        return false;

    ST.wait_millisec(5);

    uint8_t dataReceived[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!readResponse(devAddr, dataReceived, 6))
        return false;

    uint8_t expectedACK[6] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    if (strncmp((char *)dataReceived, (char *)expectedACK, 6))
        return false;

    ST.wait_millisec(5);

    return true;
}

bool NearFieldCommunication::readResponse(uint8_t devAddr, uint8_t *data, uint8_t len)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite((devAddr >> 1), true))
        return false;

    uint8_t tmpData = 0;
    if (!I2C.readData(&tmpData, false))
        return false;

    for (uint8_t i=0; i<len; i++)
    {
        if (!I2C.readData(&data[i], false))
            return false;
    }

    if (!I2C.readData(&tmpData, true))
        return false;

    if (!I2C.sendStop())
        return false;

    uint8_t expectedACK[6] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    if (strncmp((char *)data, (char *)expectedACK, 6))
        return false;

    ST.wait_millisec(5);

    return true;
}

#endif
