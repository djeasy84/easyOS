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
#define SERIAL_HW_M
#define ANALOG_PIN_M

#include "easyOS.h"

#include "MQ2.h"
#include "MQ135.h"

int main()
{
    SHW0.setup(9600);

    GasSensor2 GS1;
    GasSensor135 GS2;
    GS1.setup(ARDUINO_PIN_A5);
    GS2.setup(ARDUINO_PIN_A0);

    uint32_t timeSerial = ST.millisec();

    while(true)
    {
        if (ST.time_diff(ST.millisec(), timeSerial) > 500)
        {
            timeSerial = ST.millisec();

            float val1 = 0.0, val2 = 0.0;
            if (GS1.read(&val1) && GS2.read(&val2))
            {
                SHW0.print("CH4: ");
                SHW0.println(val1);
                SHW0.print("CO2: ");
                SHW0.println(val2);
                SHW0.println();
            }
        }

        GS1.update();
        GS2.update();

        // ----------------------
        ST.watchdog_reset();
    }
    return 0;
}
