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

#ifndef PID_H
#define PID_H

#include "Time.h"

class ProportionalIntegralDerivative
{
    public:
        ProportionalIntegralDerivative(uint32_t interval);

        float runPID(float input, float target, float kP, float kI, float kD, float maxP, float maxI, float maxD, float min, float max);

    private:
        float lastError;
        float historyError;
        float lastOutput;
        uint32_t lastTime;
        uint32_t pidInterval;
};

/****************************************************************************************/

ProportionalIntegralDerivative::ProportionalIntegralDerivative(uint32_t interval)
{
    lastError = 0.0;
    historyError = 0.0;
    lastOutput = 0.0;
    lastTime = 0;
    pidInterval = interval;
}

float ProportionalIntegralDerivative::runPID(float input, float target, float kP, float kI, float kD, float maxP, float maxI, float maxD, float min, float max)
{
    uint32_t timeDiff = ST.time_diff(ST.millisec(), lastTime);
    if (timeDiff < pidInterval)
        return lastOutput;

    lastTime = ST.millisec();
    float interval = (float)timeDiff / 1000.0;

    float error = target - input;

    float P = error * kP;
    P = (P < -maxP) ? -maxP : (P > +maxP) ? +maxP : P;

    float D = ((error - lastError) / interval) * kD;
    D = (D < -maxD) ? -maxD : (D > +maxD) ? +maxD : D;

    float I = historyError + ((error * interval) * kI);
    I = (I < -maxI) ? -maxI : (I > +maxI) ? +maxI : I;

    historyError = I;
    lastError = error;

    lastOutput = P + I + D;
    lastOutput = (lastOutput < min) ? min : (lastOutput > max) ? max : lastOutput;

    return lastOutput;
}

#endif
