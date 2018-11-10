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

struct pinInfo
{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
    uint16_t mask;
};

class DigitalPin
{
    public:
        DigitalPin();

        bool read(uint8_t id, bool pullup=false);
        void write(uint8_t id, bool status);

    private:
        pinInfo *digList;
        pinInfo *anaList;
        pinInfo *pwmList;
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

    DigitalPin::DigitalPin()
    {
        digList = (pinInfo *)malloc(sizeof(pinInfo)*10);
        memset(digList, 0, sizeof(digList));
        digList[0].ddr = &DDRD;  digList[0].port = &PORTD;  digList[0].pin = &PIND;  digList[0].mask = 2;
        digList[1].ddr = &DDRD;  digList[1].port = &PORTD;  digList[1].pin = &PIND;  digList[1].mask = 3;
        digList[2].ddr = &DDRD;  digList[2].port = &PORTD;  digList[2].pin = &PIND;  digList[2].mask = 4;
        digList[3].ddr = &DDRD;  digList[3].port = &PORTD;  digList[3].pin = &PIND;  digList[3].mask = 7;
        digList[4].ddr = &DDRB;  digList[4].port = &PORTB;  digList[4].pin = &PINB;  digList[4].mask = 0;
        digList[5].ddr = &DDRB;  digList[5].port = &PORTB;  digList[5].pin = &PINB;  digList[5].mask = 1;
        digList[6].ddr = &DDRB;  digList[6].port = &PORTB;  digList[6].pin = &PINB;  digList[6].mask = 2;
        #if !defined (SPI_M)
        digList[7].ddr = &DDRB;  digList[7].port = &PORTB;  digList[7].pin = &PINB;  digList[7].mask = 3;
        digList[8].ddr = &DDRB;  digList[8].port = &PORTB;  digList[8].pin = &PINB;  digList[8].mask = 4;
        digList[9].ddr = &DDRB;  digList[9].port = &PORTB;  digList[9].pin = &PINB;  digList[9].mask = 5;
        #endif

        anaList = (pinInfo *)malloc(sizeof(pinInfo)*6);
        memset(anaList, 0, sizeof(anaList));
        anaList[0].ddr = &DDRC;  anaList[0].port = &PORTC;  anaList[0].pin = &PINC;  anaList[0].mask = 0;
        anaList[1].ddr = &DDRC;  anaList[1].port = &PORTC;  anaList[1].pin = &PINC;  anaList[1].mask = 1;
        anaList[2].ddr = &DDRC;  anaList[2].port = &PORTC;  anaList[2].pin = &PINC;  anaList[2].mask = 2;
        anaList[3].ddr = &DDRC;  anaList[3].port = &PORTC;  anaList[3].pin = &PINC;  anaList[3].mask = 3;
        #if !defined (I2C_M)
        anaList[4].ddr = &DDRC;  anaList[4].port = &PORTC;  anaList[4].pin = &PINC;  anaList[4].mask = 4;
        anaList[5].ddr = &DDRC;  anaList[5].port = &PORTC;  anaList[5].pin = &PINC;  anaList[5].mask = 5;
        #endif

        pwmList = (pinInfo *)malloc(sizeof(pinInfo)*2);
        memset(pwmList, 0, sizeof(pwmList));
        #if !defined (PWM_M)
        pwmList[0].ddr = &DDRD;  pwmList[0].port = &PORTD;  pwmList[0].pin = &PIND;  pwmList[0].mask = 5;
        pwmList[1].ddr = &DDRD;  pwmList[1].port = &PORTD;  pwmList[1].pin = &PIND;  pwmList[1].mask = 6;
        #endif
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
    #define EASYHOME_LED_CPU 202
    #define EASYHOME_010V_1 209
    #define EASYHOME_010V_2 208
    #define EASYHOME_010V_3 207
    #define EASYHOME_010V_4 210
    #define EASYHOME_RS232_RX 38
    #define EASYHOME_RS232_TX 37
    #define EASYHOME_RS485_RX 36
    #define EASYHOME_RS485_TX 35
    #define EASYHOME_RS485_WE 30
    #define EASYHOME_EX2 39
    #define EASYHOME_EX1 40
    #define EASYHOME_SDA 41
    #define EASYHOME_SCL 42
    #define EASYHOME_IN_1 115
    #define EASYHOME_IN_2 114
    #define EASYHOME_IN_3 113
    #define EASYHOME_IN_4 112
    #define EASYHOME_IN_5 111
    #define EASYHOME_IN_6 110
    #define EASYHOME_IN_7 109
    #define EASYHOME_IN_5V 107
    #define EASYHOME_IN_12V 100
    #define EASYHOME_OUT_5V 4
    #define EASYHOME_OUT_12V 21
    #define EASYHOME_OUT_1 48
    #define EASYHOME_OUT_2 19
    #define EASYHOME_OUT_3 9
    #define EASYHOME_OUT_4 8
    #define EASYHOME_OUT_5 7
    #define EASYHOME_OUT_6 6
    #define EASYHOME_OUT_7 5
    #define EASYHOME_CAN_RESET 206
    #define EASYHOME_ETH_RESET 46
    #define EASYHOME_CAN_CS 47
    #define EASYHOME_ETH_CS 27
    #define EASYHOME_SD_CS 0
    #define EASYHOME_SPI_SCK 45
    #define EASYHOME_SPI_MISO 43
    #define EASYHOME_SPI_MOSI 44
#endif

DigitalPin::DigitalPin()
    {
        digList = (pinInfo *)malloc(sizeof(pinInfo)*10);
        memset(digList, 0, sizeof(digList));
        digList[0].ddr  = &DDRB;  digList[0].port  = &PORTB;  digList[0].pin  = &PINB;  digList[0].mask  = 5;
        digList[1].ddr  = &DDRB;  digList[1].port  = &PORTB;  digList[1].pin  = &PINB;  digList[1].mask  = 6;
        digList[2].ddr  = &DDRA;  digList[2].port  = &PORTA;  digList[2].pin  = &PINA;  digList[2].mask  = 0;
        digList[3].ddr  = &DDRA;  digList[3].port  = &PORTA;  digList[3].pin  = &PINA;  digList[3].mask  = 1;
        digList[4].ddr  = &DDRA;  digList[4].port  = &PORTA;  digList[4].pin  = &PINA;  digList[4].mask  = 2;
        digList[5].ddr  = &DDRA;  digList[5].port  = &PORTA;  digList[5].pin  = &PINA;  digList[5].mask  = 3;
        digList[6].ddr  = &DDRA;  digList[6].port  = &PORTA;  digList[6].pin  = &PINA;  digList[6].mask  = 4;
        digList[7].ddr  = &DDRA;  digList[7].port  = &PORTA;  digList[7].pin  = &PINA;  digList[7].mask  = 5;
        digList[8].ddr  = &DDRA;  digList[8].port  = &PORTA;  digList[8].pin  = &PINA;  digList[8].mask  = 6;
        digList[9].ddr  = &DDRA;  digList[9].port  = &PORTA;  digList[9].pin  = &PINA;  digList[9].mask = 7;
        digList[10].ddr = &DDRC;  digList[10].port = &PORTC;  digList[10].pin = &PINC;  digList[10].mask = 7;
        digList[11].ddr = &DDRC;  digList[11].port = &PORTC;  digList[11].pin = &PINC;  digList[11].mask = 6;
        digList[12].ddr = &DDRC;  digList[12].port = &PORTC;  digList[12].pin = &PINC;  digList[12].mask = 5;
        digList[13].ddr = &DDRC;  digList[13].port = &PORTC;  digList[13].pin = &PINC;  digList[13].mask = 4;
        digList[14].ddr = &DDRC;  digList[14].port = &PORTC;  digList[14].pin = &PINC;  digList[14].mask = 3;
        digList[15].ddr = &DDRC;  digList[15].port = &PORTC;  digList[15].pin = &PINC;  digList[15].mask = 2;
        digList[16].ddr = &DDRC;  digList[16].port = &PORTC;  digList[16].pin = &PINC;  digList[16].mask = 1;
        digList[17].ddr = &DDRC;  digList[17].port = &PORTC;  digList[17].pin = &PINC;  digList[17].mask = 0;
        digList[18].ddr = &DDRD;  digList[18].port = &PORTD;  digList[18].pin = &PIND;  digList[18].mask = 7;
        digList[19].ddr = &DDRG;  digList[19].port = &PORTG;  digList[19].pin = &PING;  digList[19].mask = 2;
        digList[20].ddr = &DDRG;  digList[20].port = &PORTG;  digList[20].pin = &PING;  digList[20].mask = 1;
        digList[21].ddr = &DDRG;  digList[21].port = &PORTG;  digList[21].pin = &PING;  digList[21].mask = 0;
        digList[22].ddr = &DDRL;  digList[22].port = &PORTL;  digList[22].pin = &PINL;  digList[22].mask = 7;
        digList[23].ddr = &DDRL;  digList[23].port = &PORTL;  digList[23].pin = &PINL;  digList[23].mask = 6;
        digList[24].ddr = &DDRL;  digList[24].port = &PORTL;  digList[24].pin = &PINL;  digList[24].mask = 2;
        digList[25].ddr = &DDRL;  digList[25].port = &PORTL;  digList[25].pin = &PINL;  digList[25].mask = 1;
        digList[26].ddr = &DDRL;  digList[26].port = &PORTL;  digList[26].pin = &PINL;  digList[26].mask = 0;
        digList[27].ddr = &DDRB;  digList[27].port = &PORTB;  digList[27].pin = &PINB;  digList[27].mask = 0;
        digList[28].ddr = &DDRE;  digList[28].port = &PORTE;  digList[28].pin = &PINE;  digList[28].mask = 6;
        digList[29].ddr = &DDRE;  digList[29].port = &PORTE;  digList[29].pin = &PINE;  digList[29].mask = 7;
        digList[30].ddr = &DDRJ;  digList[30].port = &PORTJ;  digList[30].pin = &PINJ;  digList[30].mask = 2;
        digList[31].ddr = &DDRJ;  digList[31].port = &PORTJ;  digList[31].pin = &PINJ;  digList[31].mask = 3;
        digList[32].ddr = &DDRJ;  digList[32].port = &PORTJ;  digList[32].pin = &PINJ;  digList[32].mask = 4;
        digList[33].ddr = &DDRJ;  digList[33].port = &PORTJ;  digList[33].pin = &PINJ;  digList[33].mask = 5;
        digList[34].ddr = &DDRD;  digList[34].port = &PORTD;  digList[34].pin = &PIND;  digList[34].mask = 4;
        #if !defined (SERIAL_HW_3_M)
        digList[35].ddr = &DDRJ;  digList[35].port = &PORTJ;  digList[35].pin = &PINJ;  digList[35].mask = 1;
        digList[36].ddr = &DDRJ;  digList[36].port = &PORTJ;  digList[36].pin = &PINJ;  digList[36].mask = 0;
        #endif
        #if !defined (SERIAL_HW_2_M)
        digList[37].ddr = &DDRH;  digList[37].port = &PORTH;  digList[37].pin = &PINH;  digList[37].mask = 1;
        digList[38].ddr = &DDRH;  digList[38].port = &PORTH;  digList[38].pin = &PINH;  digList[38].mask = 0;
        #endif
        #if !defined (SERIAL_HW_1_M)
        digList[39].ddr = &DDRD;  digList[39].port = &PORTD;  digList[39].pin = &PIND;  digList[39].mask = 3;
        digList[40].ddr = &DDRD;  digList[40].port = &PORTD;  digList[40].pin = &PIND;  digList[40].mask = 2;
        #endif
        #if !defined (I2C_M)
        digList[41].ddr = &DDRD;  digList[41].port = &PORTD;  digList[41].pin = &PIND;  digList[41].mask = 1;
        digList[42].ddr = &DDRD;  digList[42].port = &PORTD;  digList[42].pin = &PIND;  digList[42].mask = 0;
        #endif
        #if !defined (SPI_M)
        digList[43].ddr = &DDRB;  digList[43].port = &PORTB;  digList[43].pin = &PINB;  digList[43].mask = 3;
        digList[44].ddr = &DDRB;  digList[44].port = &PORTB;  digList[44].pin = &PINB;  digList[44].mask = 2;
        digList[45].ddr = &DDRB;  digList[45].port = &PORTB;  digList[45].pin = &PINB;  digList[45].mask = 1;
        #endif
        digList[46].ddr = &DDRH;  digList[46].port = &PORTH;  digList[46].pin = &PINH;  digList[46].mask = 6;
        digList[47].ddr = &DDRB;  digList[47].port = &PORTB;  digList[47].pin = &PINB;  digList[47].mask = 4;
        digList[48].ddr = &DDRJ;  digList[48].port = &PORTJ;  digList[48].pin = &PINJ;  digList[48].mask = 6;

        anaList = (pinInfo *)malloc(sizeof(pinInfo)*16);
        memset(anaList, 0, sizeof(anaList));
        anaList[0].ddr  = &DDRF;  anaList[0].port  = &PORTF;  anaList[0].pin  = &PINF;  anaList[0].mask  = 0;
        anaList[1].ddr  = &DDRF;  anaList[1].port  = &PORTF;  anaList[1].pin  = &PINF;  anaList[1].mask  = 1;
        anaList[2].ddr  = &DDRF;  anaList[2].port  = &PORTF;  anaList[2].pin  = &PINF;  anaList[2].mask  = 2;
        anaList[3].ddr  = &DDRF;  anaList[3].port  = &PORTF;  anaList[3].pin  = &PINF;  anaList[3].mask  = 3;
        anaList[4].ddr  = &DDRF;  anaList[4].port  = &PORTF;  anaList[4].pin  = &PINF;  anaList[4].mask  = 4;
        anaList[5].ddr  = &DDRF;  anaList[5].port  = &PORTF;  anaList[5].pin  = &PINF;  anaList[5].mask  = 5;
        anaList[6].ddr  = &DDRF;  anaList[6].port  = &PORTF;  anaList[6].pin  = &PINF;  anaList[6].mask  = 6;
        anaList[7].ddr  = &DDRF;  anaList[7].port  = &PORTF;  anaList[7].pin  = &PINF;  anaList[7].mask  = 7;
        anaList[8].ddr  = &DDRK;  anaList[8].port  = &PORTK;  anaList[8].pin  = &PINK;  anaList[8].mask  = 0;
        anaList[9].ddr  = &DDRK;  anaList[9].port  = &PORTK;  anaList[9].pin  = &PINK;  anaList[9].mask  = 1;
        anaList[10].ddr = &DDRK;  anaList[10].port = &PORTK;  anaList[10].pin = &PINK;  anaList[10].mask = 2;
        anaList[11].ddr = &DDRK;  anaList[11].port = &PORTK;  anaList[11].pin = &PINK;  anaList[11].mask = 3;
        anaList[12].ddr = &DDRK;  anaList[12].port = &PORTK;  anaList[12].pin = &PINK;  anaList[12].mask = 4;
        anaList[13].ddr = &DDRK;  anaList[13].port = &PORTK;  anaList[13].pin = &PINK;  anaList[13].mask = 5;
        anaList[14].ddr = &DDRK;  anaList[14].port = &PORTK;  anaList[14].pin = &PINK;  anaList[14].mask = 6;
        anaList[15].ddr = &DDRK;  anaList[15].port = &PORTK;  anaList[15].pin = &PINK;  anaList[15].mask = 7;

        pwmList = (pinInfo *)malloc(sizeof(pinInfo)*11);
        memset(pwmList, 0, sizeof(pwmList));
        pwmList[0].ddr  = &DDRE;  pwmList[0].port  = &PORTE;  pwmList[0].pin  = &PINE;  pwmList[0].mask  = 4;
        pwmList[1].ddr  = &DDRE;  pwmList[1].port  = &PORTE;  pwmList[1].pin  = &PINE;  pwmList[1].mask  = 5;
        pwmList[2].ddr  = &DDRG;  pwmList[2].port  = &PORTG;  pwmList[2].pin  = &PING;  pwmList[2].mask  = 5;
        pwmList[3].ddr  = &DDRE;  pwmList[3].port  = &PORTE;  pwmList[3].pin  = &PINE;  pwmList[3].mask  = 3;
        pwmList[4].ddr  = &DDRH;  pwmList[4].port  = &PORTH;  pwmList[4].pin  = &PINH;  pwmList[4].mask  = 3;
        pwmList[5].ddr  = &DDRH;  pwmList[5].port  = &PORTH;  pwmList[5].pin  = &PINH;  pwmList[5].mask  = 4;
        pwmList[6].ddr  = &DDRH;  pwmList[6].port  = &PORTH;  pwmList[6].pin  = &PINH;  pwmList[6].mask  = 5;
        pwmList[7].ddr  = &DDRB;  pwmList[7].port  = &PORTB;  pwmList[7].pin  = &PINB;  pwmList[7].mask  = 7;
        pwmList[8].ddr  = &DDRL;  pwmList[8].port  = &PORTL;  pwmList[8].pin  = &PINL;  pwmList[8].mask  = 3;
        pwmList[9].ddr  = &DDRL;  pwmList[9].port  = &PORTL;  pwmList[9].pin  = &PINL;  pwmList[9].mask  = 4;
        pwmList[10].ddr = &DDRL;   pwmList[10].port = &PORTL;  pwmList[10].pin = &PINL;  pwmList[10].mask = 5;
    }

#endif

bool DigitalPin::read(uint8_t id, bool pullup)
{
    if (id >= 0 && id <=99)
    {
        if (digList[id].ddr == 0)
            return false;
        id = id - 0;
        *digList[id].ddr &= ~(1<<digList[id].mask);
        if (pullup)
            *digList[id].port |= (1<<digList[id].mask);
        else
            *digList[id].port &= ~(1<<digList[id].mask);
        asm("nop");
        if(*digList[id].pin & (1<<digList[id].mask))
            return true;
    }
    else if (id >= 100 && id <=199)
    {
        if (anaList[id].ddr == 0)
            return false;
        id = id - 100;
        *anaList[id].ddr |= (1<<anaList[id].mask);
        if (pullup)
            *anaList[id].port |= (1<<anaList[id].mask);
        else
            *anaList[id].port &= ~(1<<anaList[id].mask);
        asm("nop");
        if(*anaList[id].pin & (1<<anaList[id].mask))
            return true;
    }
    else if (id >= 200 && id <=299)
    {
        if (pwmList[id].ddr == 0)
            return false;
        id = id - 200;
        *pwmList[id].ddr |= (1<<pwmList[id].mask);
        if (pullup)
            *pwmList[id].port |= (1<<pwmList[id].mask);
        else
            *pwmList[id].port &= ~(1<<pwmList[id].mask);
        asm("nop");
        if(*pwmList[id].pin & (1<<pwmList[id].mask))
            return true;
    }
    return false;
}

void DigitalPin::write(uint8_t id, bool status)
{
    if (id >= 0 && id <=99)
    {
        if (digList[id].ddr == 0)
            return;
        id = id - 0;
        *digList[id].ddr |= (1<<digList[id].mask);
        if (status)
            *digList[id].port |= (1<<digList[id].mask);
        else
            *digList[id].port &= ~(1<<digList[id].mask);
    }
    else if (id >= 100 && id <=199)
    {
        if (anaList[id].ddr == 0)
            return;
        id = id - 100;
        *anaList[id].ddr |= (1<<anaList[id].mask);
        if (status)
            *anaList[id].port |= (1<<anaList[id].mask);
        else
            *anaList[id].port &= ~(1<<anaList[id].mask);
    }
    else if (id >= 200 && id <=299)
    {
        if (pwmList[id].ddr == 0)
            return;
        id = id - 200;
        *pwmList[id].ddr |= (1<<pwmList[id].mask);
        if (status)
            *pwmList[id].port |= (1<<pwmList[id].mask);
        else
            *pwmList[id].port &= ~(1<<pwmList[id].mask);
    }
}

#endif
