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

#ifndef EASYHOME_H
#define EASYHOME_H

#define INPUT_NUM 7
#define OUTPUT_NUM 7
#define OUT010V_NUM 4

#include "./DS1307.h"
#include "./MCP2515.h"
#include "./W5500.h"
#include "./M24M02.h"

class SerialRS485 :  public SerialHW3
{
    public:
        bool write(uint8_t data);
};

/****************************************************************************************/

bool SerialRS485::write(uint8_t data)
{
    bool bResult = false;
    DP.write(EASYHOME_RS485_WE, true);
    bResult = SerialHW3::write(data);
    DP.write(EASYHOME_RS485_WE, false);
    return bResult;
}

/****************************************************************************************/
/****************************************************************************************/

class EasyHome
{
    public:
        EasyHome();

        bool setup();

        void CMD5V(bool status);
        void CMD12V(bool status);
        bool CMD5V_status();
        bool CMD12V_status();
        float CMD5V_voltage();
        float CMD12V_voltage();

        void OUTPUT(unsigned int idx, bool status);
        bool OUTPUT_status(unsigned int idx);

        bool INPUT_status(unsigned int idx);
        float INPUT_voltage(unsigned int idx);

        void OUT010V(unsigned int idx, float value);
        float OUT010V_status(unsigned int idx);

        SerialHW2 RS232();
        SerialRS485 RS485();
        RTCClock CLOCK();
        CanBus CANBUS();
        Ethernet ETHERNET();
        ExternalEEPROM EXEEPROM();

        void update();

    private:
        bool cpuLedStatus;
        uint32_t cpuLedTime;

        bool CMD5V_bStatus;
        bool CMD12V_bStatus;
        uint32_t CMD5V_sVoltage;
        uint32_t CMD12V_sVoltage;
        float CMD5V_fVoltage;
        float CMD12V_fVoltage;
        uint32_t CMD_time;
        uint16_t CMD_counter;

        bool OUTPUT_bStatus[OUTPUT_NUM];

        uint32_t INPUT_sVoltage[INPUT_NUM];
        float INPUT_fVoltage[INPUT_NUM];
        uint32_t INPUT_time;
        uint16_t INPUT_counter;

        float OUT010V_fStatus[OUT010V_NUM];

        SerialRS485 objRS485;
};

EasyHome EH;

/****************************************************************************************/

EasyHome::EasyHome()
{
    // CPU LED
    cpuLedTime = 0;
    cpuLedStatus = false;
    DP.write(EASYHOME_LED_CPU, cpuLedStatus);

    // OUTPUT 0-10V
    DP.write(EASYHOME_010V_1, false);
    DP.write(EASYHOME_010V_2, false);
    DP.write(EASYHOME_010V_3, false);
    DP.write(EASYHOME_010V_4, false);

    // RS232
    SHW2.setup();

    // RS485
    SHW3.setup();
    DP.write(EASYHOME_RS485_WE, false);

    // UART or GPIO on "EX" CONNECTOR
    //SHW1.setup();
    //DP.write(EASYHOME_EX1, false);
    //DP.write(EASYHOME_EX2, false);

    // ANALOG INPUT
    DP.read(EASYHOME_IN_1, false);
    DP.read(EASYHOME_IN_2, false);
    DP.read(EASYHOME_IN_3, false);
    DP.read(EASYHOME_IN_4, false);
    DP.read(EASYHOME_IN_5, false);
    DP.read(EASYHOME_IN_6, false);
    DP.read(EASYHOME_IN_7, false);
    DP.read(EASYHOME_IN_5V, false);
    DP.read(EASYHOME_IN_12V, false);
    AP.read(EASYHOME_IN_1);
    AP.read(EASYHOME_IN_2);
    AP.read(EASYHOME_IN_3);
    AP.read(EASYHOME_IN_4);
    AP.read(EASYHOME_IN_5);
    AP.read(EASYHOME_IN_6);
    AP.read(EASYHOME_IN_7);
    AP.read(EASYHOME_IN_5V);
    AP.read(EASYHOME_IN_12V);
    for (uint8_t i=0; i<INPUT_NUM; i++)
    {
        INPUT_sVoltage[i] = 0;
        INPUT_fVoltage[i] = 0.0;
    }
    INPUT_time = 0;
    INPUT_counter = 0;

    // 5V-12V CMD
    DP.write(EASYHOME_OUT_5V, false);
    DP.write(EASYHOME_OUT_12V, false);
    CMD5V_bStatus = CMD12V_bStatus = false;
    CMD5V_sVoltage = CMD12V_sVoltage = 0;
    CMD5V_fVoltage = CMD12V_fVoltage = 0.0;
    CMD_time = 0;
    CMD_counter  = 0;

    // OUTPUT
    DP.write(EASYHOME_OUT_1, false);
    DP.write(EASYHOME_OUT_2, false);
    DP.write(EASYHOME_OUT_3, false);
    DP.write(EASYHOME_OUT_4, false);
    DP.write(EASYHOME_OUT_5, false);
    DP.write(EASYHOME_OUT_6, false);
    DP.write(EASYHOME_OUT_7, false);
    for (uint8_t i=0; i<OUTPUT_NUM; i++)
        OUTPUT_bStatus[i] = false;

    // CAN-ETH RESET
    DP.write(EASYHOME_CAN_RESET, false);
    DP.write(EASYHOME_ETH_RESET, false);

    // CAN-ETH-SD CHIP SELECT
    DP.write(EASYHOME_CAN_CS, true);
    DP.write(EASYHOME_ETH_CS, true);
    DP.write(EASYHOME_SD_CS, true);
}

