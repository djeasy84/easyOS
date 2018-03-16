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

#ifndef DIGITAL_PIN_H
#define DIGITAL_PIN_H

#include <avr/interrupt.h>

class DigitalPin
{
    public:
        bool read(uint8_t id, bool pullup=false);
        void write(uint8_t id, bool status);
};

DigitalPin DP;

/****************************************************************************************/

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
#endif

bool DigitalPin::read(uint8_t id, bool pullup)
{
    switch(id)
    {
        case 0:
        {
            DDRD &= ~(1<<2);
            if (pullup)
                PORTD |= (1<<2);
            else
                PORTD &= ~(1<<2);
            asm("nop");
            if(PIND & (1<<2))
                return true;
        }
        break;
        case 1:
        {
            DDRD &= ~(1<<3);
            if (pullup)
                PORTD |= (1<<3);
            else
                PORTD &= ~(1<<3);
            asm("nop");
            if(PIND & (1<<3))
                return true;
        }
        break;
        case 2:
        {
            DDRD &= ~(1<<4);
            if (pullup)
                PORTD |= (1<<4);
            else
                PORTD &= ~(1<<4);
            asm("nop");
            if(PIND & (1<<4))
                return true;
        }
        break;
        case 3:
        {
            DDRD &= ~(1<<7);
            if (pullup)
                PORTD |= (1<<7);
            else
                PORTD &= ~(1<<7);
            asm("nop");
            if(PIND & (1<<7))
                return true;
        }
        break;
        case 4:
        {
            DDRB &= ~(1<<0);
            if (pullup)
                PORTB |= (1<<0);
            else
                PORTB &= ~(1<<0);
            asm("nop");
            if(PINB & (1<<0))
                return true;
        }
        break;
        case 5:
        {
            DDRB &= ~(1<<1);
            if (pullup)
                PORTB |= (1<<1);
            else
                PORTB &= ~(1<<1);
            asm("nop");
            if(PINB & (1<<1))
                return true;
        }
        break;
        case 6:
        {
            DDRB &= ~(1<<2);
            if (pullup)
                PORTB |= (1<<2);
            else
                PORTB &= ~(1<<2);
            asm("nop");
            if(PINB & (1<<2))
                return true;
        }
        break;
        case 7:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<3);
            if (pullup)
                PORTB |= (1<<3);
            else
                PORTB &= ~(1<<3);
            asm("nop");
            if(PINB & (1<<3))
                return true;
            #endif
        }
        break;
        case 8:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<4);
            if (pullup)
                PORTB |= (1<<4);
            else
                PORTB &= ~(1<<4);
            asm("nop");
            if(PINB & (1<<4))
                return true;
            #endif
        }
        break;
        case 9:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<5);
            if (pullup)
                PORTB |= (1<<5);
            else
                PORTB &= ~(1<<5);
            asm("nop");
            if(PINB & (1<<5))
                return true;
            #endif
        }
        break;
        case 100:
        {
            DDRC &= ~(1<<0);
            if (pullup)
                PORTC |= (1<<0);
            else
                PORTC &= ~(1<<0);
            asm("nop");
            if(PINC & (1<<0))
                return true;
        }
        break;
        case 101:
        {
            DDRC &= ~(1<<1);
            if (pullup)
                PORTC |= (1<<1);
            else
                PORTC &= ~(1<<1);
            asm("nop");
            if(PINC & (1<<1))
                return true;
        }
        break;
        case 102:
        {
            DDRC &= ~(1<<2);
            if (pullup)
                PORTC |= (1<<2);
            else
                PORTC &= ~(1<<2);
            asm("nop");
            if(PINC & (1<<2))
                return true;
        }
        break;
        case 103:
        {
            DDRC &= ~(1<<3);
            if (pullup)
                PORTC |= (1<<3);
            else
                PORTC &= ~(1<<3);
            asm("nop");
            if(PINC & (1<<3))
                return true;
        }
        break;
        case 104:
        {
            #if !defined (I2C_M)
            DDRC &= ~(1<<4);
            if (pullup)
                PORTC |= (1<<4);
            else
                PORTC &= ~(1<<4);
            asm("nop");
            if(PINC & (1<<4))
                return true;
            #endif
        }
        break;
        case 105:
        {
            #if !defined (I2C_M)
            DDRC &= ~(1<<5);
            if (pullup)
                PORTC |= (1<<5);
            else
                PORTC &= ~(1<<5);
            asm("nop");
            if(PINC & (1<<5))
                return true;
            #endif
        }
        break;
        case 200:
        {
            #if !defined (PWM_M)
            DDRD &= ~(1<<5);
            if (pullup)
                PORTD |= (1<<5);
            else
                PORTD &= ~(1<<5);
            asm("nop");
            if(PIND & (1<<5))
                return true;
            #endif
        }
        break;
        case 201:
        {
            #if !defined (PWM_M)
            DDRD &= ~(1<<6);
            if (pullup)
                PORTD |= (1<<6);
            else
                PORTD &= ~(1<<6);
            asm("nop");
            if(PIND & (1<<6))
                return true;
            #endif
        }
        break;

    }
    return false;
}

