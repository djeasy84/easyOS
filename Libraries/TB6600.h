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

#ifndef TB6600_H
#define TB6600_H

class StepperMotor
{
    public:
        bool setup(uint8_t ena, uint8_t dir, uint8_t pul);

        void enable(bool val);  // Active HIGH if used with "Common-Anode Connection"
        void direction(bool val);
        void speed(uint16_t val);

        void step();
        void steps(uint16_t val);

    private:
        uint8_t enaPin;
        uint8_t dirPin;
        uint8_t pulPin;

        uint16_t spdVal;
};

StepperMotor STP;

/****************************************************************************************/

bool StepperMotor::setup(uint8_t ena, uint8_t dir, uint8_t pul)
{
    enaPin = ena;
    dirPin = dir;
    pulPin = pul;

    DP.write(enaPin, false);
    DP.write(dirPin, true);
    DP.write(pulPin, true);

    spdVal = 0;

    return true;
}

void StepperMotor::enable(bool val)
{
    DP.write(enaPin, val);
}

void StepperMotor::direction(bool val)
{
    DP.write(dirPin, val);
}

void StepperMotor::speed(uint16_t val)
{
    #if defined (__BOARD_arduinoUNO__) || defined (__BOARD_arduinoNANO__)
    if (val != ARDUINO_PIN_3)
    #endif
    #if defined (__BOARD_arduinoMEGA__)
    if (val != ARDUINO_PIN_9)
    #endif
        return;

    spdVal = val;

    PFM.write(spdVal);
}

void StepperMotor::step()
{
    if (spdVal != 0)
        return;

    DP.write(pulPin, false);
    _delay_us(250);
    DP.write(pulPin, true);
    _delay_us(250);
}

void StepperMotor::steps(uint16_t val)
{
    for (uint16_t i=0; i<val; i++)
        step();
}

#endif
