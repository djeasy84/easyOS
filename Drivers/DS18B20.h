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

#ifndef DS18B20_H
#define DS18B20_H

class Temperature
{
    public:
        bool setup(uint8_t pin, uint32_t refresh = 15000);

        bool read(float *temp);

        bool update();

    private:
        bool reset();

        void writeBit(bool dataB);
        bool readBit();

        void writeByte(uint8_t data);
        uint8_t readByte();

        uint8_t dataPin;
        uint32_t tempRefresh;
        uint32_t lastUpdate;
        bool tempReaded;
        bool firstDone;
        float lastTemp;
};

/****************************************************************************************/

bool Temperature::setup(uint8_t pin, uint32_t refresh)
{
    dataPin = pin;
    tempRefresh = (refresh < 5000) ? 5000 : refresh;

    lastUpdate = 0;
    DP.read(dataPin, true);

    tempReaded = false;
    firstDone = false;

    lastTemp = -273.15;

    return reset();
}

bool Temperature::read(float *temp)
{
    if (firstDone == false)
        return false;

    uint32_t currTime = ST.millisec();
    if (ST.time_diff(currTime, lastUpdate) < 1000)
    {
        *temp = lastTemp;

        return true;
    }

    if (ST.time_diff(currTime, lastUpdate) < (tempRefresh * 3))
    {
        if (tempReaded == false)
        {
            if (!reset())
                return false;

            uint8_t oldSREG = SREG;
            cli();

            writeByte(0xCC);
            writeByte(0xBE);

            uint8_t scratchpad[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (uint8_t i=0; i<2/*9*/; i++)
                scratchpad[i] = readByte();

            SREG = oldSREG;  //sei();

            uint16_t tempI = (scratchpad[1]<<8) | scratchpad[0];

            *temp = lastTemp = (float)tempI / 16.0;

            tempReaded = true;
        }
        else
        {
            *temp = lastTemp;
        }

        return true;
    }

    return false;
}

bool Temperature::update()
{
    if (ST.time_diff(ST.millisec(), lastUpdate) > tempRefresh)
    {
        if (!reset())
            return false;

        lastUpdate = ST.millisec();

        uint8_t oldSREG = SREG;
        cli();

        writeByte(0xCC);
        writeByte(0x44);

        SREG = oldSREG;  //sei();

        tempReaded = false;

        firstDone = true;
    }

    return true;
}

bool Temperature::reset()
{
    DP.write(dataPin, false);
    _delay_us(580);
    DP.read(dataPin, true);
    _delay_us(120);
    bool res = !DP.read(dataPin, true);
    _delay_us(460);
    return res;
}

void Temperature::writeBit(bool dataB)
{
    if (dataB)
    {
        DP.write(dataPin, false);
        _delay_us(2);
        DP.read(dataPin, true);
        _delay_us(78);
    }
    else
    {
        DP.write(dataPin, false);
        _delay_us(78);
        DP.read(dataPin, true);
        _delay_us(2);
    }
}

bool Temperature::readBit()
{
    DP.write(dataPin, false);
    _delay_us(2);
    DP.read(dataPin, true);
    _delay_us(8);
    uint8_t dataB = DP.read(dataPin, true);
    _delay_us(70);
    return dataB;
}

void Temperature::writeByte(uint8_t data)
{
    for (uint8_t i=0; i<8; i++)
        writeBit(data & (1<<i));
}

uint8_t Temperature::readByte()
{
    uint8_t data = 0;
    for (uint8_t i=0; i<8; i++)
        data = data | (readBit() << i);
    return data;
}

#endif