void DigitalPin::write(uint8_t id, bool status)
{
    switch(id)
    {
        case 0:
        {
            DDRD |= (1<<2);
            if (status)
                PORTD |= (1<<2);
            else
                PORTD &= ~(1<<2);
        }
        break;
        case 1:
        {
            DDRD |= (1<<3);
            if (status)
                PORTD |= (1<<3);
            else
                PORTD &= ~(1<<3);
        }
        break;
        case 2:
        {
            DDRD |= (1<<4);
            if (status)
                PORTD |= (1<<4);
            else
                PORTD &= ~(1<<4);
        }
        break;
        case 3:
        {
            DDRD |= (1<<7);
            if (status)
                PORTD |= (1<<7);
            else
                PORTD &= ~(1<<7);
        }
        break;
        case 4:
        {
            DDRB |= (1<<0);
            if (status)
                PORTB |= (1<<0);
            else
                PORTB &= ~(1<<0);
        }
        break;
        case 5:
        {
            DDRB |= (1<<1);
            if (status)
                PORTB |= (1<<1);
            else
                PORTB &= ~(1<<1);
        }
        break;
        case 6:
        {
            DDRB |= (1<<2);
            if (status)
                PORTB |= (1<<2);
            else
                PORTB &= ~(1<<2);
        }
        break;
        case 7:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<3);
            if (status)
                PORTB |= (1<<3);
            else
                PORTB &= ~(1<<3);
            #endif
        }
        break;
        case 8:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<4);
            if (status)
                PORTB |= (1<<4);
            else
                PORTB &= ~(1<<4);
            #endif
        }
        break;
        case 9:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<5);
            if (status)
                PORTB |= (1<<5);
            else
                PORTB &= ~(1<<5);
            #endif
        }
        break;
        case 100:
        {
            DDRC |= (1<<0);
            if (status)
                PORTC |= (1<<0);
            else
                PORTC &= ~(1<<0);
        }
        break;
        case 101:
        {
            DDRC |= (1<<1);
            if (status)
                PORTC |= (1<<1);
            else
                PORTC &= ~(1<<1);
        }
        break;
        case 102:
        {
            DDRC |= (1<<2);
            if (status)
                PORTC |= (1<<2);
            else
                PORTC &= ~(1<<2);
        }
        break;
        case 103:
        {
            DDRC |= (1<<3);
            if (status)
                PORTC |= (1<<3);
            else
                PORTC &= ~(1<<3);
        }
        break;
        case 104:
        {
            #if !defined (I2C_M)
            DDRC |= (1<<4);
            if (status)
                PORTC |= (1<<4);
            else
                PORTC &= ~(1<<4);
            #endif
        }
        break;
        case 105:
        {
            #if !defined (I2C_M)
            DDRC |= (1<<5);
            if (status)
                PORTC |= (1<<5);
            else
                PORTC &= ~(1<<5);
            #endif
        }
        break;
        case 200:
        {
            #if !defined (PWM_M)
            DDRD |= (1<<5);
            if (status)
                PORTD |= (1<<5);
            else
                PORTD &= ~(1<<5);
            #endif
        }
        break;
        case 201:
        {
            #if !defined (PWM_M)
            DDRD |= (1<<6);
            if (status)
                PORTD |= (1<<6);
            else
                PORTD &= ~(1<<6);
            #endif
        }
        break;
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
    #define ARDUINO_PIN_9 46
    #define ARDUINO_PIN_10 47
    #define ARDUINO_PIN_11 0
    #define ARDUINO_PIN_12 1
    #define ARDUINO_PIN_13 207
    #define ARDUINO_PIN_14 35
    #define ARDUINO_PIN_15 36
    #define ARDUINO_PIN_16 37
    #define ARDUINO_PIN_17 38
    #define ARDUINO_PIN_18 39
    #define ARDUINO_PIN_19 40
    #define ARDUINO_PIN_20 41
    #define ARDUINO_PIN_21 42
    #define ARDUINO_PIN_22 2
    #define ARDUINO_PIN_23 3
    #define ARDUINO_PIN_24 4
    #define ARDUINO_PIN_25 5
    #define ARDUINO_PIN_26 6
    #define ARDUINO_PIN_27 7
    #define ARDUINO_PIN_28 8
    #define ARDUINO_PIN_29 9
    #define ARDUINO_PIN_30 10
    #define ARDUINO_PIN_31 11
    #define ARDUINO_PIN_32 12
    #define ARDUINO_PIN_33 13
    #define ARDUINO_PIN_34 14
    #define ARDUINO_PIN_35 15
    #define ARDUINO_PIN_36 16
    #define ARDUINO_PIN_37 17
    #define ARDUINO_PIN_38 18
    #define ARDUINO_PIN_39 19
    #define ARDUINO_PIN_40 20
    #define ARDUINO_PIN_41 21
    #define ARDUINO_PIN_42 22
    #define ARDUINO_PIN_43 23
    #define ARDUINO_PIN_44 210
    #define ARDUINO_PIN_45 209
    #define ARDUINO_PIN_46 208
    #define ARDUINO_PIN_47 24
    #define ARDUINO_PIN_48 25
    #define ARDUINO_PIN_49 26
    #define ARDUINO_PIN_50 43
    #define ARDUINO_PIN_51 44
    #define ARDUINO_PIN_52 45
    #define ARDUINO_PIN_53 27
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
    #define EASYHOME_DIP_1 9
    #define EASYHOME_DIP_2 8
    #define EASYHOME_DIP_3 7
    #define EASYHOME_DIP_4 6
    #define EASYHOME_WE_485 34
    #define EASYHOME_CE_RF 4
    #define EASYHOME_CS_RF 5
    #define EASYHOME_CS_SD 3
    #define EASYHOME_CS_CB 27
    #define EASYHOME_CS_ETH 2
    #define EASYHOME_AUX_1 33
    #define EASYHOME_AUX_2 32
    #define EASYHOME_AUX_3 31
    #define EASYHOME_AUX_4 36
    #define EASYHOME_AUX_5 35
    #define EASYHOME_AUX_6 30
    #define EASYHOME_LED_CPU 18
    #define EASYHOME_IN_1 113
    #define EASYHOME_IN_2 112
    #define EASYHOME_IN_3 111
    #define EASYHOME_IN_4 110
    #define EASYHOME_IN_5 109
    #define EASYHOME_IN_6 108
    #define EASYHOME_IN_7 107
    #define EASYHOME_IN_8 106
    #define EASYHOME_IN_9 105
    #define EASYHOME_IN_10 104
    #define EASYHOME_IN_11 103
    #define EASYHOME_IN_12 102
    #define EASYHOME_IN_13 101
    #define EASYHOME_IN_14 100
    #define EASYHOME_IN_15 28
    #define EASYHOME_IN_16 29
    #define EASYHOME_IN_12V 115
    #define EASYHOME_OUT_1 15
    #define EASYHOME_OUT_2 16
    #define EASYHOME_OUT_3 17
    #define EASYHOME_OUT_4 210
    #define EASYHOME_OUT_5 209
    #define EASYHOME_OUT_6 208
    #define EASYHOME_OUT_7 47
    #define EASYHOME_OUT_8 207
    #define EASYHOME_OUT_9 46
    #define EASYHOME_OUT_10 206
    #define EASYHOME_OUT_11 205
    #define EASYHOME_OUT_12 204
    #define EASYHOME_OUT_13 201
    #define EASYHOME_OUT_14 200
    #define EASYHOME_OUT_15 203
    #define EASYHOME_OUT_16 202
