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

#define TIME_M
#define MEMORY_M
#define SERIAL_HW_M
#define SERIAL_HW_1_M
#define SERIAL_HW_2_M
#define SERIAL_HW_3_M
#define SPI_M
#define I2C_M
#define INTERRUPT_M
#define COUNTER_M
#define PWM_M
#define DIGITAL_PIN_M
#define ANALOG_PIN_M
#define EEPROM_M
#define MULTI_TASKING_M
#define STATE_MACHINE_M
#define PID_M

#include "./../../easyOS.h"

StateMachine SM;
ProportionalIntegralDerivative PID(0);

bool init()
{
    ST.wait_millisec(0);
    calculateFreeMemory();
    SHW0.println("");
    SHW1.println("");
    SHW2.println("");
    SHW3.println("");
    SPI.transfer(0);
    I2C.sendStart();
    IM.enableInterrupt(0, 0, 0);
    CT.read();
    PWM.write(0, 0);
    DP.read(0);
    AP.read(0);
    EEPROM.read(0, 0);
    SM.addState(0, 0);
    PID.runPID(0,0,0,0,0,0,0,0,0,0);
    return false;
}
