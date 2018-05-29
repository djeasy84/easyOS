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
#define SPI_M
#define I2C_M
#define INTERRUPT_M
#define PWM_M
#define DIGITAL_PIN_M
#define ANALOG_PIN_M
#define EEPROM_M
#define MULTI_TASKING_M
#define PID_M
#define STATE_MACHINE_M

#include "./../../easyOS.h"

StateMachine SM;
ProportionalIntegralDerivative PID;

#include "./../../Libraries/ModBus.h"
#include "./../../Libraries/MPU6050.h"
#include "./../../Libraries/MCP2515.h"
#include "./../../Libraries/DS1307.h"
#include "./../../Libraries/W5100.h"
#include "./../../Libraries/NRF24L01.h"
#include "./../../Libraries/DS18B20.h"
#include "./../../Libraries/WS2812B.h"
#include "./../../Libraries/MAX6675.h"
#include "./../../Libraries/TC1602.h"

bool init()
{
    return false;
}
