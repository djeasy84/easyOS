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

#define DATA_SIZE 10

class GasSensor135
{
    public:
        bool setup(uint8_t pin);

        bool read(float *val);

        bool update();

    private:
        uint8_t dataPin;
        uint32_t lastUpdate;
        bool firstDone;
        uint8_t countValue;
        uint16_t dataValue[DATA_SIZE];
};

/****************************************************************************************/

bool GasSensor135::setup(uint8_t pin)
{
    dataPin = pin;

    lastUpdate = 0;
    firstDone = false;

    countValue = 0;
    memset(dataValue, 0, sizeof(uint16_t)*DATA_SIZE);

    return true;
}

bool GasSensor135::read(float *value)
{
    if (firstDone == false)
        return false;

    uint32_t sumValue = 0;
    for (uint8_t i=0; i<DATA_SIZE; i++)
        sumValue = sumValue + dataValue[i];

    float charactCurve[3] =  {log10(10), log10(2.1), (log10(0.8)-log10(2.1))/(log10(200)-log10(10))};  // CO2 - Normally ~415ppm

    float RsRoValue = (float)(((1023.0 / (float)((uint32_t)sumValue / (uint32_t)DATA_SIZE)) - 1.0) * 20000.0) / 16500.0;

    *value = (float)(pow(10, (((log10(RsRoValue)-charactCurve[1])/charactCurve[2])+charactCurve[0])));

    return true;
}

bool GasSensor135::update()
{
    if (ST.time_diff(ST.millisec(), lastUpdate) > ((countValue == 0 && firstDone == false) ? 2500/*60000*/ : 100))
    {
        lastUpdate = ST.millisec();

        dataValue[countValue++] = AP.read(dataPin);
        if (countValue >= DATA_SIZE)
        {
            countValue = 0;
            firstDone = true;
        }
    }
    return true;
}

#endif
