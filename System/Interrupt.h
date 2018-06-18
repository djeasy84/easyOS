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

#include "./Time.h"

#define CHANGE 1
#define FALLING 2
#define RISING 3

class Interrupt
{
    public:
        Interrupt();

        bool enableInterrupt(uint8_t ID, void (*Func)(void), uint8_t Mode, uint32_t Debounce = 25);
		bool disableInterrupt(uint8_t ID);
};

Interrupt IM;

/****************************************************************************************/

#if defined (__AVR_ATmega328P__)

#if defined (__BOARD_arduinoUNO__) or defined (__BOARD_arduinoNANO__)
#define I0 0  // ARDUINO_PIN_2 - PD2 - INT0_vect
#define I1 1  // ARDUINO_PIN_3 - PD3 - INT1_vect

#define MAX_INTERRUPT 2

uint32_t lastTime[MAX_INTERRUPT];
uint32_t debounceTime[MAX_INTERRUPT];
void (*intFunc[MAX_INTERRUPT])(void);

ISR(INT0_vect)
{
	if (intFunc[I0] == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime[I0]) > debounceTime[I0])
	{
		lastTime[I0] = ST.millisec();
		intFunc[I0]();
	}
}
ISR(INT1_vect)
{
	if (intFunc[I1] == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime[I1]) > debounceTime[I1])
	{
		lastTime[I1] = ST.millisec();
		intFunc[I1]();
	}
}
#endif

Interrupt::Interrupt()
{
	EICRA = EIMSK = 0;
	for (uint8_t i=0; i<MAX_INTERRUPT; i++)
	{
		lastTime[i] = 0;
		debounceTime[i] = 0;
        intFunc[i] = 0x0000;
	}
}

bool Interrupt::enableInterrupt(uint8_t ID, void (*Func)(void), uint8_t Mode, uint32_t Debounce)
{
	if ((Mode != CHANGE) && (Mode != RISING) && (Mode != FALLING))
		return false;
	switch(ID)
	{
		case I0:
		{
			EICRA |= (Mode<<0) & 0b00000011;
			EIMSK |= (1<<INT0);
			lastTime[I0] = 0;
			debounceTime[I0] = Debounce;
			intFunc[I0] = Func;
			return true;
		}
		break;
		case I1:
		{
			EICRA |= (Mode<<2) & 0b00001100;
			EIMSK |= (1<<INT1);
			lastTime[I1] = 0;
			debounceTime[I1] = Debounce;
			intFunc[I1] = Func;
			return true;
		}
		break;
	}
    return false;
}

bool Interrupt::disableInterrupt(uint8_t ID)
{
   	switch(ID)
	{
		case I0:
		{
			EICRA &= ~(0b00000011);
			EIMSK &= ~(1<<INT0);
			lastTime[I0] = 0;
			debounceTime[I0] = 0;
			intFunc[I0] = 0x0000;
			return true;
		}
		break;
		case I1:
		{
			EICRA &= ~(0b00001100);
			EIMSK &= ~(1<<INT1);
			lastTime[I1] = 0;
			debounceTime[I1] = 0;
			intFunc[I1] = 0x0000;
			return true;
		}
		break;
	}
    return false;
}

#endif

#if defined (__AVR_ATmega2560__)

#if defined (__BOARD_arduinoMEGA__)
#define I0 0  // ARDUINO_PIN_2  - PE4 - INT4_vect
#define I1 1  // ARDUINO_PIN_3  - PE5 - INT5_vect

#define MAX_INTERRUPT 2

uint32_t lastTime[MAX_INTERRUPT];
uint32_t debounceTime[MAX_INTERRUPT];
void (*intFunc[MAX_INTERRUPT])(void);

ISR(INT4_vect)
{
	if (intFunc[I0] == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime[I0]) > debounceTime[I0])
	{
		lastTime[I0] = ST.millisec();
		intFunc[I0]();
	}
}
ISR(INT5_vect)
{
	if (intFunc[I1] == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime[I1]) > debounceTime[I1])
	{
		lastTime[I1] = ST.millisec();
		intFunc[I1]();
	}
}
#endif

Interrupt::Interrupt()
{
	EICRA = EIMSK = 0;
	for (uint8_t i=0; i<MAX_INTERRUPT; i++)
	{
		lastTime[i] = 0;
		debounceTime[i] = 0;
        intFunc[i] = 0x0000;
	}
}

bool Interrupt::enableInterrupt(uint8_t ID, void (*Func)(void), uint8_t Mode, uint32_t Debounce)
{
	if ((Mode != CHANGE) && (Mode != RISING) && (Mode != FALLING))
		return false;
	switch(ID)
	{
		case I0:
		{
			EICRB |= (Mode<<0) & 0b00000011;
			EIMSK |= (1<<INT4);
			lastTime[I0] = 0;
			debounceTime[I0] = Debounce;
			intFunc[I0] = Func;
			return true;
		}
		break;
		case I1:
		{
			EICRB |= (Mode<<2) & 0b00001100;
			EIMSK |= (1<<INT5);
			lastTime[I1] = 0;
			debounceTime[I1] = Debounce;
			intFunc[I1] = Func;
			return true;
		}
		break;
	}
    return false;
}

bool Interrupt::disableInterrupt(uint8_t ID)
{
	switch(ID)
	{
		case I0:
		{
			EICRB &= ~(0b00000011);
			EIMSK &= ~(1<<INT4);
			lastTime[I0] = 0;
			debounceTime[I0] = 0;
			intFunc[I0] = 0x0000;
			return true;
		}
		break;
		case I1:
		{
			EICRA &= ~(0b00001100);
			EIMSK &= ~(1<<INT5);
			lastTime[I1] = 0;
			debounceTime[I1] = 0;
			intFunc[I1] = 0x0000;
			return true;
		}
		break;
	}
    return false;
}

#endif

#endif
