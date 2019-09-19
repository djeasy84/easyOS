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

#ifndef SPI_H
#define SPI_H

#include <avr/interrupt.h>

class SerialPeripheralInterface
{
    public:
        SerialPeripheralInterface();

        uint8_t transfer(uint8_t data);
};

SerialPeripheralInterface SPI;

/****************************************************************************************/

SerialPeripheralInterface::SerialPeripheralInterface()
{
    #if defined (__AVR_ATmega328P__)
    DDRB &= ~(1<<4);
    DDRB |= (1<<2);
    DDRB |= (1<<3);
    DDRB |= (1<<5);
    #endif
    #if defined (__AVR_ATmega2560__)
    DDRB &= ~(1<<3);
    DDRB |= (1<<0);
    DDRB |= (1<<1);
    DDRB |= (1<<2);
    #endif

    SPSR = 0;
	SPSR |= (1<<SPI2X);

    SPCR = 0;
    SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);

    // CLOCK SPEED 2MHz
}

uint8_t SerialPeripheralInterface::transfer(uint8_t data)
{
    SPDR = data;
    while((SPSR & (1<<SPIF)) == 0);
    return SPDR;
}

#endif
