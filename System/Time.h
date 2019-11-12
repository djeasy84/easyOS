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

#ifndef TIME_H
#define TIME_H

#include <math.h>
#include <stdlib.h>
#include <util/delay.h>

#include <avr/wdt.h>
#include <avr/interrupt.h>

volatile uint32_t milliseconds = 0;

ISR(TIMER1_COMPA_vect)
{
    uint32_t millisec = milliseconds;
    millisec += 8;
    milliseconds = millisec;
}

#if defined (__FCPU_16MHz__)
#define OCR1A_MAX 15999
#endif
#if defined (__FCPU_8MHz__)
#define OCR1A_MAX 7999
#endif

#define POWER_ON 0
#define EXTERNAL 1
#define BROWN_ON 2
#define WATCHDOG 3
#define JTAG 4

class Time
{
    public:
        Time();
        void enable();
        void disable();

        void restart();
        uint8_t restart_reason();

        void watchdog_reset();
        void watchdog_disable();

        uint32_t microsec();
        //_delay_us(double us);  // use this for precision delay

        uint32_t millisec();
        void wait_millisec(uint32_t value);

        uint32_t time_diff(uint32_t stop, uint32_t start);

    private:
        uint8_t restartReason;
};

Time ST;

/****************************************************************************************/

Time::Time()
{
    enable();

    wdt_enable(WDTO_8S);
}

void Time::enable()
{
    cli();

    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCNT1 = 0;

    TCCR1B |= (1<<WGM12);
    TIMSK1 |= (1<<OCIE1A);
    OCR1A = OCR1A_MAX;

    TCCR1B |= (1<<CS11);

    restartReason = MCUSR;
    MCUSR = 0;

    sei();
}

void Time::disable()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TCNT1 = 0;
}

void Time::restart()
{
    //wdt_enable(WDTO_8S);
    while(true);
}

uint8_t Time::restart_reason()
{
    if (restartReason & (1<<PORF))
        return POWER_ON;
    else if (restartReason & (1<<EXTRF))
        return EXTERNAL;
    else if (restartReason & (1<<BORF))
        return BROWN_ON;
    else if (restartReason & (1<<WDRF))
        return WATCHDOG;
    #if defined (__AVR_ATmega2560__)
    else if (restartReason & (1<<JTRF))
        return JTAG;
    #endif
    else
        return 0xFF;  // NOT_VALID
}

void Time::watchdog_reset()
{
    wdt_reset();
}

void Time::watchdog_disable()
{
    wdt_disable();
}

uint32_t Time::microsec()
{
    uint8_t oldSREG = SREG;
    cli();

    uint32_t microsec = milliseconds * 1000;
    uint32_t counter = TCNT1;
    if ((TIFR1 & (1<<OCF1A)) && (counter < OCR1A_MAX))
        microsec += 8000;

    SREG = oldSREG;  //sei();

    return microsec + (uint32_t)round((float)counter * (float)((float)8000 / (float)OCR1A_MAX));
}

uint32_t Time::millisec()
{
    uint8_t oldSREG = SREG;
    cli();

    uint32_t millisec = milliseconds;
    uint32_t counter = TCNT1;
    if ((TIFR1 & (1<<OCF1A)) && (counter < OCR1A_MAX))
        millisec += 8;

    SREG = oldSREG;  //sei();

    return millisec + (uint32_t)round((float)(((float)((float)counter * (float)((float)8000 / (float)OCR1A_MAX))) / (float)1000));
}

void Time::wait_millisec(uint32_t value)
{
    uint32_t start = millisec();
    while((millisec() - start) < value)
        wdt_reset();
}

uint32_t Time::time_diff(uint32_t stop, uint32_t start)
{
    if (stop >= start)
        return stop - start;
    return (0xFFFFFFFF - start) + stop;
}

#endif
