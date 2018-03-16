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

#include "./Time.h"

#include <stdlib.h>

class ProportionalIntegralDerivative
{
    public:
        ProportionalIntegralDerivative();

        float runPID(float input, float target, float kP, float kI, float kD, float min, float max);

    private:
        float lastError;
        float historyError;
        uint32_t lastTime;
};

/****************************************************************************************/

ProportionalIntegralDerivative::ProportionalIntegralDerivative()
{
    lastError = 0.0;
    historyError = 0.0;
    lastTime = 0;
}

float ProportionalIntegralDerivative::runPID(float input, float target, float kP, float kI, float kD, float min, float max)
{
    float interval = labs(ST.microsec() - lastTime) / 1000000.0;
    lastTime = ST.microsec();

    float error = target - input;

    float P = error * kP;
    P = (P < min) ? min : (P > max) ? max : P;

    float D = ((error - lastError) / interval) * kD;
    D = (D < min) ? min : (D > max) ? max : D;

    float I = (historyError + (error * interval)) * kI;
    I = (I < min) ? min : (I > max) ? max : I;

    if ((P + I + D) > min && (P + I + D) < max)
        historyError = historyError + (error * interval);

    lastError = error;

    float output = P + I + D;
    output = (output < min) ? min : (output > max) ? max : output;

    return output;
}

#endif
