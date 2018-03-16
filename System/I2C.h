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

#ifndef I2C_H
#define I2C_H

#include "./Time.h"

#include <stdlib.h>

class InterIntegratedCircuit
{
    public:
        InterIntegratedCircuit();

        bool sendStart();
        bool sendDeviceAddressWithReadWrite(uint8_t devAddr, bool writeRead);
        bool sendData(uint8_t data);
        bool readData(uint8_t *data, bool noAck);
        bool sendStop();
};

InterIntegratedCircuit I2C;

/****************************************************************************************/

InterIntegratedCircuit::InterIntegratedCircuit()
{
    TWSR = 0;

    TWBR = ((F_CPU / 400000UL) - 16) / 2;

    TWCR = (1<<TWEN);
}

bool InterIntegratedCircuit::sendStart()
{
    TWDR = 0;
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    uint32_t start_microsec = ST.microsec();
    while((TWCR & (1<<TWINT)) == false)
    {
         if (labs(ST.microsec()-start_microsec) > 5)
            return false;
    }
    if ((TWSR & 0xF8) != 0x08 && (TWSR & 0xF8) != 0x10)
        return false;
    return true;
}

bool InterIntegratedCircuit::sendDeviceAddressWithReadWrite(uint8_t devAddr, bool writeRead)
{
    TWDR = writeRead;
    TWDR |= devAddr<<1;
    TWCR = (1<<TWINT) | (1<<TWEN);
    uint32_t start_microsec = ST.microsec();
    while((TWCR & (1<<TWINT)) == false)
    {
         if (labs(ST.microsec()-start_microsec) > 5)
            return false;
    }
    if (!writeRead)
    {
        if ((TWSR & 0xF8) != 0x18 && (TWSR & 0xF8) != 0x20)
            return false;
    }
    else
    {
        if ((TWSR & 0xF8) != 0x40 && (TWSR & 0xF8) != 0x48)
            return false;
    }
    return true;
}

bool InterIntegratedCircuit::sendData(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    uint32_t start_microsec = ST.microsec();
    while((TWCR & (1<<TWINT)) == false)
    {
         if (labs(ST.microsec()-start_microsec) > 5)
            return false;
    }
    if ((TWSR & 0xF8) != 0x28 && (TWSR & 0xF8) != 0x30)
        return false;
    return true;
}

bool InterIntegratedCircuit::readData(uint8_t *data, bool noAck)
{
    if (noAck)
        TWCR = (1 << TWINT) | (1 << TWEN);
    else
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    uint32_t start_microsec = ST.microsec();
    while((TWCR & (1<<TWINT)) == false)
    {
         if (labs(ST.microsec()-start_microsec) > 5)
            return false;
    }
    if ((TWSR & 0xF8) != 0x50 && (TWSR & 0xF8) != 0x58)
        return false;
    *data = TWDR;
    return true;
}

bool InterIntegratedCircuit::sendStop()
{
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
    return true;
}

#endif
