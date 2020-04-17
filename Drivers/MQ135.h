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

#ifndef MQ135_H
#define MQ135_H

#include <string.h>

#define AQS_DATA_SIZE 25

class AirQualitySensor
{
    public:
        bool setup(uint8_t pin);

        bool read(uint8_t *val);
        bool leakDetect();

        bool update();

    private:
        uint8_t dataPin;
        uint32_t lastUpdate;
        bool firstDone;
        uint8_t countValue;
        uint8_t dataValue[AQS_DATA_SIZE];
        uint8_t lastAverage;
        bool leakDetected;
};

/****************************************************************************************/

bool AirQualitySensor::setup(uint8_t pin)
{
    dataPin = pin;

    lastUpdate = 0;
    firstDone = false;

    countValue = 0;
    memset(dataValue, 0, AQS_DATA_SIZE);
    lastAverage = 0;

    leakDetected = false;

    return true;
}

bool AirQualitySensor::read(uint8_t *value)
{
    if (firstDone == false)
        return false;

    if (lastAverage >= 0 && lastAverage < 80)  // Good
        *value = 0;
    else if (lastAverage >= 80 && lastAverage < 120)  // Medium
        *value = 1;
    else if (lastAverage >= 120 && lastAverage < 140)  // Bad
        *value = 2;
    else  // Very Bad - Emergency
        *value = 3;

    *value = lastAverage;

    return true;
}

bool AirQualitySensor::leakDetect()
{
    if (firstDone == false)
        return false;

    return leakDetected;
}

bool AirQualitySensor::update()
{
    if (ST.time_diff(ST.millisec(), lastUpdate) > ((countValue == 0 && firstDone == false) ? 100/*120000*/ : 100))
    {
        lastUpdate = ST.millisec();

        dataValue[countValue++] = AP.read(dataPin) / 4;
        if (countValue >= AQS_DATA_SIZE)
        {
            countValue = 0;
            firstDone = true;
        }

        if (firstDone == true)
        {
            uint16_t sumValue = 0;
            for (uint8_t i=0; i<AQS_DATA_SIZE; i++)
                sumValue = sumValue + dataValue[i];

            lastAverage = sumValue / (uint16_t)AQS_DATA_SIZE;

            leakDetected = true;

            if (lastAverage < 150)
                leakDetected = false;

            if (lastAverage >= 150 || abs(dataValue[countValue] - lastAverage) >= 10)
                leakDetected = true;
        }
    }
    return true;
}

#endif
