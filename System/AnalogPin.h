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

#if defined (__AVR_ATmega328P__)
#define ANALOG_SIZE 8
#endif
#if defined (__AVR_ATmega2560__)
#define ANALOG_SIZE 16
#endif

struct adcInfo
{
    uint8_t admux_mask;
    uint8_t adcsrb_mask;
};

class AnalogPin
{
    public:
        AnalogPin();

        uint16_t read(uint16_t id);

    private:
        adcInfo anaList[ANALOG_SIZE];
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

    AnalogPin::AnalogPin()
    {
        anaList[0].admux_mask = 0;                                  anaList[0].adcsrb_mask = 0;
        anaList[1].admux_mask = (1<<MUX0);                          anaList[1].adcsrb_mask = 0;
        anaList[2].admux_mask = (1<<MUX1);                          anaList[2].adcsrb_mask = 0;
        anaList[3].admux_mask = (1<<MUX1) | (1<<MUX0);              anaList[3].adcsrb_mask = 0;
        anaList[4].admux_mask = (1<<MUX2);                          anaList[4].adcsrb_mask = 0;
        anaList[5].admux_mask = (1<<MUX2) | (1<<MUX0);              anaList[5].adcsrb_mask = 0;
        anaList[6].admux_mask = (1<<MUX2) | (1<<MUX1);              anaList[6].adcsrb_mask = 0;
        anaList[7].admux_mask = (1<<MUX2) | (1<<MUX1) | (1<<MUX0);  anaList[7].adcsrb_mask = 0;
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

    AnalogPin::AnalogPin()
    {
        anaList[0].admux_mask  = 0;                                  anaList[0].adcsrb_mask  = 0;
        anaList[1].admux_mask  = (1<<MUX0);                          anaList[1].adcsrb_mask  = 0;
        anaList[2].admux_mask  = (1<<MUX1);                          anaList[2].adcsrb_mask  = 0;
        anaList[3].admux_mask  = (1<<MUX1) | (1<<MUX0);              anaList[3].adcsrb_mask  = 0;
        anaList[4].admux_mask  = (1<<MUX2);                          anaList[4].adcsrb_mask  = 0;
        anaList[5].admux_mask  = (1<<MUX2) | (1<<MUX0);              anaList[5].adcsrb_mask  = 0;
        anaList[6].admux_mask  = (1<<MUX2) | (1<<MUX1);              anaList[6].adcsrb_mask  = 0;
        anaList[7].admux_mask  = (1<<MUX2) | (1<<MUX1) | (1<<MUX0);  anaList[7].adcsrb_mask  = 0;
        anaList[8].admux_mask  = 0;                                  anaList[8].adcsrb_mask  = (1<<MUX5);
        anaList[9].admux_mask  = (1<<MUX0);                          anaList[9].adcsrb_mask  = (1<<MUX5);
        anaList[10].admux_mask = (1<<MUX1);                          anaList[10].adcsrb_mask = (1<<MUX5);
        anaList[11].admux_mask = (1<<MUX1);                          anaList[11].adcsrb_mask = (1<<MUX5);
        anaList[12].admux_mask = (1<<MUX1);                          anaList[12].adcsrb_mask = (1<<MUX5);
        anaList[13].admux_mask = (1<<MUX1);                          anaList[13].adcsrb_mask = (1<<MUX5);
        anaList[14].admux_mask = (1<<MUX1);                          anaList[14].adcsrb_mask = (1<<MUX5);
        anaList[15].admux_mask = (1<<MUX1);                          anaList[15].adcsrb_mask = (1<<MUX5);
    }
#endif

uint16_t AnalogPin::read(uint16_t id)
{
    if (id >= 100 && id <=199)
    {
        id = id - 100;
        if (id >= ANALOG_SIZE)
            return 0;
        ADCSRA = ADCSRB = ADMUX = 0;
        ADMUX = (1<<REFS0) | anaList[id].admux_mask;
        ADCSRB |= anaList[id].adcsrb_mask;
        ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS0);
        while (ADCSRA & (1<<ADSC));
        ADCSRA = ADCSRB = ADMUX = 0;
        return ADC;
    }
    return 0;
}

#endif