bool EasyHome::setup()
{
    // RTC
    if (!RTC.setup())
        return false;

    // ExEEPROM
    if (!ExEEPROM.setup(true))
        return false;

    // CAN-BUS
    ST.wait_millisec(250);
    DP.write(EASYHOME_CAN_RESET, true);
    ST.wait_millisec(250);
    if (!CB.setup(EASYHOME_CAN_CS, NORMAL_MODE, 500))
        return false;

    // ETH
    ST.wait_millisec(250);
    DP.write(EASYHOME_ETH_RESET, true);
    ST.wait_millisec(250);
    if (!ETH.setup(EASYHOME_ETH_CS))
        return false;
}

void EasyHome::CMD5V(bool status)
{
    CMD5V_bStatus = status;
    DP.write(EASYHOME_OUT_5V, CMD5V_bStatus);
}

void EasyHome::CMD12V(bool status)
{
    CMD12V_bStatus = status;
    DP.write(EASYHOME_OUT_12V, CMD12V_bStatus);
}

bool EasyHome::CMD5V_status()
{
    return CMD5V_bStatus;
}

bool EasyHome::CMD12V_status()
{
    return CMD12V_bStatus;
}

float EasyHome::CMD5V_voltage()
{
    return CMD5V_fVoltage;
}

float EasyHome::CMD12V_voltage()
{
    return CMD12V_fVoltage;
}

void EasyHome::OUTPUT(unsigned int idx, bool status)
{
    idx = idx - 1;
    if (idx >= OUTPUT_NUM)
        return;
    OUTPUT_bStatus[idx] = status;
    switch (idx)
    {
        case 0:
            DP.write(EASYHOME_OUT_1, OUTPUT_bStatus[idx]);
            break;
        case 1:
            DP.write(EASYHOME_OUT_2, OUTPUT_bStatus[idx]);
            break;
        case 2:
            DP.write(EASYHOME_OUT_3, OUTPUT_bStatus[idx]);
            break;
        case 3:
            DP.write(EASYHOME_OUT_4, OUTPUT_bStatus[idx]);
            break;
        case 4:
            DP.write(EASYHOME_OUT_5, OUTPUT_bStatus[idx]);
            break;
        case 5:
            DP.write(EASYHOME_OUT_6, OUTPUT_bStatus[idx]);
            break;
        case 6:
            DP.write(EASYHOME_OUT_7, OUTPUT_bStatus[idx]);
            break;
    }
}

bool EasyHome::OUTPUT_status(unsigned int idx)
{
    idx = idx - 1;
    if (idx >= OUTPUT_NUM)
        return false;
    return OUTPUT_bStatus[idx];
}

