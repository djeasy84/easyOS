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
#define PFM_M
#define DIGITAL_PIN_M
#define ANALOG_PIN_M
#define EEPROM_M
#define MULTI_TASKING_M
#define STATE_MACHINE_M
#define PID_M

#include "easyOS.h"

StateMachine SM;
ProportionalIntegralDerivative PID(0);

#include "MPU6050.h"
#include "MCP2515.h"
#include "DS1307.h"
#include "W5500.h"
#include "NRF24L01.h"
#include "DS18B20.h"
#include "WS2812B.h"
#include "MAX6675.h"
#include "TC1602.h"
#include "M24M02.h"
#include "DMX512.h"
#include "TB6600.h"
#include "PN532.h"

Temperature DTS;

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
    IM.enableInterrupt(0);
    CT.read();
    PWM.write(0,0);
    PFM.write(0);
    DP.read(0);
    AP.read(0);
    EEPROM.read(0,0);
    SM.addState(0,0);
    PID.runPID(0,0,0,0,0,0,0,0,0,0);

    AG.read(0,0,0,0,0,0);
    CB.read(0,0,0,0);
    RTC.read(0,0,0,0,0,0);
    ETH.close();
    RF.read(0);
    DTS.update();
    LED.update();
    TC.read();
    LCD.write(0,0,0,0);
    ExEEPROM.read(0,0);
    DMX.update();
    STP.step();
    NFC.read(0);

    return false;
}
