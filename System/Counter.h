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

#ifndef COUNTER_H
#define COUNTER_H

#include <avr/interrupt.h>

volatile bool counter_ok = false;
volatile uint8_t counter_div = 0;
volatile uint8_t counter_val = 0;
volatile uint8_t counter_count = 0;

ISR(TIMER0_OVF_vect)
{
    counter_val = counter_count = TCNT0 = 0;
    counter_ok = false;
}
ISR(TIMER2_COMPA_vect)
{
    if (++counter_count == counter_div)
    {
        counter_val = TCNT0;
        counter_count = TCNT0 = 0;
        counter_ok = true;
    }
}

class Counter
{
    public:
        Counter();

        bool setup(uint16_t max_freq=2000);

        unsigned int read();

    private:
        uint8_t freq_multiplier;
};

Counter CT;

/****************************************************************************************/

Counter::Counter()
{
    setup();
}

bool Counter::setup(uint16_t max_freq)
{
    switch(max_freq)
    {
        case 250:
        {
            counter_div = 248;  // 1000ms - 1Hz to 250Hz
            freq_multiplier = 1;
        }
        break;
        case 500:
        {
            counter_div = 124;  // 500ms - 2Hz to 500Hz
            freq_multiplier = 2;
        }
        break;
        case 1000:
        {
            counter_div = 62;  // 250ms - 4Hz to 1000Hz
            freq_multiplier = 4;
        }
        break;
        case 2000:
        {
            counter_div = 31;  // 125ms - 8Hz to 2000Hz
            freq_multiplier = 8;
        }
        break;
        default:
        {
            return false;
        }
        break;
    }

    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    TCNT0 = 0;

    TCCR0B |= (1<<CS02) | (1<<CS01) | (1<<CS00);  // Pin T0 as Clock
    TIMSK0 |= (1<<TOIE0);
    TIFR0 |= (1<<TOV0);

    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    TCNT2 = 0;

    TCCR2A |= (1<<WGM21);
    TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);  // 248Hz
    TIMSK2 |= (1<<OCIE2A);
    OCR2A = 62;

    return true;
}

unsigned int Counter::read()
{
    return (counter_ok) ? (uint16_t)counter_val * (uint16_t)freq_multiplier : 0;
}

#endif
