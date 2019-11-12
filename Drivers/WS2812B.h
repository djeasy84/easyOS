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

#ifndef WS2812B_H
#define WS2812B_H

/****************************************************************************************/
/****************************************************************************************/

void updateLED(uint8_t *data, uint16_t len, uint8_t pin)
{
  uint8_t oldSREG = SREG;
  cli();

  #if defined (__BOARD_arduinoUNO__) || defined (__BOARD_arduinoNANO__) || defined (__BOARD_arduinoMEGA__)
  #if defined (__BOARD_arduinoUNO__) || defined (__BOARD_arduinoNANO__)
  volatile uint8_t *portADDR = &PORTD;
  #endif
  #if defined (__BOARD_arduinoMEGA__)
  volatile uint8_t *portADDR = &PORTC;
  #endif
  uint8_t pinHI = *portADDR |  (1<<pin);
  uint8_t pinLO = *portADDR & ~(1<<pin);
  uint8_t lenHI = (len >> 8);
  uint8_t lenLO = (len >> 0);

  asm ("mov  r22, %[pinHI]      ;\n" :: [pinHI] "r" (pinHI) : "r22");
  asm ("mov  r23, %[pinLO]      ;\n" :: [pinLO] "r" (pinLO) : "r23");
  asm ("movw r30, %[data]       ;\n" :: [data]  "e" (data)  : "r30", "r31");
  asm ("ld   r18, Z             ;\n");
  asm ("mov  r25, %[lenHI]      ;\n" :: [lenHI] "r" (lenHI) : "r25");
  asm ("mov  r24, %[lenLO]      ;\n" :: [lenLO] "r" (lenLO) : "r24");
  asm ("ldi  r20, 0b10000000    ;\n");

  asm ("startEXE:               ;\n");
  asm ("  mov  r21, r18         ;\n");
  asm ("  and  r21, r20         ;\n");
  asm ("  tst  r21              ;\n");
  asm ("  breq bitZERO          ;\n");
  asm ("  jmp  bitONE           ;\n");

  asm ("bitNEXT:                ;\n");
  asm ("  lsr  r20              ;\n");
  asm ("  tst  r20              ;\n");
  asm ("  breq byteNEXT         ;\n");
  asm ("  jmp  startEXE         ;\n");

  asm ("byteNEXT:               ;\n");
  asm ("  ldi  r20, 0b10000000  ;\n");
  asm ("  ld   r18, Z+          ;\n");
  asm ("  ld   r18, Z           ;\n");
  asm ("  sbiw r24, 1           ;\n");
  asm ("  breq stopEXE          ;\n");
  asm ("  jmp  startEXE         ;\n");

  asm ("bitZERO:                ;\n"); // H: 374.5 ns - L: 1501.0 ns = 1875.5
  asm ("  out %[port], r22      ;\n" :: [port] "M" (_SFR_IO_ADDR(*portADDR)));
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  out %[port], r23      ;\n" :: [port] "M" (_SFR_IO_ADDR(*portADDR)));
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  jmp bitNEXT           ;\n");

  asm ("bitONE:                 ;\n"); // H: 812.5 ns + L: 1063.0 ns = 1875.5 ns
  asm ("  out %[port], r22      ;\n" :: [port] "M" (_SFR_IO_ADDR(*portADDR)));
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  nop                   ;\n");
  asm ("  out %[port], r23      ;\n" :: [port] "M" (_SFR_IO_ADDR(*portADDR)));
  asm ("  jmp bitNEXT           ;\n");

  asm ("stopEXE:                ;\n"); // L: 58.0 us
  asm ("  ldi r19, 0x3C         ;\n");
  asm ("  resetLOOP:            ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    nop                 ;\n");
  asm ("    dec  r19            ;\n");
  asm ("    brne resetLOOP      ;\n");
  #endif

  SREG = oldSREG;  //sei();
}

/****************************************************************************************/
/****************************************************************************************/

