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

#ifndef EEPROM_H
#define EEPROM_H

#include <avr/eeprom.h>

class ElectricallyErasableProgrammableROM
{
    public:
        bool read(uint16_t addr, uint8_t *data);
        bool write(uint16_t addr, uint8_t data);

        bool read(uint16_t addr, uint8_t *data, uint16_t len);
        bool write(uint16_t addr, uint8_t *data, uint16_t len);
};

ElectricallyErasableProgrammableROM ER;

/****************************************************************************************/

bool ElectricallyErasableProgrammableROM::read(uint16_t addr, uint8_t *data)
{
    if (addr >= EEPROM_CPU)
        return false;
    *data = eeprom_read_byte((uint8_t *) addr);
    return true;
}

bool ElectricallyErasableProgrammableROM::write(uint16_t addr, uint8_t data)
{
    if (addr >= EEPROM_CPU)
        return false;
    eeprom_write_byte((uint8_t *) addr, data);
    eeprom_busy_wait();
    uint8_t data_check = eeprom_read_byte((uint8_t *) addr);
    if (data_check != data)
        return false;
    return true;
}

bool ElectricallyErasableProgrammableROM::read(uint16_t addr, uint8_t *data, uint16_t len)
{
    for (uint16_t i=0; i<len; i++)
    {
        if (!read(addr+i, &data[i]))
            return false;
    }
    return true;
}

bool ElectricallyErasableProgrammableROM::write(uint16_t addr, uint8_t *data, uint16_t len)
{
    for (uint16_t i=0; i<len; i++)
    {
        if (!write(addr+i, data[i]))
            return false;
    }
    return true;
}

#endif
