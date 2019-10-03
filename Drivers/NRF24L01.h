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

#ifndef NRF24L01_H
#define NRF24L01_H

#include <string.h>

class Radio
{
    public:
        bool setup(uint8_t ce, uint8_t ss, uint8_t id /* 0 to 0xFF */, uint8_t ch /* 0 to 125 */, uint8_t ml /* 1 to 32 */);

        bool read(uint8_t *data);
        bool write(uint8_t id, uint8_t *data);

    private:
        bool setup(uint8_t id, uint8_t ch);

        void getRegistry(uint8_t addr, uint8_t *value);
        void getRegistry(uint8_t addr, uint8_t len, uint8_t *value);

        bool setRegistry(uint8_t addr, uint8_t value);
        bool setRegistry(uint8_t addr, uint8_t len, uint8_t *value);

        bool checkMessage();

        void delay20MicroSeconds();

        bool isRX;
        uint8_t cePin;
        uint8_t ssPin;
        uint8_t messBytes;
};

Radio RF;

/****************************************************************************************/

bool Radio::setup(uint8_t ce, uint8_t ss, uint8_t id, uint8_t ch, uint8_t ml)
{
    cePin = ce;
    ssPin = ss;
    messBytes = ml;

    DP.write(cePin, false);
    DP.write(ssPin, true);

    return setup(id, ch);
}

bool Radio::read(uint8_t *data)
{
    if (!isRX)
    {
        if (!setRegistry(0x00, 0b00001111))
            return false;
        DP.write(ssPin, false);
        SPI.transfer(0xE1);
        SPI.transfer(0xE2);
        DP.write(ssPin, true);
        setRegistry(0x07, 0b01110000);
        DP.write(cePin, true);
        isRX = true;
    }
    if (!checkMessage())
        return false;
    DP.write(ssPin, false);
    SPI.transfer(0x61);
    for (uint8_t i=0; i<messBytes; i++)
        *(data+i) = SPI.transfer(0x00);
    DP.write(ssPin, true);
    setRegistry(0x07, 0b01110000);
    return true;
}

bool Radio::write(uint8_t id, uint8_t *data)
{
    if (isRX)
    {
        DP.write(cePin, false);
        if (!setRegistry(0x00, 0b00001110))
            return false;
        DP.write(ssPin, false);
        SPI.transfer(0xE1);
        SPI.transfer(0xE2);
        DP.write(ssPin, true);
        setRegistry(0x07, 0b01110000);
        isRX = false;
    }
    uint8_t Addr[5] = { 0x11, 0x22, 0x33, 0x44, id };
    if (!setRegistry(0x0A, 5, Addr))
        return false;
    if (!setRegistry(0x10, 5, Addr))
        return false;
    DP.write(ssPin, false);
    SPI.transfer(0xA0);
    for (uint8_t i=0; i<messBytes; i++)
        SPI.transfer(data[i]);
    DP.write(ssPin, true);
    DP.write(cePin, true);
    delay20MicroSeconds();
    DP.write(cePin, false);
    uint8_t value = 0x00;
    do
    {
        getRegistry(0x07, &value);
        if (value & 0b00010000)
        {
            DP.write(ssPin, false);
            SPI.transfer(0xE1);
            SPI.transfer(0xE2);
            DP.write(ssPin, true);
            setRegistry(0x07, 0b01110000);
            return false;
        }
    }
    while(!(value & 0b00100000));
    setRegistry(0x07, 0b01110000);
    return true;
}

bool Radio::setup(uint8_t id, uint8_t ch)
{
    if (!setRegistry(0x00, 0b00001100))
        return false;
    if (!setRegistry(0x01, 0b00000011))
        return false;
    if (!setRegistry(0x02, 0b00000011))
        return false;
    if (!setRegistry(0x03, 0b00000011))
        return false;
    if (!setRegistry(0x04, 0b00011010))
        return false;
    if (!setRegistry(0x05, ch))
        return false;
    if (!setRegistry(0x06, 0b00001110))
        return false;
    uint8_t Addr[5] = { 0x11, 0x22, 0x33, 0x44, id };
    if (!setRegistry(0x0B, 5, Addr))
        return false;
    if (!setRegistry(0x0C, Addr[4]))
        return false;
    if (!setRegistry(0x0D, Addr[4]))
        return false;
    if (!setRegistry(0x0E, Addr[4]))
        return false;
    if (!setRegistry(0x0F, Addr[4]))
        return false;
    if (!setRegistry(0x11, messBytes))
        return false;
    if (!setRegistry(0x12, messBytes))
        return false;
    if (!setRegistry(0x13, 0))
        return false;
    if (!setRegistry(0x14, 0))
        return false;
    if (!setRegistry(0x15, 0))
        return false;
    if (!setRegistry(0x16, 0))
        return false;
    if (!setRegistry(0x1C, 0b00000000))
        return false;
    if (!setRegistry(0x1D, 0b00000000))
        return false;
    if (!setRegistry(0x00, 0b00001111))
        return false;
    DP.write(ssPin, false);
    SPI.transfer(0xE1);
    SPI.transfer(0xE2);
    DP.write(ssPin, true);
    setRegistry(0x07, 0b01110000);
    DP.write(cePin, true);
    isRX = true;
    return true;
}

void Radio::getRegistry(uint8_t addr, uint8_t *value)
{
    getRegistry(addr, 1, value);
}

void Radio::getRegistry(uint8_t addr, uint8_t len, uint8_t *value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x00 | (0x1F & addr));
    for (uint8_t i=0; i<len; i++)
        *(value+i) = SPI.transfer(0x00);
    DP.write(ssPin, true);
}

bool Radio::setRegistry(uint8_t addr, uint8_t value)
{
    return setRegistry(addr, 1, &value);
}

bool Radio::setRegistry(uint8_t addr, uint8_t len, uint8_t *value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x20 | (0x1F & addr));
    for (uint8_t i=0; i<len; i++)
        SPI.transfer(value[i]);
    DP.write(ssPin, true);
    uint8_t saved_value[len];
    getRegistry(addr, len, saved_value);
    if (memcmp((void *)value, (void *)saved_value, len))
        return false;
    return true;
}

bool Radio::checkMessage()
{
    uint8_t value = 0x00;
    getRegistry(0x07, &value);
    if(value & 0b01000000)
        return true;
    getRegistry(0x17, &value);
    if(!(value & 0b00000001))
        return true;
    return false;
}

void Radio::delay20MicroSeconds()
{
    _delay_us(20);
}

#endif
