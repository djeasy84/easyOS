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

#define MOTOR_ENABLE true
#define MOTOR_DISABLE false

#define MOTOR_CLOCKWISE false
#define MOTOR_ANTICLOCKWISE true

class StepperMotor
{
    public:
        bool setup(uint8_t ena, uint8_t dir, uint8_t pul);  //Use with "Common-Anode Connection"

        void enable(bool val);
        void direction(bool val);
        void speed(uint16_t val);

        void step();
        void steps(uint16_t num);

        void update();

    private:
        uint8_t enaPin;
        uint8_t dirPin;
        uint8_t pulPin;

        uint16_t spdVal;
        uint16_t curSpdVal;

        uint32_t lastUpdate;
};

StepperMotor STP;

/****************************************************************************************/

bool StepperMotor::setup(uint8_t ena, uint8_t dir, uint8_t pul)
{
    enaPin = ena;
    dirPin = dir;
    pulPin = pul;

    DP.write(enaPin, false);  // MOTOR_DISABLE
    DP.write(dirPin, false);  // MOTOR_CLOCKWISE
    DP.write(pulPin, false);

    spdVal = curSpdVal = 0;

    lastUpdate = 0;

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
    if (pulPin != ARDUINO_PIN_3)
    #endif
    #if defined (__BOARD_arduinoMEGA__)
    if (pulPin != ARDUINO_PIN_9)
    #endif
        return;

    spdVal = val;
}

void StepperMotor::step()
{
    if (spdVal != 0)
        return;

    DP.write(pulPin, true);
    _delay_us(500);
    DP.write(pulPin, false);
    _delay_us(500);
}

void StepperMotor::steps(uint16_t num)
{
    if (num == 0)
        return;

    for (uint16_t i=0; i<num; i++)
    {
        step();
        _delay_us(2500);
    }
}

void StepperMotor::update()
{
    if (ST.time_diff(ST.microsec(), lastUpdate) >= 1000)
    {
        lastUpdate = ST.microsec();

        if (curSpdVal < spdVal)
            curSpdVal = (curSpdVal < 200) ? (curSpdVal + 1) : (curSpdVal >= 200 && curSpdVal < 500) ? (curSpdVal + 2) : (curSpdVal + 4);
        if (curSpdVal > spdVal)
            curSpdVal = (curSpdVal < 200) ? (curSpdVal - 1) : (curSpdVal >= 200 && curSpdVal < 500) ? (curSpdVal - 4) : (curSpdVal - 8);

        PFM.write(curSpdVal);
    }
}

#endif
