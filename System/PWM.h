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

#ifndef PWM_H
#define PWM_H

#include <avr/interrupt.h>

class PulseWidthModulation
{
    public:
        PulseWidthModulation();

        bool setup(uint16_t freq);

        void write(uint8_t id, uint8_t value);
};

PulseWidthModulation PWM;

/****************************************************************************************/

#if defined (__AVR_ATmega328P__)

#if defined (__BOARD_arduinoUNO__)
    #define ARDUINO_PIN_5 200
    #define ARDUINO_PIN_6 201
#endif

PulseWidthModulation::PulseWidthModulation()
{
    #if !defined (COUNTER_M)
    DDRD |= (1<<5);
    DDRD |= (1<<6);

    setup(1000);

    write(200, 0);
    write(201, 0);
    #endif
}

bool PulseWidthModulation::setup(uint16_t freq)
{
    switch(freq)
    {
        #if !defined (COUNTER_M)
        case 125:
        {
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000100;
        }
        break;
        case 250:
        {
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000100;
        }
        break;
        case 500:
        {
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000011;
        }
        break;
        case 1000:
        {
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000011;
        }
        break;
        case 4000:
        {
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000010;
        }
        break;
        case 8000:
        {
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000010;
        }
        break;
        case 30000:
        {
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000001;
        }
        break;
        case 60000:
        {
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000001;
        }
        break;
        #endif
        default:
        {
            return false;
        }
        break;
    }
    return true;
}

void PulseWidthModulation::write(uint8_t id, uint8_t value)
{
    switch(id)
    {
        #if !defined (COUNTER_M)
        case 200:
        {
            if (value == 0)
            {
                TCCR0A &= ~(0b00110000);
                PORTD &= ~(1<<5);
            }
            else if (value == 255)
            {
                TCCR0A &= ~(0b00110000);
                PORTD |= (1<<5);
            }
            else
            {
                TCCR0A |= 0b00100000;
                OCR0B = value;
            }
        }
        break;
        case 201:
        {
            if (value == 0)
            {
                TCCR0A &= ~(0b11000000);
                PORTD &= ~(1<<6);
            }
            else if (value == 255)
            {
                TCCR0A &= ~(0b11000000);
                PORTD |= (1<<6);
            }
            else
            {
                TCCR0A |= 0b10000000;
                OCR0A = value;
            }
        }
        break;
        #endif
    }
}

#endif

#if defined (__AVR_ATmega2560__)

#if defined (__BOARD_arduinoMEGA__)
    #define ARDUINO_PIN_2 200
    #define ARDUINO_PIN_3 201
    #define ARDUINO_PIN_4 202
    #define ARDUINO_PIN_5 203
    #define ARDUINO_PIN_6 204
    #define ARDUINO_PIN_7 205
    #define ARDUINO_PIN_8 206
    #define ARDUINO_PIN_13 207
    #define ARDUINO_PIN_44 210
    #define ARDUINO_PIN_45 209
    #define ARDUINO_PIN_46 208
#endif
#if defined (__BOARD_easyHOME__)
    #define EASYHOME_010V_1 209
    #define EASYHOME_010V_2 208
    #define EASYHOME_010V_3 207
    #define EASYHOME_010V_4 210
#endif

PulseWidthModulation::PulseWidthModulation()
{
	#if !defined (COUNTER_M)
    DDRG |= (1<<5);
	#endif
	#if !defined (COUNTER_M)
    DDRB |= (1<<7);
	#endif
    DDRE |= (1<<3);
    DDRE |= (1<<4);
    DDRE |= (1<<5);
    DDRH |= (1<<3);
    DDRH |= (1<<4);
    DDRH |= (1<<5);
    DDRL |= (1<<3);
    DDRL |= (1<<4);
    DDRL |= (1<<5);

    setup(1000);

    write(200, 0);
    write(201, 0);
	#if !defined (COUNTER_M)
    write(202, 0);
	#endif
    write(203, 0);
    write(204, 0);
    write(205, 0);
    write(206, 0);
	#if !defined (COUNTER_M)
    write(207, 0);
	#endif
    write(208, 0);
    write(209, 0);
    write(210, 0);
}

bool PulseWidthModulation::setup(uint16_t freq)
{
    switch(freq)
    {
        case 125:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000100;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00000100;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00000100;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00000100;
        }
        break;
        case 250:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000100;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00001100;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00001100;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00001100;
        }
        break;
        case 500:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000011;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00000011;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00000011;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00000011;
        }
        break;
        case 1000:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000011;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00001011;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00001011;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00001011;
        }
        break;
        case 4000:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000010;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00000010;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00000010;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00000010;
        }
        break;
        case 8000:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000010;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00001010;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00001010;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00001010;
        }
        break;
        case 30000:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000001;
            TCCR0B = 0b00000001;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00000001;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00000001;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00000001;
        }
        break;
        case 60000:
        {
			#if !defined (COUNTER_M)
            TCCR0A = 0b00000011;
            TCCR0B = 0b00000001;
			#endif
            TCCR3A = 0b00000001;
            TCCR3B = 0b00001001;

            TCCR4A = 0b00000001;
            TCCR4B = 0b00001001;

            TCCR5A = 0b00000001;
            TCCR5B = 0b00001001;
        }
        break;
        default:
        {
            return false;
        }
        break;
    }
    return true;
}

