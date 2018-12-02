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

#include <avr/pgmspace.h>
#include <avr/interrupt.h>

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
    #define DIGITAL_SIZE 10
    #define ANALOG_SIZE 6
    #define PWM_SIZE 2
        /*anaList[0].ddr = &DDRC;  anaList[0].port = &PORTC;  anaList[0].pin = &PINC;  anaList[0].mask = 0;
        anaList[1].ddr = &DDRC;  anaList[1].port = &PORTC;  anaList[1].pin = &PINC;  anaList[1].mask = 1;
        anaList[2].ddr = &DDRC;  anaList[2].port = &PORTC;  anaList[2].pin = &PINC;  anaList[2].mask = 2;
        anaList[3].ddr = &DDRC;  anaList[3].port = &PORTC;  anaList[3].pin = &PINC;  anaList[3].mask = 3;
        #if !defined (I2C_M)
        anaList[4].ddr = &DDRC;  anaList[4].port = &PORTC;  anaList[4].pin = &PINC;  anaList[4].mask = 4;
        anaList[5].ddr = &DDRC;  anaList[5].port = &PORTC;  anaList[5].pin = &PINC;  anaList[5].mask = 5;
        #endif*/
    const PROGMEM uint16_t digitalListDrr[DIGITAL_SIZE]  = { (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint16_t digitalListPort[DIGITAL_SIZE] = { (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint16_t digitalListPin[DIGITAL_SIZE]  = { (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint8_t digitalListMask[DIGITAL_SIZE]  = { 2,
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
    const PROGMEM uint16_t analogListDrr[ANALOG_SIZE]    = { (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint16_t analogListPort[ANALOG_SIZE]   = { (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint16_t analogListPin[ANALOG_SIZE]    = { (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint8_t analogListMask[ANALOG_SIZE]    = { 0,
                                                             1,
                                                             2,
                                                             3,
                                                             #if !defined (I2C_M)
                                                             4,
                                                             5,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                          };
    const PROGMEM uint16_t pwmListDrr[PWM_SIZE]          = { (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                          };
    const PROGMEM uint16_t pwmListPort[DIGITAL_SIZE]     = { (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                          };
    const PROGMEM uint16_t pwmListPin[PWM_SIZE]          = { (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                          };
    const PROGMEM uint8_t pwmListMask[PWM_SIZE]          = { 5,
                                                             6,
                                                          };
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
    #define DIGITAL_SIZE 49
    #define ANALOG_SIZE 16
    #define PWM_SIZE 11
    const PROGMEM uint16_t digitalListDrr[DIGITAL_SIZE]  = { (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRA,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRC,
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRG,
                                                             (uint16_t)&DDRG,
                                                             (uint16_t)&DDRG,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRE,
                                                             (uint16_t)&DDRE,
                                                             (uint16_t)&DDRJ,
                                                             (uint16_t)&DDRJ,
                                                             (uint16_t)&DDRJ,
                                                             (uint16_t)&DDRJ,
                                                             (uint16_t)&DDRD,
                                                             #if !defined (SERIAL_HW_3_M)
                                                             (uint16_t)&DDRJ,
                                                             (uint16_t)&DDRJ,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_2_M)
                                                             (uint16_t)&DDRH,
                                                             (uint16_t)&DDRH,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_1_M)
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&DDRD,
                                                             (uint16_t)&DDRD,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                             (uint16_t)&DDRH,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRJ,
                                                          };
    const PROGMEM uint16_t digitalListPort[DIGITAL_SIZE] = { (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTA,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTC,
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTG,
                                                             (uint16_t)&PORTG,
                                                             (uint16_t)&PORTG,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTE,
                                                             (uint16_t)&PORTE,
                                                             (uint16_t)&PORTJ,
                                                             (uint16_t)&PORTJ,
                                                             (uint16_t)&PORTJ,
                                                             (uint16_t)&PORTJ,
                                                             (uint16_t)&PORTD,
                                                             #if !defined (SERIAL_HW_3_M)
                                                             (uint16_t)&PORTJ,
                                                             (uint16_t)&PORTJ,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_2_M)
                                                             (uint16_t)&PORTH,
                                                             (uint16_t)&PORTH,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_1_M)
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&PORTD,
                                                             (uint16_t)&PORTD,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                             (uint16_t)&PORTH,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTJ,
                                                          };
    const PROGMEM uint16_t digitalListPin[DIGITAL_SIZE]  = { (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINA,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PINC,
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PING,
                                                             (uint16_t)&PING,
                                                             (uint16_t)&PING,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINE,
                                                             (uint16_t)&PINE,
                                                             (uint16_t)&PINJ,
                                                             (uint16_t)&PINJ,
                                                             (uint16_t)&PINJ,
                                                             (uint16_t)&PINJ,
                                                             (uint16_t)&PIND,
                                                             #if !defined (SERIAL_HW_3_M)
                                                             (uint16_t)&PINJ,
                                                             (uint16_t)&PINJ,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_2_M)
                                                             (uint16_t)&PINH,
                                                             (uint16_t)&PINH,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_1_M)
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (I2C_M)
                                                             (uint16_t)&PIND,
                                                             (uint16_t)&PIND,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SPI_M)
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINB,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                             (uint16_t)&PINH,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINJ,
                                                          };
    const PROGMEM uint8_t digitalListMask[DIGITAL_SIZE]  = { 5,
                                                             6,
                                                             0,
                                                             1,
                                                             2,
                                                             3,
                                                             4,
                                                             5,
                                                             6,
                                                             7,
                                                             7,
                                                             6,
                                                             5,
                                                             4,
                                                             3,
                                                             2,
                                                             1,
                                                             0,
                                                             7,
                                                             2,
                                                             1,
                                                             0,
                                                             7,
                                                             6,
                                                             2,
                                                             1,
                                                             0,
                                                             0,
                                                             6,
                                                             7,
                                                             2,
                                                             3,
                                                             4,
                                                             5,
                                                             4,
                                                             #if !defined (SERIAL_HW_3_M)
                                                             1,
                                                             0,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_2_M)
                                                             1,
                                                             0,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SERIAL_HW_1_M)
                                                             3,
                                                             2,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (I2C_M)
                                                             1,
                                                             0,
                                                             #else
                                                             0,
                                                             0,
                                                             #endif
                                                             #if !defined (SPI_M)
                                                             3,
                                                             2,
                                                             1,
                                                             #else
                                                             0,
                                                             0,
                                                             0,
                                                             #endif
                                                             6,
                                                             4,
                                                             6,
                                                          };
    const PROGMEM uint16_t analogListDrr[ANALOG_SIZE]    = { (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRF,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                             (uint16_t)&DDRK,
                                                          };
    const PROGMEM uint16_t analogListPort[ANALOG_SIZE]   = { (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTF,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                             (uint16_t)&PORTK,
                                                          };
    const PROGMEM uint16_t analogListPin[ANALOG_SIZE]    = { (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINF,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                             (uint16_t)&PINK,
                                                          };
    const PROGMEM uint8_t analogListMask[ANALOG_SIZE]    = { 0,
                                                             1,
                                                             2,
                                                             3,
                                                             4,
                                                             5,
                                                             6,
                                                             7,
                                                             0,
                                                             1,
                                                             2,
                                                             3,
                                                             4,
                                                             5,
                                                             6,
                                                             7,
                                                          };
    const PROGMEM uint16_t pwmListDrr[PWM_SIZE]          = { (uint16_t)&DDRE,
                                                             (uint16_t)&DDRE,
                                                             (uint16_t)&DDRG,
                                                             (uint16_t)&DDRE,
                                                             (uint16_t)&DDRH,
                                                             (uint16_t)&DDRH,
                                                             (uint16_t)&DDRH,
                                                             (uint16_t)&DDRB,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                             (uint16_t)&DDRL,
                                                          };
    const PROGMEM uint16_t pwmListPort[DIGITAL_SIZE]     = { (uint16_t)&PORTE,
                                                             (uint16_t)&PORTE,
                                                             (uint16_t)&PORTG,
                                                             (uint16_t)&PORTE,
                                                             (uint16_t)&PORTH,
                                                             (uint16_t)&PORTH,
                                                             (uint16_t)&PORTH,
                                                             (uint16_t)&PORTB,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                             (uint16_t)&PORTL,
                                                          };
    const PROGMEM uint16_t pwmListPin[PWM_SIZE]          = { (uint16_t)&PINE,
                                                             (uint16_t)&PINE,
                                                             (uint16_t)&PING,
                                                             (uint16_t)&PINE,
                                                             (uint16_t)&PINH,
                                                             (uint16_t)&PINH,
                                                             (uint16_t)&PINH,
                                                             (uint16_t)&PINB,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                             (uint16_t)&PINL,
                                                          };
    const PROGMEM uint8_t pwmListMask[PWM_SIZE]          = { 4,
                                                             5,
                                                             5,
                                                             3,
                                                             3,
                                                             4,
                                                             5,
                                                             7,
                                                             3,
                                                             4,
                                                             5,
                                                          };
#endif

class DigitalPin
{
    public:
        bool read(uint16_t id, bool pullup=false);
        void write(uint16_t id, bool status);
};

DigitalPin DP;

/****************************************************************************************/

bool DigitalPin::read(uint16_t id, bool pullup)
{
    if (id >= 0 && id <=99)
    {
        id = id - 0;
        if (id >= DIGITAL_SIZE)
            return false;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(digitalListDrr + (id)));
        if (ddr == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(digitalListMask + (id)));
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(digitalListPort + (id)));
        if (pullup)
            *port |= mask;
        else
            *port &= ~mask;
        *ddr &= ~mask;
        asm("nop");
        volatile uint8_t *pin = (volatile uint8_t *)(pgm_read_word(digitalListPin + (id)));
        if(*pin & mask)
            return true;
    }
    else if (id >= 100 && id <=199)
    {
        id = id - 100;
        if (id >= ANALOG_SIZE)
            return false;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(analogListDrr + (id)));
        if (ddr == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(analogListMask + (id)));
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(analogListPort + (id)));
        if (pullup)
            *port |= mask;
        else
            *port &= ~mask;
        *ddr &= ~mask;
        asm("nop");
        volatile uint8_t *pin = (volatile uint8_t *)(pgm_read_word(analogListPin + (id)));
        if(*pin & mask)
            return true;
    }
    else if (id >= 200 && id <=299)
    {
        id = id - 200;
        if (id >= PWM_SIZE)
            return false;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(pwmListDrr + (id)));
        if (ddr == 0)
            return false;
        uint8_t mask = (1<<pgm_read_byte(pwmListMask + (id)));
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(pwmListPort + (id)));
        if (pullup)
            *port |= mask;
        else
            *port &= ~mask;
        *ddr &= ~mask;
        asm("nop");
        volatile uint8_t *pin = (volatile uint8_t *)(pgm_read_word(pwmListPin + (id)));
        if(*pin & mask)
            return true;
    }
    return false;
}

void DigitalPin::write(uint16_t id, bool status)
{
    if (id >= 0 && id <=99)
    {
        id = id - 0;
        if (id >= DIGITAL_SIZE)
            return;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(digitalListDrr + (id)));
        if (ddr == 0)
            return;
        uint8_t mask = (1<<pgm_read_byte(digitalListMask + (id)));
        *ddr |= mask;
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(digitalListPort + (id)));
        if (status)
            *port |= mask;
        else
            *port &= ~mask;
    }
    else if (id >= 100 && id <=199)
    {
        id = id - 100;
        if (id >= ANALOG_SIZE)
            return;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(analogListDrr + (id)));
        if (ddr == 0)
            return;
        uint8_t mask = (1<<pgm_read_byte(analogListMask + (id)));
        *ddr |= mask;
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(analogListPort + (id)));
        if (status)
            *port |= mask;
        else
            *port &= ~mask;
    }
    else if (id >= 200 && id <=299)
    {
        id = id - 200;
        if (id >= PWM_SIZE)
            return;
        volatile uint8_t *ddr = (volatile uint8_t *)(pgm_read_word(pwmListDrr + (id)));
        if (ddr == 0)
            return;
        uint8_t mask = (1<<pgm_read_byte(pwmListMask + (id)));
        *ddr |= mask;
        volatile uint8_t *port = (volatile uint8_t *)(pgm_read_word(pwmListPort + (id)));
        if (status)
            *port |= mask;
        else
            *port &= ~mask;
    }
}

#endif
