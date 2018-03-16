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

#ifndef DS1307_H
#define DS1307_H

#define I2C_CLOCK_ID 0x68

#include <stdio.h>

class RTCClock
{
    public:
        bool setup();

        bool read(uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint8_t *day, uint8_t *month, uint16_t *year);
        bool write(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year);

        bool print(char *buffer);  // LEN: 20

    private:
        bool setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t data);
        bool setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len);
        bool setRegistryBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, bool value);

        bool getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
        bool getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len);

};

RTCClock RTC;

/****************************************************************************************/

bool RTCClock::setup()
{
    if (!setRegistryBit(I2C_CLOCK_ID, 0x02, 6, false))
        return false;
    return setRegistryBit(I2C_CLOCK_ID, 0x00, 7, false);
}

bool RTCClock::read(uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint8_t *day, uint8_t *month, uint16_t *year)
{
    uint8_t value = 0;
    if (!getRegistry(I2C_CLOCK_ID, 0x02, &value))
        return false;
    *hours = (((value & 0b00110000) >> 4) * 10) + (value & 0b00001111);
    if (!getRegistry(I2C_CLOCK_ID, 0x01, &value))
        return false;
    *minutes = (((value & 0b01110000) >> 4) * 10) + (value & 0b00001111);
    if (!getRegistry(I2C_CLOCK_ID, 0x00, &value))
        return false;
    *seconds = (((value & 0b01110000) >> 4) * 10) + (value & 0b00001111);
    if (!getRegistry(I2C_CLOCK_ID, 0x04, &value))
        return false;
    *day = (((value & 0b00110000) >> 4) * 10) + (value & 0b00001111);
    if (!getRegistry(I2C_CLOCK_ID, 0x05, &value))
        return false;
    *month = (((value & 0b00010000) >> 4) * 10) + (value & 0b00001111);
    if (!getRegistry(I2C_CLOCK_ID, 0x06, &value))
        return false;
    *year = (((value & 0b11110000) >> 4) * 10) + (value & 0b00001111);
    return true;
}

bool RTCClock::write(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year)
{
    uint8_t value = 0;
    value = 0b00111111 & (((hours / 10) << 4) | (hours % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x02, value))
        return false;
    value = 0b01111111 & (((minutes / 10) << 4) | (minutes % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x01, value))
        return false;
    value = 0b01111111 & (((seconds / 10) << 4) | (seconds % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x00, value))
        return false;
    value = 0b00111111 & (((day / 10) << 4) | (day % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x04, value))
        return false;
    value = 0b00011111 & (((month / 10) << 4) | (month % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x05, value))
        return false;
    value = 0b11111111 & ((((year - 2000) / 10) << 4) | ((year - 2000) % 10));
    if (!setRegistry(I2C_CLOCK_ID, 0x06, value))
        return false;
    return true;
}

bool RTCClock::print(char *buffer)
{
    uint8_t day; uint8_t month; uint16_t year; uint8_t hours; uint8_t minutes; uint8_t seconds;
    if (RTC.read(&hours, &minutes, &seconds, &day, &month, &year))
    {
        sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%04d", hours, minutes, seconds, day, month, 2000 + year);
        return true;
    }
    return false;
}

bool RTCClock::setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return setRegistry(devAddr, regAddr, &data, 1);
}

bool RTCClock::setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, false))
        return false;
    if (!I2C.sendData(regAddr))
        return false;
    if (!I2C.sendData(*data))
        return false;
    if (!I2C.sendStop())
        return false;
    return true;
}

bool RTCClock::setRegistryBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, bool value)
{
    uint8_t regValue = 0;
    if (!getRegistry(devAddr, regAddr, &regValue))
        return false;
    regValue = (value) ? (regValue | (1<<bitPos)) : (regValue & ~(1<<bitPos));
    return setRegistry(devAddr, regAddr, regValue);
}

bool RTCClock::getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
    return getRegistry(devAddr, regAddr, data, 1);
}

bool RTCClock::getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, false))
        return false;
    if (!I2C.sendData(regAddr))
        return false;
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, true))
        return false;
    for (uint8_t idx=0; idx<len; idx++)
    {
        if (!I2C.readData(&data[idx], (idx!=(len-1))?false:true))
            return false;
    }
    if (!I2C.sendStop())
        return false;
    return true;
}

#endif
