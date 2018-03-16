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

#if defined (__AVR_ATmega328P__)
    #if defined (__FCPU_16MHz__)
        #define F_CPU 16000000UL
    #endif
    #if defined (__FCPU_8MHz__)
        #define F_CPU 8000000UL
    #endif
    #define EEPROM_CPU 1024UL
#endif

#if defined (__AVR_ATmega2560__)
    #define F_CPU 16000000UL
    #define EEPROM_CPU 4096UL
#endif

#if defined (TIME_M)
    #include "./System/Time.h"
#endif

#if defined (MEMORY_M)
    #include "./System/Memory.h"
#endif

#if defined (SERIAL_HW_M)
    #include "./System/SerialHW.h"
#endif
#if defined (SPI_M)
    #include "./System/SPI.h"
#endif
#if defined (I2C_M)
    #include "./System/I2C.h"
#endif

#if defined (INTERRUPT_M)
	#include "./System/Interrupt.h"
#endif

#if defined (PWM_M)
    #include "./System/PWM.h"
#endif
#if defined (DIGITAL_PIN_M)
    #include "./System/DigitalPin.h"
#endif
#if defined (ANALOG_PIN_M)
    #include "./System/AnalogPin.h"
#endif

#if defined (EEPROM_M)
    #include "./System/EEPROM.h"
#endif

#if defined (MULTI_TASKING_M)
    #include "./System/MultiTasking.h"
#endif

#if defined (PID_M)
    #include "./System/PID.h"
#endif
