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

#ifndef PFM_H
#define PFM_H

#include <avr/interrupt.h>

class PulseFrequencyModulation
{
    public:
        PulseFrequencyModulation();

        void write(uint32_t value);
};

PulseFrequencyModulation PFM;

/****************************************************************************************/

PulseFrequencyModulation::PulseFrequencyModulation()
{
    #if !defined (COUNTER_M)
    #if defined (__AVR_ATmega328P__)
    DDRD |= (1<<3);
    #endif
    #if defined (__AVR_ATmega2560__)
    DDRH |= (1<<6);
    #endif

    write(0);
    #endif
}

void PulseFrequencyModulation::write(uint32_t value)
{
    #if !defined (COUNTER_M)
    if (value == 0)
    {
        TCCR2A = 0b00000010;
        TCCR2B = 0b00000000;  // Timer 2 OFF
        OCR2A = 0;
    }
    else if (value > 0 && value <= 30)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000111;  // Timer 2 Prescaler 1024 (Minimum Possible Frequency Value: 30 Hz)
        OCR2A = 255;
    }
    else if (value >= 31 && value <= 122)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000111;  // Timer 2 Prescaler 1024 (Frequency: From 31 To 122 Hz)
        OCR2A = (F_CPU - (value*2*1024))/((value*2*1024));
    }
    else if (value >= 123 && value <= 488)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000110;  // Timer 2 Prescaler 256 (Frequency: From 123 To 488 Hz)
        OCR2A = (F_CPU - (value*2*256))/((value*2*256));
    }
    else if (value >= 489 && value <= 3906)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000100;  // Timer 2 Prescaler 64 (Frequency: From 489 To 3906 Hz)
        OCR2A = (F_CPU - (value*2*64))/((value*2*64));
    }
    else if (value >= 3907 && value <= 31250)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000010;  // Timer 2 Prescaler 8 (Frequency: From 3907 To 31250 Hz)
        OCR2A = (F_CPU - (value*2*8))/((value*2*8));
    }
    else if (value >= 31251 && value <= 250000)
    {
        TCCR2A = 0b00010010;
        TCCR2B = 0b00000001;  // Timer 2 Prescaler 1 (Frequency: From 31251 To 250000 Hz)
        OCR2A = OCR2A = (F_CPU - (value*2*1))/((value*2*1));
    }
    #endif
}

#endif