void PulseWidthModulation::write(uint8_t id, uint8_t value)
{
    switch(id)
    {
        case 200:
        {
            if (value == 0)
            {
                TCCR3A &= ~(0b00110000);
                PORTE &= ~(1<<4);
            }
            else if (value == 255)
            {
                TCCR3A &= ~(0b00110000);
                PORTE |= (1<<4);
            }
            else
            {
                TCCR3A |= 0b00100000;
                OCR3B = value;
            }
        }
        break;
        case 201:
        {
            if (value == 0)
            {
                TCCR3A &= ~(0b00001100);
                PORTE &= ~(1<<5);
            }
            else if (value == 255)
            {
                TCCR3A &= ~(0b00001100);
                PORTE |= (1<<5);
            }
            else
            {
                TCCR3A |= 0b00001000;
                OCR3C = value;
            }
        }
        break;
        case 202:
        {
			#if !defined (COUNTER_M)
            if (value == 0)
            {
                TCCR0A &= ~(0b00110000);
                PORTG &= ~(1<<5);
            }
            else if (value == 255)
            {
                TCCR0A &= ~(0b00110000);
                PORTG |= (1<<5);
            }
            else
            {
                TCCR0A |= 0b00100000;
                OCR0B = value;
            }
			#endif
        }
        break;
        case 203:
        {
            if (value == 0)
            {
                TCCR3A &= ~(0b11000000);
                PORTE &= ~(1<<3);
            }
            else if (value == 255)
            {
                TCCR3A &= ~(0b11000000);
                PORTE |= (1<<3);
            }
            else
            {
                TCCR3A |= 0b10000000;
                OCR3A = value;
            }
        }
        break;
        case 204:
        {
            if (value == 0)
            {
                TCCR4A &= ~(0b11000000);
                PORTH &= ~(1<<3);
            }
            else if (value == 255)
            {
                TCCR4A &= ~(0b11000000);
                PORTH |= (1<<3);
            }
            else
            {
                TCCR4A |= 0b10000000;
                OCR4A = value;
            }
        }
        break;
        case 205:
        {
            if (value == 0)
            {
                TCCR4A &= ~(0b00110000);
                PORTH &= ~(1<<4);
            }
            else if (value == 255)
            {
                TCCR4A &= ~(0b00110000);
                PORTH |= (1<<4);
            }
            else
            {
                TCCR4A |= 0b00100000;
                OCR4B = value;
            }
        }
        break;
        case 206:
        {
            if (value == 0)
            {
                TCCR4A &= ~(0b00001100);
                PORTH &= ~(1<<5);
            }
            else if (value == 255)
            {
                TCCR4A &= ~(0b00001100);
                PORTH |= (1<<5);
            }
            else
            {
                TCCR4A |= 0b00001000;
                OCR4C = value;
            }
        }
        break;
        case 207:
        {
			#if !defined (COUNTER_M)
            if (value == 0)
            {
                TCCR0A &= ~(0b11000000);
                PORTB &= ~(1<<7);
            }
            else if (value == 255)
            {
                TCCR0A &= ~(0b11000000);
                PORTB |= (1<<7);
            }
            else
            {
                TCCR0A |= 0b10000000;
                OCR0A = value;
            }
			#endif
        }
        break;
        case 208:
        {
            if (value == 0)
            {
                TCCR5A &= ~(0b11000000);
                PORTL &= ~(1<<3);
            }
            else if (value == 255)
            {
                TCCR5A &= ~(0b11000000);
                PORTL |= (1<<3);
            }
            else
            {
                TCCR5A |= 0b10000000;
                OCR5A = value;
            }
        }
        break;
        case 209:
        {
            if (value == 0)
            {
                TCCR5A &= ~(0b00110000);
                PORTL &= ~(1<<4);
            }
            else if (value == 255)
            {
                TCCR5A &= ~(0b00110000);
                PORTL |= (1<<4);
            }
            else
            {
                TCCR5A |= 0b00100000;
                OCR5B = value;
            }
        }
        break;
        case 210:
        {
            if (value == 0)
            {
                TCCR5A &= ~(0b00001100);
                PORTL &= ~(1<<5);
            }
            else if (value == 255)
            {
                TCCR5A &= ~(0b00001100);
                PORTL |= (1<<5);
            }
            else
            {
                TCCR5A |= 0b00001000;
                OCR5C = value;
            }
        }
        break;
    }
}

#endif

#endif