#endif

bool DigitalPin::read(uint8_t id, bool pullup)
{
    switch(id)
    {
        case 0:
        {
            DDRB &= ~(1<<5);
            if (pullup)
                PORTB |= (1<<5);
            else
                PORTB &= ~(1<<5);
            asm("nop");
            if(PINB & (1<<5))
                return true;
        }
        break;
        case 1:
        {
            DDRB &= ~(1<<6);
            if (pullup)
                PORTB |= (1<<6);
            else
                PORTB &= ~(1<<6);
            asm("nop");
            if(PINB & (1<<6))
                return true;
        }
        break;
        case 2:
        {
            DDRA &= ~(1<<0);
            if (pullup)
                PORTA |= (1<<0);
            else
                PORTA &= ~(1<<0);
            asm("nop");
            if(PINA & (1<<0))
                return true;
        }
        break;
        case 3:
        {
            DDRA &= ~(1<<1);
            if (pullup)
                PORTA |= (1<<1);
            else
                PORTA &= ~(1<<1);
            asm("nop");
            if(PINA & (1<<1))
                return true;
        }
        break;
        case 4:
        {
            DDRA &= ~(1<<2);
            if (pullup)
                PORTA |= (1<<2);
            else
                PORTA &= ~(1<<2);
            asm("nop");
            if(PINA & (1<<2))
                return true;
        }
        break;
        case 5:
        {
            DDRA &= ~(1<<3);
            if (pullup)
                PORTA |= (1<<3);
            else
                PORTA &= ~(1<<3);
            asm("nop");
            if(PINA & (1<<3))
                return true;
        }
        break;
        case 6:
        {
            DDRA &= ~(1<<4);
            if (pullup)
                PORTA |= (1<<4);
            else
                PORTA &= ~(1<<4);
            asm("nop");
            if(PINA & (1<<4))
                return true;
        }
        break;
        case 7:
        {
            DDRA &= ~(1<<5);
            if (pullup)
                PORTA |= (1<<5);
            else
                PORTA &= ~(1<<5);
            asm("nop");
            if(PINA & (1<<5))
                return true;
        }
        break;
        case 8:
        {
            DDRA &= ~(1<<6);
            if (pullup)
                PORTA |= (1<<6);
            else
                PORTA &= ~(1<<6);
            asm("nop");
            if(PINA & (1<<6))
                return true;
        }
        break;
        case 9:
        {
            DDRA &= ~(1<<7);
            if (pullup)
                PORTA |= (1<<7);
            else
                PORTA &= ~(1<<7);
            asm("nop");
            if(PINA & (1<<7))
                return true;
        }
        break;
        case 10:
        {
            DDRC &= ~(1<<7);
            if (pullup)
                PORTC |= (1<<7);
            else
                PORTC &= ~(1<<7);
            asm("nop");
            if(PINC & (1<<7))
                return true;
        }
        break;
        case 11:
        {
            DDRC &= ~(1<<6);
            if (pullup)
                PORTC |= (1<<6);
            else
                PORTC &= ~(1<<6);
            asm("nop");
            if(PINC & (1<<6))
                return true;
        }
        break;
        case 12:
        {
            DDRC &= ~(1<<5);
            if (pullup)
                PORTC |= (1<<5);
            else
                PORTC &= ~(1<<5);
            asm("nop");
            if(PINC & (1<<5))
                return true;
        }
        break;
        case 13:
        {
            DDRC &= ~(1<<4);
            if (pullup)
                PORTC |= (1<<4);
            else
                PORTC &= ~(1<<4);
            asm("nop");
            if(PINC & (1<<4))
                return true;
        }
        break;
        case 14:
        {
            DDRC &= ~(1<<3);
            if (pullup)
                PORTC |= (1<<3);
            else
                PORTC &= ~(1<<3);
            asm("nop");
            if(PINC & (1<<3))
                return true;
        }
        break;
        case 15:
        {
            DDRC &= ~(1<<2);
            if (pullup)
                PORTC |= (1<<2);
            else
                PORTC &= ~(1<<2);
            asm("nop");
            if(PINC & (1<<2))
                return true;
        }
        break;
        case 16:
        {
            DDRC &= ~(1<<1);
            if (pullup)
                PORTC |= (1<<1);
            else
                PORTC &= ~(1<<1);
            asm("nop");
            if(PINC & (1<<1))
                return true;
        }
        break;
        case 17:
        {
            DDRC &= ~(1<<0);
            if (pullup)
                PORTC |= (1<<0);
            else
                PORTC &= ~(1<<0);
            asm("nop");
            if(PINC & (1<<0))
                return true;
        }
        break;
        case 18:
        {
            DDRD &= ~(1<<7);
            if (pullup)
                PORTD |= (1<<7);
            else
                PORTD &= ~(1<<7);
            asm("nop");
            if(PIND & (1<<7))
                return true;
        }
        break;
        case 19:
        {
            DDRG &= ~(1<<2);
            if (pullup)
                PORTG |= (1<<2);
            else
                PORTG &= ~(1<<2);
            asm("nop");
            if(PING & (1<<2))
                return true;
        }
        break;
        case 20:
        {
            DDRG &= ~(1<<1);
            if (pullup)
                PORTG |= (1<<1);
            else
                PORTG &= ~(1<<1);
            asm("nop");
            if(PING & (1<<1))
                return true;
        }
        break;
        case 21:
        {
            DDRG &= ~(1<<0);
            if (pullup)
                PORTG |= (1<<0);
            else
                PORTG &= ~(1<<0);
            asm("nop");
            if(PING & (1<<0))
                return true;
        }
        break;
        case 22:
        {
            DDRL &= ~(1<<7);
            if (pullup)
                PORTL |= (1<<7);
            else
                PORTL &= ~(1<<7);
            asm("nop");
            if(PINL & (1<<7))
                return true;
        }
        break;
        case 23:
        {
            DDRL &= ~(1<<6);
            if (pullup)
                PORTL |= (1<<6);
            else
                PORTL &= ~(1<<6);
            asm("nop");
            if(PINL & (1<<6))
                return true;
        }
        break;
        case 24:
        {
            DDRL &= ~(1<<2);
            if (pullup)
                PORTL |= (1<<2);
            else
                PORTL &= ~(1<<2);
            asm("nop");
            if(PINL & (1<<2))
                return true;
        }
        break;
        case 25:
        {
            DDRL &= ~(1<<1);
            if (pullup)
                PORTL |= (1<<1);
            else
                PORTL &= ~(1<<1);
            asm("nop");
            if(PINL & (1<<1))
                return true;
        }
        break;
        case 26:
        {
            DDRL &= ~(1<<0);
            if (pullup)
                PORTL |= (1<<0);
            else
                PORTL &= ~(1<<0);
            asm("nop");
            if(PINL & (1<<0))
                return true;
        }
        break;
        case 27:
        {
            DDRB &= ~(1<<0);
            if (pullup)
                PORTB |= (1<<0);
            else
                PORTB &= ~(1<<0);
            asm("nop");
            if(PINB & (1<<0))
                return true;
        }
        break;
        case 28:
        {
            DDRE &= ~(1<<6);
            if (pullup)
                PORTE |= (1<<6);
            else
                PORTE &= ~(1<<6);
            asm("nop");
            if(PINE & (1<<6))
                return true;
        }
        break;
        case 29:
        {
            DDRE &= ~(1<<7);
            if (pullup)
                PORTE |= (1<<7);
            else
                PORTE &= ~(1<<7);
            asm("nop");
            if(PINE & (1<<7))
                return true;
        }
        break;
        case 30:
        {
            DDRJ &= ~(1<<2);
            if (pullup)
                PORTJ |= (1<<2);
            else
                PORTJ &= ~(1<<2);
            asm("nop");
            if(PINJ & (1<<2))
                return true;
        }
        break;
        case 31:
        {
            DDRJ &= ~(1<<3);
            if (pullup)
                PORTJ |= (1<<3);
            else
                PORTJ &= ~(1<<3);
            asm("nop");
            if(PINJ & (1<<3))
                return true;
        }
        break;
        case 32:
        {
            DDRJ &= ~(1<<4);
            if (pullup)
                PORTJ |= (1<<4);
            else
                PORTJ &= ~(1<<4);
            asm("nop");
            if(PINJ & (1<<4))
                return true;
        }
        break;
        case 33:
        {
            DDRJ &= ~(1<<5);
            if (pullup)
                PORTJ |= (1<<5);
            else
                PORTJ &= ~(1<<5);
            asm("nop");
            if(PINJ & (1<<5))
                return true;
        }
        break;
        case 34:
        {
            DDRD &= ~(1<<4);
            if (pullup)
                PORTD |= (1<<4);
            else
                PORTD &= ~(1<<4);
            asm("nop");
            if(PIND & (1<<4))
                return true;
        }
        break;
        case 35:
        {
            #if !defined (SERIAL_HW_3_M)
            DDRJ &= ~(1<<1);
            if (pullup)
                PORTJ |= (1<<1);
            else
                PORTJ &= ~(1<<1);
            asm("nop");
            if(PINJ & (1<<1))
                return true;
            #endif
        }
        break;
        case 36:
        {
            #if !defined (SERIAL_HW_3_M)
            DDRJ &= ~(1<<0);
            if (pullup)
                PORTJ |= (1<<0);
            else
                PORTJ &= ~(1<<0);
            asm("nop");
            if(PINJ & (1<<0))
                return true;
            #endif
        }
        break;
        case 37:
        {
            #if !defined (SERIAL_HW_2_M)
            DDRH &= ~(1<<1);
            if (pullup)
                PORTH |= (1<<1);
            else
                PORTH &= ~(1<<1);
            asm("nop");
            if(PINH & (1<<1))
                return true;
            #endif
        }
        break;
        case 38:
        {
            #if !defined (SERIAL_HW_2_M)
            DDRH &= ~(1<<0);
            if (pullup)
                PORTH |= (1<<0);
            else
                PORTH &= ~(1<<0);
            asm("nop");
            if(PINH & (1<<0))
                return true;
            #endif
        }
        break;
        case 39:
        {
            #if !defined (SERIAL_HW_1_M)
            DDRD &= ~(1<<3);
            if (pullup)
                PORTD |= (1<<3);
            else
                PORTD &= ~(1<<3);
            asm("nop");
            if(PIND & (1<<3))
                return true;
            #endif
        }
        break;
        case 40:
        {
            #if !defined (SERIAL_HW_1_M)
            DDRD &= ~(1<<2);
            if (pullup)
                PORTD |= (1<<2);
            else
                PORTD &= ~(1<<2);
            asm("nop");
            if(PIND & (1<<2))
                return true;
            #endif
        }
        break;
        case 41:
        {
            #if !defined (I2C_M)
            DDRD &= ~(1<<1);
            if (pullup)
                PORTD |= (1<<1);
            else
                PORTD &= ~(1<<1);
            asm("nop");
            if(PIND & (1<<1))
                return true;
            #endif
        }
        break;
        case 42:
        {
            #if !defined (I2C_M)
            DDRD &= ~(1<<0);
            if (pullup)
                PORTD |= (1<<0);
            else
                PORTD &= ~(1<<0);
            asm("nop");
            if(PIND & (1<<0))
                return true;
            #endif
        }
        break;
        case 43:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<3);
            if (pullup)
                PORTB |= (1<<3);
            else
                PORTB &= ~(1<<3);
            asm("nop");
            if(PINB & (1<<3))
                return true;
            #endif
        }
        break;
        case 44:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<2);
            if (pullup)
                PORTB |= (1<<2);
            else
                PORTB &= ~(1<<2);
            asm("nop");
            if(PINB & (1<<2))
                return true;
            #endif
        }
        break;
        case 45:
        {
            #if !defined (SPI_M)
            DDRB &= ~(1<<1);
            if (pullup)
                PORTB |= (1<<1);
            else
                PORTB &= ~(1<<1);
            asm("nop");
            if(PINB & (1<<1))
                return true;
            #endif
        }
        break;
        case 46:
        {
            DDRH &= ~(1<<6);
            if (pullup)
                PORTH |= (1<<6);
            else
                PORTH &= ~(1<<6);
            asm("nop");
            if(PINH & (1<<6))
                return true;
        }
        break;
        case 47:
        {
            DDRB &= ~(1<<4);
            if (pullup)
                PORTB |= (1<<4);
            else
                PORTB &= ~(1<<4);
            asm("nop");
            if(PINB & (1<<4))
                return true;
        }
        break;
        case 100:
        {
            DDRF &= ~(1<<0);
            if (pullup)
                PORTF |= (1<<0);
            else
                PORTF &= ~(1<<0);
            asm("nop");
            if(PINF & (1<<0))
                return true;
        }
        break;
        case 101:
        {
            DDRF &= ~(1<<1);
            if (pullup)
                PORTF |= (1<<1);
            else
                PORTF &= ~(1<<1);
            asm("nop");
            if(PINF & (1<<1))
                return true;
        }
        break;
        case 102:
        {
            DDRF &= ~(1<<2);
            if (pullup)
                PORTF |= (1<<2);
            else
                PORTF &= ~(1<<2);
            asm("nop");
            if(PINF & (1<<2))
                return true;
        }
        break;
        case 103:
        {
            DDRF &= ~(1<<3);
            if (pullup)
                PORTF |= (1<<3);
            else
                PORTF &= ~(1<<3);
            asm("nop");
            if(PINF & (1<<3))
                return true;
        }
        break;
        case 104:
        {
            DDRF &= ~(1<<4);
            if (pullup)
                PORTF |= (1<<4);
            else
                PORTF &= ~(1<<4);
            asm("nop");
            if(PINF & (1<<4))
                return true;
        }
        break;
        case 105:
        {
            DDRF &= ~(1<<5);
            if (pullup)
                PORTF |= (1<<5);
            else
                PORTF &= ~(1<<5);
            asm("nop");
            if(PINF & (1<<5))
                return true;
        }
        break;
        case 106:
        {
            DDRF &= ~(1<<6);
            if (pullup)
                PORTF |= (1<<6);
            else
                PORTF &= ~(1<<6);
            asm("nop");
            if(PINF & (1<<6))
                return true;
        }
        break;
        case 107:
        {
            DDRF &= ~(1<<7);
            if (pullup)
                PORTF |= (1<<7);
            else
                PORTF &= ~(1<<7);
            asm("nop");
            if(PINF & (1<<7))
                return true;
        }
        break;
        case 108:
        {
            DDRK &= ~(1<<0);
            if (pullup)
                PORTK |= (1<<0);
            else
                PORTK &= ~(1<<0);
            asm("nop");
            if(PINK & (1<<0))
                return true;
        }
        break;
        case 109:
        {
            DDRK &= ~(1<<1);
            if (pullup)
                PORTK |= (1<<1);
            else
                PORTK &= ~(1<<1);
            asm("nop");
            if(PINK & (1<<1))
                return true;
        }
        break;
        case 110:
        {
            DDRK &= ~(1<<2);
            if (pullup)
                PORTK |= (1<<2);
            else
                PORTK &= ~(1<<2);
            asm("nop");
            if(PINK & (1<<2))
                return true;
        }
        break;
        case 111:
        {
            DDRK &= ~(1<<3);
            if (pullup)
                PORTK |= (1<<3);
            else
                PORTK &= ~(1<<3);
            asm("nop");
            if(PINK & (1<<3))
                return true;
        }
        break;
        case 112:
        {
            DDRK &= ~(1<<4);
            if (pullup)
                PORTK |= (1<<4);
            else
                PORTK &= ~(1<<4);
            asm("nop");
            if(PINK & (1<<4))
                return true;
        }
        break;
        case 113:
        {
            DDRK &= ~(1<<5);
            if (pullup)
                PORTK |= (1<<5);
            else
                PORTK &= ~(1<<5);
            asm("nop");
            if(PINK & (1<<5))
                return true;
        }
        break;
        case 114:
        {
            DDRK &= ~(1<<6);
            if (pullup)
                PORTK |= (1<<6);
            else
                PORTK &= ~(1<<6);
            asm("nop");
            if(PINK & (1<<6))
                return true;
        }
        break;
        case 115:
        {
            DDRK &= ~(1<<7);
            if (pullup)
                PORTK |= (1<<7);
            else
                PORTK &= ~(1<<7);
            asm("nop");
            if(PINK & (1<<7))
                return true;
        }
        break;
        case 200:
        {
            #if !defined (PWM_M)
            DDRE &= ~(1<<4);
            if (pullup)
                PORTE |= (1<<4);
            else
                PORTE &= ~(1<<4);
            asm("nop");
            if(PINE & (1<<4))
                return true;
            #endif
        }
        break;
        case 201:
        {
            #if !defined (PWM_M)
            DDRE &= ~(1<<5);
            if (pullup)
                PORTE |= (1<<5);
            else
                PORTE &= ~(1<<5);
            asm("nop");
            if(PINE & (1<<5))
                return true;
            #endif
        }
        break;
        case 202:
        {
            #if !defined (PWM_M)
            DDRG &= ~(1<<5);
            if (pullup)
                PORTG |= (1<<5);
            else
                PORTG &= ~(1<<5);
            asm("nop");
            if(PING & (1<<5))
                return true;
            #endif
        }
        break;
        case 203:
        {
            #if !defined (PWM_M)
            DDRE &= ~(1<<3);
            if (pullup)
                PORTE |= (1<<3);
            else
                PORTE &= ~(1<<3);
            asm("nop");
            if(PINE & (1<<3))
                return true;
            #endif
        }
        break;
        case 204:
        {
            #if !defined (PWM_M)
            DDRH &= ~(1<<3);
            if (pullup)
                PORTH |= (1<<3);
            else
                PORTH &= ~(1<<3);
            asm("nop");
            if(PINH & (1<<3))
                return true;
            #endif
        }
        break;
        case 205:
        {
            #if !defined (PWM_M)
            DDRH &= ~(1<<4);
            if (pullup)
                PORTH |= (1<<4);
            else
                PORTH &= ~(1<<4);
            asm("nop");
            if(PINH & (1<<4))
                return true;
            #endif
        }
        break;
        case 206:
        {
            #if !defined (PWM_M)
            DDRH &= ~(1<<5);
            if (pullup)
                PORTH |= (1<<5);
            else
                PORTH &= ~(1<<5);
            asm("nop");
            if(PINH & (1<<5))
                return true;
            #endif
        }
        break;
        case 207:
        {
            #if !defined (PWM_M)
            DDRB &= ~(1<<7);
            if (pullup)
                PORTB |= (1<<7);
            else
                PORTB &= ~(1<<7);
            asm("nop");
            if(PINB & (1<<7))
                return true;
            #endif
        }
        break;
        case 208:
        {
            #if !defined (PWM_M)
            DDRL &= ~(1<<3);
            if (pullup)
                PORTL |= (1<<3);
            else
                PORTL &= ~(1<<3);
            asm("nop");
            if(PINL & (1<<3))
                return true;
            #endif
        }
        break;
        case 209:
        {
            #if !defined (PWM_M)
            DDRL &= ~(1<<4);
            if (pullup)
                PORTL |= (1<<4);
            else
                PORTL &= ~(1<<4);
            asm("nop");
            if(PINL & (1<<4))
                return true;
            #endif
        }
        break;
        case 210:
        {
            #if !defined (PWM_M)
            DDRL &= ~(1<<5);
            if (pullup)
                PORTL |= (1<<5);
            else
                PORTL &= ~(1<<5);
            asm("nop");
            if(PINL & (1<<5))
                return true;
            #endif
        }
        break;
    }
    return false;
}