bool EasyHome::INPUT_status(unsigned int idx)
{
    idx = idx - 1;
    if (idx >= INPUT_NUM)
        return false;
    switch (idx)
    {
        case 0:
            return DP.read(EASYHOME_IN_1);
            break;
        case 1:
            return DP.read(EASYHOME_IN_2);
            break;
        case 2:
            return DP.read(EASYHOME_IN_3);
            break;
        case 3:
            return DP.read(EASYHOME_IN_4);
            break;
        case 4:
            return DP.read(EASYHOME_IN_5);
            break;
        case 5:
            return DP.read(EASYHOME_IN_6);
            break;
        case 6:
            return DP.read(EASYHOME_IN_7);
            break;
    }
}

float EasyHome::INPUT_voltage(unsigned int idx)
{
    idx = idx - 1;
    if (idx >= INPUT_NUM)
        return false;
    return INPUT_fVoltage[idx];
}

void EasyHome::OUT010V(unsigned int idx, float value)
{
    idx = idx - 1;
    if (idx >= OUT010V_NUM)
        return;
    OUT010V_fStatus[idx] = value;
    switch (idx)
    {
        case 0:
            PWM.write(EASYHOME_010V_1, OUT010V_fStatus[idx]);
            break;
        case 1:
            DP.write(EASYHOME_010V_2, (OUTPUT_bStatus[idx]>127)?true:false);
            break;
        case 2:
            DP.write(EASYHOME_010V_3, (OUTPUT_bStatus[idx]>127)?true:false);
            break;
        case 3:
            PWM.write(EASYHOME_010V_4, OUT010V_fStatus[idx]);
            break;
    }
}

float EasyHome::OUT010V_status(unsigned int idx)
{
    idx = idx - 1;
    if (idx >= OUT010V_NUM)
        return false;
    return OUT010V_fStatus[idx];
}

SerialHW2 EasyHome::RS232()
{
    return SHW2;
}

SerialRS485 EasyHome::RS485()
{
    return objRS485;
}

RTCClock EasyHome::CLOCK()
{
    return RTC;
}

CanBus EasyHome::CANBUS()
{
    return CB;
}

Ethernet EasyHome::ETHERNET()
{
    return ETH;
}

ExternalEEPROM EasyHome::EXEEPROM()
{
    return ExEEPROM;
}

/****************************************************************************************/

void EasyHome::update()
{
    if (ST.time_diff(ST.millisec(), cpuLedTime) >= 500)
    {
        cpuLedTime = ST.millisec();
        cpuLedStatus = !cpuLedStatus;
        DP.write(EASYHOME_LED_CPU, cpuLedStatus);
    }

    CMD_counter++;
    CMD5V_sVoltage += AP.read(EASYHOME_IN_5V);
    CMD12V_sVoltage += AP.read(EASYHOME_IN_12V);
    if (ST.time_diff(ST.millisec(), CMD_time) >= 250)
    {
        CMD_time = ST.millisec();
        CMD5V_fVoltage = (3.0/2.0) * ((5.0 / 1024.0) * ((float)CMD5V_sVoltage / (float)CMD_counter));
        CMD12V_fVoltage = (3.0/1.0) * ((5.0 / 1024.0) * ((float)CMD12V_sVoltage / (float)CMD_counter));
        CMD_counter = CMD5V_sVoltage = CMD12V_sVoltage = 0;
    }

    INPUT_counter++;
    INPUT_sVoltage[0] += AP.read(EASYHOME_IN_1);
    INPUT_sVoltage[1] += AP.read(EASYHOME_IN_2);
    INPUT_sVoltage[2] += AP.read(EASYHOME_IN_3);
    INPUT_sVoltage[3] += AP.read(EASYHOME_IN_4);
    INPUT_sVoltage[4] += AP.read(EASYHOME_IN_5);
    INPUT_sVoltage[5] += AP.read(EASYHOME_IN_6);
    INPUT_sVoltage[6] += AP.read(EASYHOME_IN_7);
    if (ST.time_diff(ST.millisec(), INPUT_time) >= 50)
    {
        INPUT_time = ST.millisec();
        for (uint8_t i=0; i<INPUT_NUM; i++)
        {
            INPUT_fVoltage[i] = (1.0/1.0) * ((5.0 / 1024.0) * ((float)INPUT_sVoltage[i] / (float)INPUT_counter));
            INPUT_sVoltage[i] = 0;
        }
        INPUT_counter = 0;
    }

    ST.watchdog_reset();
}

#endif
