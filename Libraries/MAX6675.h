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

#ifndef MAX6675_H
#define MAX6675_H

class ThermoCouple
{
    public:
        bool setup(uint8_t ss);

        float read();

    private:
		uint16_t getData();

        uint8_t ssPin;
};

ThermoCouple TC;

/****************************************************************************************/

bool ThermoCouple::setup(uint8_t ss)
{
    ssPin = ss;

    DP.write(ssPin, true);

    return true;
}

float ThermoCouple::read()
{
	uint16_t raw_data = getData();

	if (raw_data & 0b0000000000000100)
		return -1.0;

    uint16_t raw_temp = (raw_data>>3) & 0b0000111111111111;

    return raw_temp*0.25;
}

uint16_t ThermoCouple::getData()
{
    DP.write(ssPin, false);
    uint16_t raw_data = (SPI.transfer(0x00)<<8) | (SPI.transfer(0x00)<<0);
    DP.write(ssPin, true);

	return raw_data;
}

#endif
