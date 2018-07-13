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

#ifndef ANALOG_PIN_H
#define ANALOG_PIN_H

#include <avr/interrupt.h>

class AnalogPin
{
    public:
        uint16_t read(uint8_t id);
};

AnalogPin AP;

/****************************************************************************************/

#if defined (__AVR_ATmega328P__)

#if defined (__BOARD_arduinoUNO__)
    #define ARDUINO_PIN_A0 100
    #define ARDUINO_PIN_A1 101
    #define ARDUINO_PIN_A2 102
    #define ARDUINO_PIN_A3 103
    #define ARDUINO_PIN_A4 104
    #define ARDUINO_PIN_A5 105
#endif
#if defined (__BOARD_arduinoNANO__)
    #define ARDUINO_PIN_A0 100
    #define ARDUINO_PIN_A1 101
    #define ARDUINO_PIN_A2 102
    #define ARDUINO_PIN_A3 103
    #define ARDUINO_PIN_A4 104
    #define ARDUINO_PIN_A5 105
    #define ARDUINO_PIN_A6 106
    #define ARDUINO_PIN_A7 107
#endif

uint16_t AnalogPin::read(uint8_t id)
{
    ADCSRA = ADCSRB = ADMUX = 0;
    switch(id)
    {
        case 100:
        {
            ADMUX = (1<<REFS0);
        }
        break;
        case 101:
        {
            ADMUX = (1<<REFS0) | (1<<MUX0);
        }
        break;
        case 102:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1);
        }
        break;
        case 103:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1) | (1<<MUX0);
        }
        break;
        case 104:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2);
        }
        break;
        case 105:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX0);
        }
        break;
        case 106:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1);
        }
        break;
        case 107:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
        }
        break;
        default:
        {
            return 0;
        }
    }
    ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS0);
    while (ADCSRA & (1<<ADSC));
    ADCSRA = ADCSRB = ADMUX = 0;
    return ADC;
}

#endif

#if defined (__AVR_ATmega2560__)

#if defined (__BOARD_arduinoMEGA__)
    #define ARDUINO_PIN_A0 100
    #define ARDUINO_PIN_A1 101
    #define ARDUINO_PIN_A2 102
    #define ARDUINO_PIN_A3 103
    #define ARDUINO_PIN_A4 104
    #define ARDUINO_PIN_A5 105
    #define ARDUINO_PIN_A6 106
    #define ARDUINO_PIN_A7 107
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
    #define EASYHOME_IN_5V 107
    #define EASYHOME_IN_12V 100
#endif

uint16_t AnalogPin::read(uint8_t id)
{
    ADCSRA = ADCSRB = ADMUX = 0;
    switch(id)
    {
        case 100:
        {
            ADMUX = (1<<REFS0);
        }
        break;
        case 101:
        {
            ADMUX = (1<<REFS0) | (1<<MUX0);
        }
        break;
        case 102:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1);
        }
        break;
        case 103:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1) | (1<<MUX0);
        }
        break;
        case 104:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2);
        }
        break;
        case 105:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX0);
        }
        break;
        case 106:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1);
        }
        break;
        case 107:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
        }
        break;
        case 108:
        {
            ADMUX = (1<<REFS0);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 109:
        {
            ADMUX = (1<<REFS0) | (1<<MUX0);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 110:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 111:
        {
            ADMUX = (1<<REFS0) | (1<<MUX1) | (1<<MUX0);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 112:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 113:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX0);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 114:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1);
            ADCSRB = (1<<MUX5);
        }
        break;
        case 115:
        {
            ADMUX = (1<<REFS0) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
            ADCSRB = (1<<MUX5);
        }
        break;
        default:
        {
            return 0;
        }
    }
    ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS0);
    while (ADCSRA & (1<<ADSC));
    ADCSRA = ADCSRB = ADMUX = 0;
    return ADC;
}

#endif

#endif