void DigitalPin::write(uint8_t id, bool status)
{
    switch(id)
    {
        case 0:
        {
            DDRB |= (1<<5);
            if (status)
                PORTB |= (1<<5);
            else
                PORTB &= ~(1<<5);
        }
        break;
        case 1:
        {
            DDRB |= (1<<6);
            if (status)
                PORTB |= (1<<6);
            else
                PORTB &= ~(1<<6);
        }
        break;
        case 2:
        {
            DDRA |= (1<<0);
            if (status)
                PORTA |= (1<<0);
            else
                PORTA &= ~(1<<0);
        }
        break;
        case 3:
        {
            DDRA |= (1<<1);
            if (status)
                PORTA |= (1<<1);
            else
                PORTA &= ~(1<<1);
        }
        break;
        case 4:
        {
            DDRA |= (1<<2);
            if (status)
                PORTA |= (1<<2);
            else
                PORTA &= ~(1<<2);
        }
        break;
        case 5:
        {
            DDRA |= (1<<3);
            if (status)
                PORTA |= (1<<3);
            else
                PORTA &= ~(1<<3);
        }
        break;
        case 6:
        {
            DDRA |= (1<<4);
            if (status)
                PORTA |= (1<<4);
            else
                PORTA &= ~(1<<4);
        }
        break;
        case 7:
        {
            DDRA |= (1<<5);
            if (status)
                PORTA |= (1<<5);
            else
                PORTA &= ~(1<<5);
        }
        break;
        case 8:
        {
            DDRA |= (1<<6);
            if (status)
                PORTA |= (1<<6);
            else
                PORTA &= ~(1<<6);
        }
        break;
        case 9:
        {
            DDRA |= (1<<7);
            if (status)
                PORTA |= (1<<7);
            else
                PORTA &= ~(1<<7);
        }
        break;
        case 10:
        {
            DDRC |= (1<<7);
            if (status)
                PORTC |= (1<<7);
            else
                PORTC &= ~(1<<7);
        }
        break;
        case 11:
        {
            DDRC |= (1<<6);
            if (status)
                PORTC |= (1<<6);
            else
                PORTC &= ~(1<<6);
        }
        break;
        case 12:
        {
            DDRC |= (1<<5);
            if (status)
                PORTC |= (1<<5);
            else
                PORTC &= ~(1<<5);
        }
        break;
        case 13:
        {
            DDRC |= (1<<4);
            if (status)
                PORTC |= (1<<4);
            else
                PORTC &= ~(1<<4);
        }
        break;
        case 14:
        {
            DDRC |= (1<<3);
            if (status)
                PORTC |= (1<<3);
            else
                PORTC &= ~(1<<3);
        }
        break;
        case 15:
        {
            DDRC |= (1<<2);
            if (status)
                PORTC |= (1<<2);
            else
                PORTC &= ~(1<<2);
        }
        break;
        case 16:
        {
            DDRC |= (1<<1);
            if (status)
                PORTC |= (1<<1);
            else
                PORTC &= ~(1<<1);
        }
        break;
        case 17:
        {
            DDRC |= (1<<0);
            if (status)
                PORTC |= (1<<0);
            else
                PORTC &= ~(1<<0);
        }
        break;
        case 18:
        {
            DDRD |= (1<<7);
            if (status)
                PORTD |= (1<<7);
            else
                PORTD &= ~(1<<7);
        }
        break;
        case 19:
        {
            DDRG |= (1<<2);
            if (status)
                PORTG |= (1<<2);
            else
                PORTG &= ~(1<<2);
        }
        break;
        case 20:
        {
            DDRG |= (1<<1);
            if (status)
                PORTG |= (1<<1);
            else
                PORTG &= ~(1<<1);
        }
        break;
        case 21:
        {
            DDRG |= (1<<0);
            if (status)
                PORTG |= (1<<0);
            else
                PORTG &= ~(1<<0);
        }
        break;
        case 22:
        {
            DDRL |= (1<<7);
            if (status)
                PORTL |= (1<<7);
            else
                PORTL &= ~(1<<7);
        }
        break;
        case 23:
        {
            DDRL |= (1<<6);
            if (status)
                PORTL |= (1<<6);
            else
                PORTL &= ~(1<<6);
        }
        break;
        case 24:
        {
            DDRL |= (1<<2);
            if (status)
                PORTL |= (1<<2);
            else
                PORTL &= ~(1<<2);
        }
        break;
        case 25:
        {
            DDRL |= (1<<1);
            if (status)
                PORTL |= (1<<1);
            else
                PORTL &= ~(1<<1);
        }
        break;
        case 26:
        {
            DDRL |= (1<<0);
            if (status)
                PORTL |= (1<<0);
            else
                PORTL &= ~(1<<0);
        }
        break;
        case 27:
        {
            DDRB |= (1<<0);
            if (status)
                PORTB |= (1<<0);
            else
                PORTB &= ~(1<<0);
        }
        break;
        case 28:
        {
            DDRE |= (1<<6);
            if (status)
                PORTE |= (1<<6);
            else
                PORTE &= ~(1<<6);
        }
        break;
        case 29:
        {
            DDRE |= (1<<7);
            if (status)
                PORTE |= (1<<7);
            else
                PORTE &= ~(1<<7);
        }
        break;
        case 30:
        {
            DDRJ |= (1<<2);
            if (status)
                PORTJ |= (1<<2);
            else
                PORTJ &= ~(1<<2);
        }
        break;
        case 31:
        {
            DDRJ |= (1<<3);
            if (status)
                PORTJ |= (1<<3);
            else
                PORTJ &= ~(1<<3);
        }
        break;
        case 32:
        {
            DDRJ |= (1<<4);
            if (status)
                PORTJ |= (1<<4);
            else
                PORTJ &= ~(1<<4);
        }
        break;
        case 33:
        {
            DDRJ |= (1<<5);
            if (status)
                PORTJ |= (1<<5);
            else
                PORTJ &= ~(1<<5);
        }
        break;
        case 34:
        {
            DDRD |= (1<<4);
            if (status)
                PORTD |= (1<<4);
            else
                PORTD &= ~(1<<4);
        }
        break;
        case 35:
        {
            #if !defined (SERIAL_HW_3_M)
            DDRJ |= (1<<1);
            if (status)
                PORTJ |= (1<<1);
            else
                PORTJ &= ~(1<<1);
            #endif
        }
        break;
        case 36:
        {
            #if !defined (SERIAL_HW_3_M)
            DDRJ |= (1<<0);
            if (status)
                PORTJ |= (1<<0);
            else
                PORTJ &= ~(1<<0);
            #endif
        }
        break;
        case 37:
        {
            #if !defined (SERIAL_HW_2_M)
            DDRH |= (1<<1);
            if (status)
                PORTH |= (1<<1);
            else
                PORTH &= ~(1<<1);
            #endif
        }
        break;
        case 38:
        {
            #if !defined (SERIAL_HW_2_M)
            DDRH |= (1<<0);
            if (status)
                PORTH |= (1<<0);
            else
                PORTH &= ~(1<<0);
            #endif
        }
        break;
        case 39:
        {
            #if !defined (SERIAL_HW_1_M)
            DDRD |= (1<<3);
            if (status)
                PORTD |= (1<<3);
            else
                PORTD &= ~(1<<3);
            #endif
        }
        break;
        case 40:
        {
            #if !defined (SERIAL_HW_1_M)
            DDRD |= (1<<2);
            if (status)
                PORTD |= (1<<2);
            else
                PORTD &= ~(1<<2);
            #endif
        }
        break;
        case 41:
        {
            #if !defined (I2C_M)
            DDRD |= (1<<1);
            if (status)
                PORTD |= (1<<1);
            else
                PORTD &= ~(1<<1);
            #endif
        }
        break;
        case 42:
        {
            #if !defined (I2C_M)
            DDRD |= (1<<0);
            if (status)
                PORTD |= (1<<0);
            else
                PORTD &= ~(1<<0);
            #endif
        }
        break;
        case 43:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<3);
            if (status)
                PORTB |= (1<<3);
            else
                PORTB &= ~(1<<3);
            #endif
        }
        break;
        case 44:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<2);
            if (status)
                PORTB |= (1<<2);
            else
                PORTB &= ~(1<<2);
            #endif
        }
        break;
        case 45:
        {
            #if !defined (SPI_M)
            DDRB |= (1<<1);
            if (status)
                PORTB |= (1<<1);
            else
                PORTB &= ~(1<<1);
            #endif
        }
        break;
        case 46:
        {
            DDRH |= (1<<6);
            if (status)
                PORTH |= (1<<6);
            else
                PORTH &= ~(1<<6);
        }
        break;
        case 47:
        {
            DDRB |= (1<<4);
            if (status)
                PORTB |= (1<<4);
            else
                PORTB &= ~(1<<4);
        }
        break;
        case 100:
        {
            DDRF |= (1<<0);
            if (status)
                PORTF |= (1<<0);
            else
                PORTF &= ~(1<<0);
        }
        break;
        case 101:
        {
            DDRF |= (1<<1);
            if (status)
                PORTF |= (1<<1);
            else
                PORTF &= ~(1<<1);
        }
        break;
        case 102:
        {
            DDRF |= (1<<2);
            if (status)
                PORTF |= (1<<2);
            else
                PORTF &= ~(1<<2);
        }
        break;
        case 103:
        {
            DDRF |= (1<<3);
            if (status)
                PORTF |= (1<<3);
            else
                PORTF &= ~(1<<3);
        }
        break;
        case 104:
        {
            DDRF |= (1<<4);
            if (status)
                PORTF |= (1<<4);
            else
                PORTF &= ~(1<<4);
        }
        break;
        case 105:
        {
            DDRF |= (1<<5);
            if (status)
                PORTF |= (1<<5);
            else
                PORTF &= ~(1<<5);
        }
        break;
        case 106:
        {
            DDRF |= (1<<6);
            if (status)
                PORTF |= (1<<6);
            else
                PORTF &= ~(1<<6);
        }
        break;
        case 107:
        {
            DDRF |= (1<<7);
            if (status)
                PORTF |= (1<<7);
            else
                PORTF &= ~(1<<7);
        }
        break;
        case 108:
        {
            DDRK |= (1<<0);
            if (status)
                PORTK |= (1<<0);
            else
                PORTK &= ~(1<<0);
        }
        break;
        case 109:
        {
            DDRK |= (1<<1);
            if (status)
                PORTK |= (1<<1);
            else
                PORTK &= ~(1<<1);
        }
        break;
        case 110:
        {
            DDRK |= (1<<2);
            if (status)
                PORTK |= (1<<2);
            else
                PORTK &= ~(1<<2);
        }
        break;
        case 111:
        {
            DDRK |= (1<<3);
            if (status)
                PORTK |= (1<<3);
            else
                PORTK &= ~(1<<3);
        }
        break;
        case 112:
        {
            DDRK |= (1<<4);
            if (status)
                PORTK |= (1<<4);
            else
                PORTK &= ~(1<<4);
        }
        break;
        case 113:
        {
            DDRK |= (1<<5);
            if (status)
                PORTK |= (1<<5);
            else
                PORTK &= ~(1<<5);
        }
        break;
        case 114:
        {
            DDRK |= (1<<6);
            if (status)
                PORTK |= (1<<6);
            else
                PORTK &= ~(1<<6);
        }
        break;
        case 115:
        {
            DDRK |= (1<<7);
            if (status)
                PORTK |= (1<<7);
            else
                PORTK &= ~(1<<7);
        }
        break;
        case 200:
        {
            #if !defined (PWM_M)
            DDRE |= (1<<4);
            if (status)
                PORTE |= (1<<4);
            else
                PORTE &= ~(1<<4);
            #endif
        }
        break;
        case 201:
        {
            #if !defined (PWM_M)
            DDRE |= (1<<5);
            if (status)
                PORTE |= (1<<5);
            else
                PORTE &= ~(1<<5);
            #endif
        }
        break;
        case 202:
        {
            #if !defined (PWM_M)
            DDRG |= (1<<5);
            if (status)
                PORTG |= (1<<5);
            else
                PORTG &= ~(1<<5);
            #endif
        }
        break;
        case 203:
        {
            #if !defined (PWM_M)
            DDRE |= (1<<3);
            if (status)
                PORTE |= (1<<3);
            else
                PORTE &= ~(1<<3);
            #endif
        }
        break;
        case 204:
        {
            #if !defined (PWM_M)
            DDRH |= (1<<3);
            if (status)
                PORTH |= (1<<3);
            else
                PORTH &= ~(1<<3);
            #endif
        }
        break;
        case 205:
        {
            #if !defined (PWM_M)
            DDRH |= (1<<4);
            if (status)
                PORTH |= (1<<4);
            else
                PORTH &= ~(1<<4);
            #endif
        }
        break;
        case 206:
        {
            #if !defined (PWM_M)
            DDRH |= (1<<5);
            if (status)
                PORTH |= (1<<5);
            else
                PORTH &= ~(1<<5);
            #endif
        }
        break;
        case 207:
        {
            #if !defined (PWM_M)
            DDRB |= (1<<7);
            if (status)
                PORTB |= (1<<7);
            else
                PORTB &= ~(1<<7);
            #endif
        }
        break;
        case 208:
        {
            #if !defined (PWM_M)
            DDRL |= (1<<3);
            if (status)
                PORTL |= (1<<3);
            else
                PORTL &= ~(1<<3);
            #endif
        }
        break;
        case 209:
        {
            #if !defined (PWM_M)
            DDRL |= (1<<4);
            if (status)
                PORTL |= (1<<4);
            else
                PORTL &= ~(1<<4);
            #endif
        }
        break;
        case 210:
        {
            #if !defined (PWM_M)
            DDRL |= (1<<5);
            if (status)
                PORTL |= (1<<5);
            else
                PORTL &= ~(1<<5);
            #endif
        }
        break;
    }
}

#endif

#endif
