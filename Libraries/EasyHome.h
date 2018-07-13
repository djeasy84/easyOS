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

class EasyHome
{
    public:
        EasyHome();

        bool update();

    private:
        bool cpuLedStatus;
        uint32_t cpuLedTime;

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

    // UART "EX" CONNECTOR
    SHW1.setup();

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

    // 5V-12V CMD
    DP.write(EASYHOME_OUT_5V, false);
    DP.write(EASYHOME_OUT_12V, false);

    // OUTPUT
    DP.write(EASYHOME_OUT_1, false);
    DP.write(EASYHOME_OUT_2, false);
    DP.write(EASYHOME_OUT_3, false);
    DP.write(EASYHOME_OUT_4, false);
    DP.write(EASYHOME_OUT_5, false);
    DP.write(EASYHOME_OUT_6, false);
    DP.write(EASYHOME_OUT_7, false);

    // CAN-ETH RESET
    DP.write(EASYHOME_CAN_RESET, false);
    DP.write(EASYHOME_ETH_RESET, false);

    // CAN-ETH-SD CHIP SELECT
    DP.write(EASYHOME_CAN_CS, true);
    DP.write(EASYHOME_ETH_CS, true);
    DP.write(EASYHOME_SD_CS, true);
}

bool EasyHome::update()
{
    if (ST.time_diff(ST.millisec(), cpuLedTime) >= 500)
    {
        cpuLedTime = ST.millisec();
        cpuLedStatus = !cpuLedStatus;
        DP.write(EASYHOME_LED_CPU, cpuLedStatus);
    }
    ST.watchdog_reset();
}

#endif
