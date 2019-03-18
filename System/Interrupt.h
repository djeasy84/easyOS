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

uint32_t lastTime;
uint32_t debounceTime;
void (*interruptFunc)(void);

ISR(PCINT0_vect)
{
	if (interruptFunc == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime) > debounceTime)
	{
		lastTime = ST.millisec();
		interruptFunc();
	}
}
ISR(PCINT1_vect)
{
	if (interruptFunc == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime) > debounceTime)
	{
		lastTime = ST.millisec();
		interruptFunc();
	}
}
ISR(PCINT2_vect)
{
	if (interruptFunc == 0x0000)
		return;
	if (ST.time_diff(ST.millisec(), lastTime) > debounceTime)
	{
		lastTime = ST.millisec();
		interruptFunc();
	}
}

class Interrupt
{
    public:
        void setup(void (*pFunc)(void), uint32_t tDebounce = 25);

        bool enableInterrupt(uint16_t id);
		bool disableInterrupt(uint16_t id);
};

Interrupt IM;

/****************************************************************************************/

#if defined (__AVR_ATmega328P__)
    #if defined (__BOARD_arduinoUNO__)
        #define ARDUINO_PIN_A0 100
        #define ARDUINO_PIN_A1 101
        #define ARDUINO_PIN_A2 102
		#define ARDUINO_PIN_A3 103
	#endif
    #if defined (__BOARD_arduinoNANO__)
		#define ARDUINO_PIN_A0 100
        #define ARDUINO_PIN_A1 101
        #define ARDUINO_PIN_A2 102
		#define ARDUINO_PIN_A3 103
	#endif
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
#endif

void Interrupt::setup(void (*pFunc)(void), uint32_t tDebounce)
{
	PCMSK0 = PCMSK1 = PCMSK2 = PCIFR = PCICR = 0;
	PCICR = (1<<2) | (1<<1) | (1<<0);

	lastTime = 0;
	debounceTime = tDebounce;
	interruptFunc = pFunc;
}

bool Interrupt::enableInterrupt(uint16_t id)
{
	#if defined (__AVR_ATmega328P__)
	switch (id)
	{
		case 100:  //PCINT8
		{
			PCMSK1 |= (1<<0);
		}
		break;
		case 101:  //PCINT9
		{
			PCMSK1 |= (1<<1);
		}
		break;
		case 102:  //PCINT10
		{
			PCMSK1 |= (1<<2);
		}
		break;
		case 103:  //PCINT11
		{
			PCMSK1 |= (1<<3);
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
	#endif
	#if defined (__AVR_ATmega2560__)
	switch (id)
	{
		case 108:  //PCINT16
		{
			PCMSK2 |= (1<<0);
		}
		break;
		case 109:  //PCINT17
		{
			PCMSK2 |= (1<<1);
		}
		break;
		case 110:  //PCINT18
		{
			PCMSK2 |= (1<<2);
		}
		break;
		case 111:  //PCINT19
		{
			PCMSK2 |= (1<<3);
		}
		break;
		case 112:  //PCINT20
		{
			PCMSK2 |= (1<<4);
		}
		break;
		case 113:  //PCINT21
		{
			PCMSK2 |= (1<<5);
		}
		break;
		case 114:  //PCINT22
		{
			PCMSK2 |= (1<<6);
		}
		break;
		case 115:  //PCINT23
		{
			PCMSK2 |= (1<<7);
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
	#endif

	return true;
}

bool Interrupt::disableInterrupt(uint16_t id)
{
    #if defined (__AVR_ATmega328P__)
	switch (id)
	{
		case 100:  //PCINT8
		{
			PCMSK1 &= ~(1<<0);
		}
		break;
		case 101:  //PCINT9
		{
			PCMSK1 &= ~(1<<1);
		}
		break;
		case 102:  //PCINT10
		{
			PCMSK1 &= ~(1<<2);
		}
		break;
		case 103:  //PCINT11
		{
			PCMSK1 &= ~(1<<3);
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
	#endif
	#if defined (__AVR_ATmega2560__)
	switch (id)
	{
		case 108:  //PCINT16
		{
			PCMSK2 &= ~(1<<0);
		}
		break;
		case 109:  //PCINT17
		{
			PCMSK2 &= ~(1<<1);
		}
		break;
		case 110:  //PCINT18
		{
			PCMSK2 &= ~(1<<2);
		}
		break;
		case 111:  //PCINT19
		{
			PCMSK2 &= ~(1<<3);
		}
		break;
		case 112:  //PCINT20
		{
			PCMSK2 &= ~(1<<4);
		}
		break;
		case 113:  //PCINT21
		{
			PCMSK2 &= ~(1<<5);
		}
		break;
		case 114:  //PCINT22
		{
			PCMSK2 &= ~(1<<6);
		}
		break;
		case 115:  //PCINT23
		{
			PCMSK2 &= ~(1<<7);
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
	#endif

	return true;
}

#endif
