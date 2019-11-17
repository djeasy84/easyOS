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

#ifndef DHT22_H
#define DHT22_H

class TemperatureUmidity
{
    public:
        bool setup(uint8_t pin, uint32_t refresh = 30000);

        bool read(float *hum, float *temp);

        bool update();

    private:
        uint8_t dataPin;
        uint32_t dataRefresh;
        uint32_t lastUpdate;
        bool firstDone;
        float lastHum;
        float lastTemp;
};

/****************************************************************************************/

bool TemperatureUmidity::setup(uint8_t pin, uint32_t refresh)
{
    dataPin = pin;
    dataRefresh = (refresh < 10000) ? 10000 : refresh;

    lastUpdate = 0;
    DP.read(dataPin, true);

    firstDone = false;

    lastHum = -100.0;
    lastTemp = -273.15;

    return true;
}

bool TemperatureUmidity::read(float *hum, float *temp)
{
    if (firstDone == false)
        return false;

    *hum = lastHum;
    *temp = lastTemp;
    return true;
}

bool TemperatureUmidity::update()
{
    if (ST.time_diff(ST.millisec(), lastUpdate) > dataRefresh)
    {
        lastUpdate = ST.millisec();

        DP.write(dataPin, false);
        _delay_us(5000);
        DP.read(dataPin, true);
        _delay_us(5);

        while(DP.read(dataPin, true))
        {
            if (ST.time_diff(ST.millisec(), lastUpdate) > 25)
            {
                lastHum = -100.0;
                lastTemp = -273.15;
                return false;
            }
        }
        _delay_us(5);
        while(!DP.read(dataPin, true))
        {
            if (ST.time_diff(ST.millisec(), lastUpdate) > 25)
            {
                lastHum = -100.0;
                lastTemp = -273.15;
                return false;
            }
        }
        _delay_us(5);

        while(DP.read(dataPin, true))
        {
            if (ST.time_diff(ST.millisec(), lastUpdate) > 25)
            {
                lastHum = -100.0;
                lastTemp = -273.15;
                return false;
            }
        }
        _delay_us(5);

        uint32_t tStart = 0;
        uint8_t tmpData[5] = {0, 0, 0, 0, 0};
        for (uint8_t i=0; i<5; i++)
        {
            for (uint8_t j=0; j<8; j++)
            {
                while(!DP.read(dataPin, true))
                {
                    if (ST.time_diff(ST.millisec(), lastUpdate) > 25)
                    {
                        lastHum = -100.0;
                        lastTemp = -273.15;
                        return false;
                    }
                }
                uint32_t tStart = ST.microsec();

                while(DP.read(dataPin, true))
                {
                    if (ST.time_diff(ST.millisec(), lastUpdate) > 25)
                    {
                        lastHum = -100.0;
                        lastTemp = -273.15;
                        return false;
                    }
                }
                uint32_t tStop = ST.microsec();

                uint8_t tmpDataBit = (ST.time_diff(tStop, tStart) > 35) ? true : false;

                tmpData[i] = tmpData[i] | (tmpDataBit << j);
            }
        }

        if ((tmpData[0] + tmpData[1] + tmpData[2] + tmpData[3]) != tmpData[4])
        {
            lastHum = -100.0;
            lastTemp = -273.15;
            return false;
        }

        uint16_t humidity = ((tmpData[2] << 8) | (tmpData[3] << 0)) & 0b01111111;
        lastHum = ((float)humidity) / 10.0;

        uint16_t temperature = ((tmpData[2] << 8) | (tmpData[3] << 0)) & 0b01111111;
        lastTemp = (tmpData[2] & 0b10000000) ? ((((float)temperature) * -1.0) / 10.0) : ((((float)temperature) * +1.0) / 10.0);

        firstDone = true;
    }

    return true;
}

#endif