#include <stdlib.h>
#include <string.h>

class LightEmittingDiode
{
    public:
        bool setup(uint8_t pin, uint16_t len);

        bool write(uint16_t led, uint8_t r, uint8_t g, uint8_t b);

        bool update();

    private:
        uint8_t dataPIN;
        uint16_t dataLEN;
        uint8_t *dataLED;

        uint32_t lastUpdate;
};

LightEmittingDiode LED;

/****************************************************************************************/

bool LightEmittingDiode::setup(uint8_t pin, uint16_t len)
{
    lastUpdate = 0;
    *dataLED = dataLEN = dataPIN = 0;

    #if !defined (__BOARD_arduinoUNO__) && !defined (__BOARD_arduinoNANO__) && !defined (__BOARD_arduinoMEGA__)
    return false;
    #endif

    #if defined (__BOARD_arduinoUNO__) || defined (__BOARD_arduinoNANO__)
    if (pin != ARDUINO_PIN_2 && pin != ARDUINO_PIN_3 && pin != ARDUINO_PIN_4 && pin != ARDUINO_PIN_5 && pin != ARDUINO_PIN_6 && pin != ARDUINO_PIN_7)
        return false;
    #endif
    #if defined (__BOARD_arduinoMEGA__)
    if (pin != ARDUINO_PIN_30 && pin != ARDUINO_PIN_31 && pin != ARDUINO_PIN_32 && pin != ARDUINO_PIN_33 && pin != ARDUINO_PIN_34 && pin != ARDUINO_PIN_35)
        return false;
    #endif

    if (len > 384)  // ~1152 Byte
        return false;

    DP.write(pin, false);
    switch(pin)
    {
        #if defined (__BOARD_arduinoUNO__) || defined (__BOARD_arduinoNANO__)
        case ARDUINO_PIN_2:
        {
            dataPIN = 2;
        }
        break;
        case ARDUINO_PIN_3:
        {
            dataPIN = 3;
        }
        break;
        case ARDUINO_PIN_4:
        {
            dataPIN = 4;
        }
        break;
        case ARDUINO_PIN_5:
        {
            dataPIN = 5;
        }
        break;
        case ARDUINO_PIN_6:
        {
            dataPIN = 6;
        }
        break;
        case ARDUINO_PIN_7:
        {
            dataPIN = 7;
        }
        break;
        #endif
        #if defined (__BOARD_arduinoMEGA__)
        case ARDUINO_PIN_30:
        {
            dataPIN = 7;
        }
        break;
        case ARDUINO_PIN_31:
        {
            dataPIN = 6;
        }
        break;
        case ARDUINO_PIN_32:
        {
            dataPIN = 5;
        }
        break;
        case ARDUINO_PIN_33:
        {
            dataPIN = 4;
        }
        break;
        case ARDUINO_PIN_34:
        {
            dataPIN = 3;
        }
        break;
        case ARDUINO_PIN_35:
        {
            dataPIN = 2;
        }
        break;
        #endif
    }

    dataLEN = len*3;
    if (dataLED = (uint8_t *)malloc(dataLEN))
    {
        memset(dataLED, 0, dataLEN);
    }
    else
    {
        dataLEN = 0;
        return false;
    }

    return true;
}

bool LightEmittingDiode::write(uint16_t led, uint8_t r, uint8_t g, uint8_t b)
{
    if (led > (dataLEN/3)-1)
        return false;

    dataLED[led*3+0] = g;
    dataLED[led*3+1] = r;
    dataLED[led*3+2] = b;

    return true;
}

bool LightEmittingDiode::update()
{
    if (dataLED == 0 || dataLEN == 0 || dataPIN == 0)
        return false;

    if (ST.time_diff(ST.millisec(), lastUpdate) > 20)
    {
        lastUpdate = ST.millisec();

        updateLED(dataLED, dataLEN, dataPIN);
    }
    return true;
}

#endif
