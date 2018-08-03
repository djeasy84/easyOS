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

#ifndef SERIAL_HW_H
#define SERIAL_HW_H

#include "./Time.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 32

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega2560__)

volatile uint8_t serial_buffer0[BUFFER_SIZE];
volatile uint8_t serial_buffer0_start_idx;
volatile uint8_t serial_buffer0_stop_idx;

class SerialHW0
{
    public:
        SerialHW0();

        void setup(uint32_t speed=9600);

        void flush();

        bool write(uint8_t data);
        bool read(uint8_t *data);

        bool write(uint8_t *data, uint8_t len);
        bool read(uint8_t *data, uint8_t *len, uint8_t max);

        bool print(unsigned char data);
        bool print(unsigned int data);
        bool print(unsigned long data);
        bool print(char data);
        bool print(int data);
        bool print(long data);
        bool print(double data);
        bool print(const char *data);

        bool println(unsigned char data);
        bool println(unsigned int data);
        bool println(unsigned long data);
        bool println(char data);
        bool println(int data);
        bool println(long data);
        bool println(double data);
        bool println(const char *data);

        bool readln(char *data, uint8_t max);
};

SerialHW0 SHW0;

/****************************************************************************************/

#if defined (__AVR_ATmega328P__)
ISR(USART_RX_vect)
#elif defined (__AVR_ATmega2560__)
ISR(USART0_RX_vect)
#endif
{
    serial_buffer0[serial_buffer0_stop_idx] = UDR0;
    if (serial_buffer0_stop_idx++ >= (BUFFER_SIZE-1))
        serial_buffer0_stop_idx = 0;
}

SerialHW0::SerialHW0()
{
    setup();
}

void SerialHW0::setup(uint32_t speed)
{
    serial_buffer0_start_idx = 0;
    serial_buffer0_stop_idx = 0;

    UCSR0A = 0x00;
    UCSR0B = 0x00;
    UCSR0C = 0x00;
    UCSR0A |= (1<<U2X0);
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0H = (((F_CPU/(speed*8)))-1) >> 8;
    UBRR0L = (((F_CPU/(speed*8)))-1);

    #if defined (__AVR_ATmega328P__)
    PORTD |= (1<<0);
    #elif defined (__AVR_ATmega2560__)
    PORTE |= (1<<0);
    #endif
}

void SerialHW0::flush()
{
    uint8_t buffer;
    while(read(&buffer));
}

