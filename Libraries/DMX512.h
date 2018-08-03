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

#ifndef DMX512_H
#define DMX512_H

class DigitalMultipleX
{
    public:
        bool setup(uint8_t port, uint8_t we);

        bool write(uint16_t addr, uint8_t val);

        bool update();

    private:
        uint8_t portNUM;
        uint8_t dataLED[512];

        uint32_t lastUpdate;
};

DigitalMultipleX DMX;

/****************************************************************************************/

bool DigitalMultipleX::setup(uint8_t port, uint8_t we)
{
    portNUM = 0;
    if (port != 1 && port != 2 && port != 3)
        return false;
    DP.write(we, true);
    portNUM = port;
    switch(portNUM)
    {
        case 1:
            SHW1.setup(100000, true);
        break;
        case 2:
            SHW2.setup(100000, true);
        break;
        case 3:
            SHW3.setup(100000, true);
        break;
    }
    memset(dataLED, 0, 512);
    lastUpdate = 0;
    return true;
}

bool DigitalMultipleX::write(uint16_t addr, uint8_t val)
{
    if (addr >= 512 || portNUM == 0)
        return false;

    dataLED[addr] = val;

    return true;
}

bool DigitalMultipleX::update()
{
    if (portNUM == 0)
        return false;

	if (ST.time_diff(ST.millisec(), lastUpdate) > 40)
    {
        lastUpdate = ST.millisec();
        switch(portNUM)
        {
            case 1:
                SHW1.setup(100000, true);
            break;
            case 2:
                SHW2.setup(100000, true);
            break;
            case 3:
                SHW3.setup(100000, true);
            break;
        }
        switch(portNUM)
        {
            case 1:
                SHW1.write(0);
            break;
            case 2:
                SHW2.write(0);
            break;
            case 3:
                SHW3.write(0);
            break;
        }
        switch(portNUM)
        {
            case 1:
                SHW1.setup(250000, true);
            break;
            case 2:
                SHW2.setup(250000, true);
            break;
            case 3:
                SHW3.setup(250000, true);
            break;
        }
        for (uint16_t i=0; i<512; i++)
        {
            switch(portNUM)
            {
                case 1:
                    SHW1.write(dataLED[i]);
                break;
                case 2:
                    SHW2.write(dataLED[i]);
                break;
                case 3:
                    SHW3.write(dataLED[i]);
                break;
            }
        }
    }
    return true;
}

#endif
