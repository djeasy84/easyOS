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

#ifndef INTERRUPT_H
#define INTERRUPT_H

#define DEBOUNCE_TIME 25000

#include "Time.h"

#include <avr/pgmspace.h>

void (*interruptFunc)(void);

ISR(PCINT0_vect)
{
    if (interruptFunc == 0x0000)
        return;
    PCICR &= ~((1<<2) | (1<<1) | (1<<0));
    _delay_us(DEBOUNCE_TIME);
    interruptFunc();
    PCICR = (1<<2) | (1<<1) | (1<<0);
}
ISR(PCINT1_vect)
{
    if (interruptFunc == 0x0000)
        return;
    PCICR &= ~((1<<2) | (1<<1) | (1<<0));
    _delay_us(DEBOUNCE_TIME);
    interruptFunc();
    PCICR = (1<<2) | (1<<1) | (1<<0);
}
ISR(PCINT2_vect)
{
    if (interruptFunc == 0x0000)
        return;
    PCICR &= ~((1<<2) | (1<<1) | (1<<0));
    _delay_us(DEBOUNCE_TIME);
    interruptFunc();
    PCICR = (1<<2) | (1<<1) | (1<<0);
}

#if defined (__AVR_ATmega328P__)
    #if defined (__BOARD_arduinoUNO__)
        #define ARDUINO_PIN_2 0
        #define ARDUINO_PIN_3 1
        #define ARDUINO_PIN_4 2
        #define ARDUINO_PIN_5 200
        #define ARDUINO_PIN_6 201
        #define ARDUINO_PIN_7 3
        #define ARDUINO_PIN_8 4
        #define ARDUINO_PIN_9 5
        #define ARDUINO_PIN_10 6
        #define ARDUINO_PIN_11 7
        #define ARDUINO_PIN_12 8
        #define ARDUINO_PIN_13 9
        #define ARDUINO_PIN_A0 100
        #define ARDUINO_PIN_A1 101
        #define ARDUINO_PIN_A2 102
        #define ARDUINO_PIN_A3 103
        #define ARDUINO_PIN_A4 104
        #define ARDUINO_PIN_A5 105
    #endif
    #if defined (__BOARD_arduinoNANO__)
        #define ARDUINO_PIN_2 0
        #define ARDUINO_PIN_3 1
        #define ARDUINO_PIN_4 2
        #define ARDUINO_PIN_5 200
        #define ARDUINO_PIN_6 201
        #define ARDUINO_PIN_7 3
        #define ARDUINO_PIN_8 4
        #define ARDUINO_PIN_9 5
        #define ARDUINO_PIN_10 6
        #define ARDUINO_PIN_11 7
        #define ARDUINO_PIN_12 8
        #define ARDUINO_PIN_13 9
        #define ARDUINO_PIN_A0 100
        #define ARDUINO_PIN_A1 101
        #define ARDUINO_PIN_A2 102
        #define ARDUINO_PIN_A3 103
        #define ARDUINO_PIN_A4 104
        #define ARDUINO_PIN_A5 105
        #define ARDUINO_PIN_A6 106
        #define ARDUINO_PIN_A7 107
    #endif
    #define DIGITAL_SIZE 10
    #define ANALOG_SIZE 8
    #define PWM_SIZE 2
    const PROGMEM uint16_t digitalListPcreg[DIGITAL_SIZE]  = { (uint16_t)&PCMSK2,
                                                               (uint16_t)&PCMSK2,
                                                               (uint16_t)&PCMSK2,
                                                               (uint16_t)&PCMSK2,
                                                               (uint16_t)&PCMSK0,
                                                               (uint16_t)&PCMSK0,
                                                               (uint16_t)&PCMSK0,
                                                               #if !defined (SPI_M)
                                                               (uint16_t)&PCMSK0,
                                                               (uint16_t)&PCMSK0,
                                                               (uint16_t)&PCMSK0,
                                                               #else
                                                               0,
                                                               0,
                                                               0,
                                                               #endif
                                                            };
    const PROGMEM uint8_t digitalListPcmask[DIGITAL_SIZE]  = { 2,
                                                               3,
                                                               4,
                                                               7,
                                                               0,
                                                               1,
                                                               2,
                                                               #if !defined (SPI_M)
                                                               3,
                                                               4,
                                                               5,
                                                               #else
                                                               0,
                                                               0,
                                                               0,
                                                               #endif
                                                            };
    const PROGMEM uint16_t analogListPcreg[DIGITAL_SIZE]   = { (uint16_t)&PCMSK1,
                                                               (uint16_t)&PCMSK1,
                                                               (uint16_t)&PCMSK1,
                                                               (uint16_t)&PCMSK1,
                                                               #if !defined (I2C_M)
                                                               (uint16_t)&PCMSK1,
                                                               (uint16_t)&PCMSK1,
                                                               0,
                                                               0,
                                                               #else
                                                               0,
                                                               0,
                                                               0,
                                                               0,
                                                               #endif
                                                            };
    const PROGMEM uint8_t analogListPcmask[DIGITAL_SIZE]   = { 0,
                                                               1,
                                                               2,
                                                               3,
                                                               #if !defined (I2C_M)
                                                               4,
                                                               5,
                                                               0,
                                                               0,
                                                               #else
                                                               0,
                                                               0,
                                                               0,
                                                               0,
                                                               #endif
                                                            };
    const PROGMEM uint16_t pwmListPcreg[DIGITAL_SIZE]      = { (uint16_t)&PCMSK2,
                                                               (uint16_t)&PCMSK2,
                                                            };
    const PROGMEM uint8_t pwmListPcmask[DIGITAL_SIZE]      = { 5,
                                                               6,
                                                            };
