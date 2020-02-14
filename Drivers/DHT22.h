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

class TemperatureHumidity
{
    public:
        bool setup(uint8_t pin, uint32_t refresh = 30000);

        bool read(float *temp, float *hum);

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

bool TemperatureHumidity::setup(uint8_t pin, uint32_t refresh)
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

bool TemperatureHumidity::read(float *temp, float *hum)
{
    if (firstDone == false)
        return false;

    if (ST.time_diff(ST.millisec(), lastUpdate) < (dataRefresh * 3))
    {
        *hum = lastHum;
        *temp = lastTemp;
        return true;
    }

    return false;
}

bool TemperatureHumidity::update()
{
    if (ST.time_diff(ST.millisec(), lastUpdate) > dataRefresh)
    {
        uint8_t edgeCounter = 0;
        uint32_t edgeTime[84];  memset(edgeTime, 0, sizeof(uint32_t)*84);

        uint16_t errCounter = 0;

        DP.write(dataPin, false);
        _delay_us(5000);
        DP.write(dataPin, true);
        _delay_us(5);

        if (DP.read(dataPin, true) == false)
            return false;

        lastUpdate = ST.millisec();

        bool oldStatus = true;
        do
        {
            if (oldStatus != DP.read(dataPin, true))
            {
                oldStatus = !oldStatus;
                edgeTime[edgeCounter++] = ST.microsec();
            }

            if (errCounter++ > 1024)
                return false;

        }while(edgeCounter < 84);

        if (DP.read(dataPin, true) == false)
            return false;

        int16_t humValue = 0;
        int16_t tempValue = 0;
        for (uint8_t i=4, j=15; i<36; i+=2, j--)
            humValue = humValue | (uint16_t)((((edgeTime[i]-edgeTime[i-1]) > 50) ? (uint16_t)1 : (uint16_t)0) << j);
        for (uint8_t i=36, j=15; i<68; i+=2, j--)
            tempValue = tempValue | (uint16_t)((((edgeTime[i]-edgeTime[i-1]) > 50) ? (uint16_t)1 : (uint16_t)0) << j);

        uint8_t crcValue = 0;
        for (uint8_t i=68, j=7; i<84; i+=2, j--)
            crcValue = crcValue | (uint8_t)((((edgeTime[i]-edgeTime[i-1]) > 50) ? (uint8_t)1 : (uint8_t)0) << j);

        if (((uint8_t)((uint8_t)((humValue & 0b1111111100000000) >> 8) + (uint8_t)((humValue & 0b0000000011111111) >> 0) + (uint8_t)((tempValue & 0b1111111100000000) >> 8) + (uint8_t)((tempValue & 0b0000000011111111) >> 0))) != crcValue)
            return false;

        if ((tempValue & 0b1000000000000000) == 0b1000000000000000)
            tempValue = (tempValue & 0b011111111111111) * (-1);

        lastHum = ((float)humValue) / 10.0;
        lastTemp = ((float)tempValue) / 10.0;

        firstDone = true;
    }
    return true;
}

#endif
