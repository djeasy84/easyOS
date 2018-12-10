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
#define DIGITAL_PIN_M
#define ANALOG_PIN_M
#define SPI_M
#define SERIAL_HW_M
#define SERIAL_HW_2_M

#include "./../../easyOS.h"

#include "./../../Libraries/W5500.h"

int main()
{
    SHW2.setup(115200);

    // ETHERNET
    DP.write(EASYHOME_CAN_RESET, false);
    DP.write(EASYHOME_ETH_RESET, true);
    ST.wait_millisec(250);
    if (!ETH.setup(EASYHOME_ETH_CS))
        return false;

    // CPU LED
    uint32_t cpuLedTime = 0;
    bool cpuLedStatus = false;
    DP.write(EASYHOME_LED_CPU, cpuLedStatus);

    // OUTPUT SETUP (AS OUTPUT, LOW STATE)
    DP.write(EASYHOME_OUT_1, false);
    DP.write(EASYHOME_OUT_2, false);
    DP.write(EASYHOME_OUT_3, false);
    DP.write(EASYHOME_OUT_4, false);
    DP.write(EASYHOME_OUT_5, false);
    DP.write(EASYHOME_OUT_6, false);
    DP.write(EASYHOME_OUT_7, false);
    DP.write(EASYHOME_OUT_12V, false);

    // INPUT SETUP (AS INPUT, NO PULLUP)
    DP.read(EASYHOME_IN_1, false);
    DP.read(EASYHOME_IN_2, false);
    DP.read(EASYHOME_IN_3, false);
    DP.read(EASYHOME_IN_4, false);
    DP.read(EASYHOME_IN_5, false);
    DP.read(EASYHOME_IN_6, false);
    DP.read(EASYHOME_IN_7, false);
    DP.write(EASYHOME_OUT_5V, true);

    while(true)
    {
        ST.watchdog_reset();

        // CPU LED
        if (ST.time_diff(ST.millisec(), cpuLedTime) > 500)
        {
            cpuLedTime = ST.millisec();
            cpuLedStatus = !cpuLedStatus;
            DP.write(EASYHOME_LED_CPU, cpuLedStatus);
        }

        if (ETH.status() == WAIT_CLOSE)
            ETH.close();
        if (ETH.status() == CLOSED)
            ETH.openServer(50000);
        if (ETH.status() == CONNECTED)
        {
            // COMMAND
            uint8_t cCMD = 0;
            if (ETH.available() < 1)
                continue;
            if (!ETH.read(1, &cCMD))
                continue;
            switch(cCMD)
            {
                // OUTPUT
                case 'O':
                {
                    uint8_t cOUT = 0;
                    if (!ETH.read(1, &cOUT))
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    if (cOUT < '0' || cOUT > '7')
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    uint8_t cSTATE = 0;
                    if (!ETH.read(1, &cSTATE))
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    if (cSTATE != '0' && cSTATE != '1')
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    uint8_t iOUT = cOUT-'0';
                    bool bOUT = (cSTATE == '1') ? true : false;
                    switch (iOUT)
                    {
                        case 0:
                            DP.write(EASYHOME_OUT_12V, bOUT);
                            break;
                        case 1:
                            DP.write(EASYHOME_OUT_1, bOUT);
                            break;
                        case 2:
                            DP.write(EASYHOME_OUT_2, bOUT);
                            break;
                        case 3:
                            DP.write(EASYHOME_OUT_3, bOUT);
                            break;
                        case 4:
                            DP.write(EASYHOME_OUT_4, bOUT);
                            break;
                        case 5:
                            DP.write(EASYHOME_OUT_5, bOUT);
                            break;
                        case 6:
                            DP.write(EASYHOME_OUT_6, bOUT);
                            break;
                        case 7:
                            DP.write(EASYHOME_OUT_7, bOUT);
                            break;
                    }
                    ETH.write(1, (uint8_t *)"O");
                    ETH.write(1, &cOUT);
                    ETH.write(1, &cSTATE);
                    ETH.write(2, (uint8_t *)"\r\n");
                }
                break;
                case 'I':
                {
                    uint8_t cIN = 0;
                    if (!ETH.read(1, &cIN))
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    if (cIN < '1' || cIN > '7')
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    uint8_t cSTATE = 0;
                    uint8_t iIN = cIN-'0';
                    switch (iIN)
                    {
                        case 1:
                            cSTATE = DP.read(EASYHOME_IN_1);
                            break;
                        case 2:
                            cSTATE = DP.read(EASYHOME_IN_2);
                            break;
                        case 3:
                            cSTATE = DP.read(EASYHOME_IN_3);
                            break;
                        case 4:
                            cSTATE = DP.read(EASYHOME_IN_4);
                            break;
                        case 5:
                            cSTATE = DP.read(EASYHOME_IN_5);
                            break;
                        case 6:
                            cSTATE = DP.read(EASYHOME_IN_6);
                            break;
                        case 7:
                            cSTATE = DP.read(EASYHOME_IN_7);
                            break;
                    }
                    ETH.write(1, (uint8_t *)"I");
                    ETH.write(1, &cIN);
                    ETH.write(1, (cSTATE)?(uint8_t *)"1":(uint8_t *)"0");
                    ETH.write(2, (uint8_t *)"\r\n");
                }
                break;
                case 'S':
                {
                    uint8_t cDATA = 0;
                    if (!ETH.read(1, &cDATA))
                    {
                        ETH.write(3, (uint8_t *)"E\r\n");
                        break;
                    }
                    SHW2.write(cDATA);
                    ETH.write(1, (uint8_t *)"S");
                    ETH.write(1, &cDATA);
                    ETH.write(2, (uint8_t *)"\r\n");
                }
                break;
            }
            uint8_t len = 0;
            uint8_t cDATA = 0;
            if (SHW2.read(&cDATA, &len, 1))
            {
                if (len == 1)
                {
                    ETH.write(1, (uint8_t *)"R");
                    ETH.write(1, &cDATA);
                    ETH.write(2, (uint8_t *)"\r\n");
                }
            }
        }
    }
    return 0;
}