#endif
#if defined (__AVR_ATmega2560__)
    #if defined (__BOARD_arduinoMEGA__)
        #define ARDUINO_PIN_A8 108
        #define ARDUINO_PIN_A9 109
        #define ARDUINO_PIN_A10 110
        #define ARDUINO_PIN_A11 111
        #define ARDUINO_PIN_A12 112
        #define ARDUINO_PIN_A13 113
        #define ARDUINO_PIN_A14 114
        #define ARDUINO_PIN_A15 115
    #endif
    #if defined (__BOARD_easyHOME__)
        #define EASYHOME_IN_1 115
        #define EASYHOME_IN_2 114
        #define EASYHOME_IN_3 113
        #define EASYHOME_IN_4 112
        #define EASYHOME_IN_5 111
        #define EASYHOME_IN_6 110
        #define EASYHOME_IN_7 109
    #endif
    // TO FIX
#endif

class Interrupt
{
    public:
        void setup(void (*pFunc)(void));

        bool enable(uint8_t id);
        bool disable(uint8_t id);
};

Interrupt IM;

/****************************************************************************************/

void Interrupt::setup(void (*pFunc)(void))
{
    PCMSK0 = PCMSK1 = PCMSK2 = PCIFR = PCICR = 0;
    PCICR = (1<<2) | (1<<1) | (1<<0);

    interruptFunc = pFunc;
}

bool Interrupt::enable(uint8_t id)
{
    if (id >= 0 && id <=99)
    {
        id = id - 0;
        if (id >= DIGITAL_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(digitalListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(digitalListPcmask + (id)));
        *pcmsk |= mask;
        return true;
    }
    else if (id >= 100 && id <=199)
    {
        id = id - 100;
        if (id >= ANALOG_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(analogListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(analogListPcmask + (id)));
        *pcmsk |= mask;
        return true;
    }
    else if (id >= 200 && id <=255)
    {
        id = id - 200;
        if (id >= PWM_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(pwmListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(pwmListPcmask + (id)));
        *pcmsk |= mask;
        return true;
    }
    return false;
}

bool Interrupt::disable(uint8_t id)
{
     if (id >= 0 && id <=99)
    {
        id = id - 0;
        if (id >= DIGITAL_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(digitalListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(digitalListPcmask + (id)));
        *pcmsk &= ~mask;
        return true;
    }
    else if (id >= 100 && id <=199)
    {
        id = id - 100;
        if (id >= ANALOG_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(analogListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(analogListPcmask + (id)));
        *pcmsk &= ~mask;
        return true;
    }
    else if (id >= 200 && id <=255)
    {
        id = id - 200;
        if (id >= PWM_SIZE)
            return false;
        volatile uint8_t *pcmsk = (volatile uint8_t *)(pgm_read_word(pwmListPcreg + (id)));
        if (pcmsk == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(pwmListPcmask + (id)));
        *pcmsk &= ~mask;
        return true;
    }
    return false;
}

#endif