bool SerialHW0::write(uint8_t data)
{
    UCSR0A |= (1<<TXC0);
    while((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = data;
    while((UCSR0A & (1<<TXC0)) == 0);
    return true;
}

bool SerialHW0::read(uint8_t *data)
{
    if (serial_buffer0_start_idx != serial_buffer0_stop_idx)
    {
        *data = serial_buffer0[serial_buffer0_start_idx];
        if (serial_buffer0_start_idx++ >= (BUFFER_SIZE-1))
            serial_buffer0_start_idx = 0;
        return true;
    }
    return false;
}

bool SerialHW0::write(uint8_t *data, uint8_t len)
{
    for (uint8_t i=0; i<len; i++)
    {
        if (!write(data[i]))
            return false;
    }
    return true;
}

bool SerialHW0::read(uint8_t *data, uint8_t *len, uint8_t max)
{
    *len = 0;
    //*data = 0;
    uint32_t start_microsec = ST.microsec();
    while(*len < max)
    {
		if (ST.time_diff(ST.microsec(), start_microsec) > 5)
            break;
        if (read(&data[*len]))
            (*len)++;
    }
    return true;
}

bool SerialHW0::print(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%c", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW0::print(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    return true;
}

bool SerialHW0::println(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::println(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW0::readln(char *data, uint8_t max)
{
    uint8_t len = 0;
    *data = 0;
    uint32_t start_microsec = ST.microsec();
    while(len < max)
    {
        if (read((uint8_t *)&data[len]))
            len++;
        if (len > 2)
        {
            if (data[len-2] == '\r' && data[len-1] == '\n')
                break;
        }
        if (ST.time_diff(ST.microsec(), start_microsec) > 5000)
            return false;
        start_microsec = ST.microsec();
    }
    data[len-2] = 0;
    return true;
}

#endif

#if defined (__AVR_ATmega2560__)

#if defined (SERIAL_HW_1_M)
volatile uint8_t serial_buffer1[BUFFER_SIZE];
volatile uint8_t serial_buffer1_start_idx;
volatile uint8_t serial_buffer1_stop_idx;

class SerialHW1
{
    public:
        SerialHW1();

        void setup(uint32_t speed=9600, bool dStop=false);

        void flush();

        virtual bool write(uint8_t data);
        virtual bool read(uint8_t *data);

        bool write(uint8_t *data, uint8_t len);
        bool read(uint8_t *data, uint8_t *len, uint8_t max);

        bool print(unsigned char data);
        bool print(unsigned int data);
        bool print(unsigned long data);
        bool print(char data);
        bool print(int data);
        bool print(long data);
        bool print(double data);
        bool print(const char *data);

        bool println(unsigned char data);
        bool println(unsigned int data);
        bool println(unsigned long data);
        bool println(char data);
        bool println(int data);
        bool println(long data);
        bool println(double data);
        bool println(const char *data);

        bool readln(char *data, uint8_t max);
};

SerialHW1 SHW1;

/****************************************************************************************/

ISR(USART1_RX_vect)
{
    serial_buffer1[serial_buffer1_stop_idx] = UDR1;
    if (serial_buffer1_stop_idx++ >= (BUFFER_SIZE-1))
        serial_buffer1_stop_idx = 0;
}

SerialHW1::SerialHW1()
{
    setup();
}

void SerialHW1::setup(uint32_t speed, bool dStop)
{
    serial_buffer1_start_idx = 0;
    serial_buffer1_stop_idx = 0;

    UCSR1A = 0x00;
    UCSR1B = 0x00;
    UCSR1C = 0x00;
    UCSR1A |= (1<<U2X1);
    UCSR1B |= (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
    UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11) | ((dStop)?(1<<USBS1):(0<<USBS1));
    UBRR1H = (((F_CPU/(speed*8)))-1) >> 8;
    UBRR1L = (((F_CPU/(speed*8)))-1);

    PORTD |= (1<<2);
}

void SerialHW1::flush()
{
    uint8_t buffer;
    while(read(&buffer));
}

bool SerialHW1::write(uint8_t data)
{
    UCSR1A |= (1<<TXC1);
    while((UCSR1A & (1<<UDRE1)) == 0);
    UDR1 = data;
    while((UCSR1A & (1<<TXC1)) == 0);
    return true;
}

bool SerialHW1::read(uint8_t *data)
{
    if (serial_buffer1_start_idx != serial_buffer1_stop_idx)
    {
        *data = serial_buffer1[serial_buffer1_start_idx];
        if (serial_buffer1_start_idx++ >= (BUFFER_SIZE-1))
            serial_buffer1_start_idx = 0;
        return true;
    }
    return false;
}

bool SerialHW1::write(uint8_t *data, uint8_t len)
{
    for (uint8_t i=0; i<len; i++)
    {
        if (!write(data[i]))
            return false;
    }
    return true;
}

bool SerialHW1::read(uint8_t *data, uint8_t *len, uint8_t max)
{
    *len = 0;
    //*data = 0;
    uint32_t start_microsec = ST.microsec();
    while(*len < max)
    {
        if (ST.time_diff(ST.microsec(), start_microsec) > 5)
            break;
        if (read(&data[*len]))
            (*len)++;
    }
    return true;
}

bool SerialHW1::print(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%c", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW1::print(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    return true;
}

bool SerialHW1::println(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::println(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW1::readln(char *data, uint8_t max)
{
    uint8_t len = 0;
    *data = 0;
    uint32_t start_microsec = ST.microsec();
    while(len < max)
    {
        if (read((uint8_t *)&data[len]))
            len++;
        if (len > 2)
        {
            if (data[len-2] == '\r' && data[len-1] == '\n')
                break;
        }
        if (ST.time_diff(ST.microsec(), start_microsec) > 5000)
            return false;
        start_microsec = ST.microsec();
    }
    data[len-2] = 0;
    return true;
}
#endif

#if defined (SERIAL_HW_2_M)
volatile uint8_t serial_buffer2[BUFFER_SIZE];
volatile uint8_t serial_buffer2_start_idx;
volatile uint8_t serial_buffer2_stop_idx;

class SerialHW2
{
    public:
        SerialHW2();

        void setup(uint32_t speed=9600, bool dStop=false);

        void flush();

        virtual bool write(uint8_t data);
        virtual bool read(uint8_t *data);

        bool write(uint8_t *data, uint8_t len);
        bool read(uint8_t *data, uint8_t *len, uint8_t max);

        bool print(unsigned char data);
        bool print(unsigned int data);
        bool print(unsigned long data);
        bool print(char data);
        bool print(int data);
        bool print(long data);
        bool print(double data);
        bool print(const char *data);

        bool println(unsigned char data);
        bool println(unsigned int data);
        bool println(unsigned long data);
        bool println(char data);
        bool println(int data);
        bool println(long data);
        bool println(double data);
        bool println(const char *data);

        bool readln(char *data, uint8_t max);
};

SerialHW2 SHW2;

/****************************************************************************************/

ISR(USART2_RX_vect)
{
    serial_buffer2[serial_buffer2_stop_idx] = UDR2;
    if (serial_buffer2_stop_idx++ >= (BUFFER_SIZE-1))
        serial_buffer2_stop_idx = 0;
}

SerialHW2::SerialHW2()
{
    setup();
}

void SerialHW2::setup(uint32_t speed, bool dStop)
{
    serial_buffer2_start_idx = 0;
    serial_buffer2_stop_idx = 0;

    UCSR2A = 0x00;
    UCSR2B = 0x00;
    UCSR2C = 0x00;
    UCSR2A |= (1<<U2X2);
    UCSR2B |= (1<<RXEN2) | (1<<TXEN2) | (1<<RXCIE2);
    UCSR2C |= (1<<UCSZ20) | (1<<UCSZ21) | ((dStop)?(1<<USBS2):(0<<USBS2));
    UBRR2H = (((F_CPU/(speed*8)))-1) >> 8;
    UBRR2L = (((F_CPU/(speed*8)))-1);

    PORTH |= (1<<0);
}

void SerialHW2::flush()
{
    uint8_t buffer;
    while(read(&buffer));
}

bool SerialHW2::write(uint8_t data)
{
    UCSR2A |= (1<<TXC2);
    while((UCSR2A & (1<<UDRE2)) == 0);
    UDR2 = data;
    while((UCSR2A & (1<<TXC2)) == 0);
    return true;
}

bool SerialHW2::read(uint8_t *data)
{
    if (serial_buffer2_start_idx != serial_buffer2_stop_idx)
    {
        *data = serial_buffer2[serial_buffer2_start_idx];
        if (serial_buffer2_start_idx++ >= (BUFFER_SIZE-1))
            serial_buffer2_start_idx = 0;
        return true;
    }
    return false;
}

bool SerialHW2::write(uint8_t *data, uint8_t len)
{
    for (uint8_t i=0; i<len; i++)
    {
        if (!write(data[i]))
            return false;
    }
    return true;
}

bool SerialHW2::read(uint8_t *data, uint8_t *len, uint8_t max)
{
    *len = 0;
    //*data = 0;
    uint32_t start_microsec = ST.microsec();
    while(*len < max)
    {
        if (ST.time_diff(ST.microsec(), start_microsec) > 5)
            break;
        if (read(&data[*len]))
            (*len)++;
    }
    return true;
}

bool SerialHW2::print(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%c", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW2::print(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    return true;
}

bool SerialHW2::println(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::println(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW2::readln(char *data, uint8_t max)
{
    uint8_t len = 0;
    *data = 0;
    uint32_t start_microsec = ST.microsec();
    while(len < max)
    {
        if (read((uint8_t *)&data[len]))
            len++;
        if (len > 2)
        {
            if (data[len-2] == '\r' && data[len-1] == '\n')
                break;
        }
        if (ST.time_diff(ST.microsec(), start_microsec) > 5000)
            return false;
        start_microsec = ST.microsec();
    }
    data[len-2] = 0;
    return true;
}
#endif

#if defined (SERIAL_HW_3_M)
volatile uint8_t serial_buffer3[BUFFER_SIZE];
volatile uint8_t serial_buffer3_start_idx;
volatile uint8_t serial_buffer3_stop_idx;

class SerialHW3
{
    public:
        SerialHW3();

        void setup(uint32_t speed=9600, bool dStop=false);

        void flush();

        virtual bool write(uint8_t data);
        virtual bool read(uint8_t *data);

        bool write(uint8_t *data, uint8_t len);
        bool read(uint8_t *data, uint8_t *len, uint8_t max);

        bool print(unsigned char data);
        bool print(unsigned int data);
        bool print(unsigned long data);
        bool print(char data);
        bool print(int data);
        bool print(long data);
        bool print(double data);
        bool print(const char *data);

        bool println(unsigned char data);
        bool println(unsigned int data);
        bool println(unsigned long data);
        bool println(char data);
        bool println(int data);
        bool println(long data);
        bool println(double data);
        bool println(const char *data);

        bool readln(char *data, uint8_t max);
};

SerialHW3 SHW3;

/****************************************************************************************/

ISR(USART3_RX_vect)
{
    serial_buffer3[serial_buffer3_stop_idx] = UDR3;
    if (serial_buffer3_stop_idx++ >= (BUFFER_SIZE-1))
        serial_buffer3_stop_idx = 0;
}

SerialHW3::SerialHW3()
{
    setup();
}

void SerialHW3::setup(uint32_t speed, bool dStop)
{
    serial_buffer3_start_idx = 0;
    serial_buffer3_stop_idx = 0;

    UCSR3A = 0x00;
    UCSR3B = 0x00;
    UCSR3C = 0x00;
    UCSR3A |= (1<<U2X3);
    UCSR3B |= (1<<RXEN3) | (1<<TXEN3) | (1<<RXCIE3);
    UCSR3C |= (1<<UCSZ30) | (1<<UCSZ31) | ((dStop)?(1<<USBS3):(0<<USBS3));
    UBRR3H = (((F_CPU/(speed*8)))-1) >> 8;
    UBRR3L = (((F_CPU/(speed*8)))-1);

    PORTJ |= (1<<0);
}

void SerialHW3::flush()
{
    uint8_t buffer;
    while(read(&buffer));
}

bool SerialHW3::write(uint8_t data)
{
    UCSR3A |= (1<<TXC3);
    while((UCSR3A & (1<<UDRE3)) == 0);
    UDR3 = data;
    while((UCSR3A & (1<<TXC3)) == 0);
    return true;
}

bool SerialHW3::read(uint8_t *data)
{
    if (serial_buffer3_start_idx != serial_buffer3_stop_idx)
    {
        *data = serial_buffer3[serial_buffer3_start_idx];
        if (serial_buffer3_start_idx++ >= (BUFFER_SIZE-1))
            serial_buffer3_start_idx = 0;
        return true;
    }
    return false;
}

bool SerialHW3::write(uint8_t *data, uint8_t len)
{
    for (uint8_t i=0; i<len; i++)
    {
        if (!write(data[i]))
            return false;
    }
    return true;
}

bool SerialHW3::read(uint8_t *data, uint8_t *len, uint8_t max)
{
    *len = 0;
    //*data = 0;
    uint32_t start_microsec = ST.microsec();
    while(*len < max)
    {
        if (ST.time_diff(ST.microsec(), start_microsec) > 5)
            break;
        if (read(&data[*len]))
            (*len)++;
    }
    return true;
}

bool SerialHW3::print(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%c", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    return true;
}

bool SerialHW3::print(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    return true;
}

bool SerialHW3::println(unsigned char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(unsigned int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(unsigned long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%lu", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(char data)
{
    uint8_t buffer[4];
    sprintf((char *)buffer, "%u", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(int data)
{
    uint8_t buffer[6];
    sprintf((char *)buffer, "%d", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(long data)
{
    uint8_t buffer[11];
    sprintf((char *)buffer, "%ld", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(double data)
{
    uint8_t buffer[32];
    sprintf((char *)buffer, "%.2f", data);
    if (!write((uint8_t *)buffer, strlen((const char *)buffer)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::println(const char *data)
{
    if (!write((uint8_t *)data, strlen((const char *)data)))
        return false;
    if (!write((uint8_t *)"\r\n", 2))
        return false;
    return true;
}

bool SerialHW3::readln(char *data, uint8_t max)
{
    uint8_t len = 0;
    *data = 0;
    uint32_t start_microsec = ST.microsec();
    while(len < max)
    {
        if (read((uint8_t *)&data[len]))
            len++;
        if (len > 2)
        {
            if (data[len-2] == '\r' && data[len-1] == '\n')
                break;
        }
        if (ST.time_diff(ST.microsec(), start_microsec) > 5000)
            return false;
        start_microsec = ST.microsec();
    }
    data[len-2] = 0;
    return true;
}
#endif

#endif

#endif
