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

#ifndef MODBUS_H
#define MODBUS_H

class ModBusMaster
{
    public:
        void setup(uint8_t we);

        bool readSingleInputWP8024(uint8_t address, uint8_t input, bool *data);  // ~16ms
        bool readMultipleInputWP8024(uint8_t address, bool *data);  // ~16ms

        bool writeSingleOutputWP8024(uint8_t address, uint8_t output, bool data);  // ~19ms
        bool writeMultipleOutputWP8024(uint8_t address, bool *data);  // ~21ms

        bool readSingleRegisterWP3066(uint8_t address, uint8_t input, float *data);  // ~35ms
        bool readMultipleRegisterWP3066(uint8_t address, float *data);  // ~167ms

        bool writeNewAddressWP8024(uint8_t newAddress);
        bool writeNewAddressWP3066(uint8_t newAddress);

    private:
        bool readMultipleInput(uint8_t address, uint16_t start, uint16_t quantity, bool *data);

        bool writeSingleOutput(uint8_t address, uint16_t registry, bool data);
        bool writeMultipleOutput(uint8_t address, uint16_t start, uint16_t quantity, bool *data);

        bool readMultipleRegister(uint8_t address, uint16_t start, uint16_t quantity, uint16_t *data);
        bool writeSingleRegister(uint8_t address, uint16_t registry, uint16_t value);

        bool sendCommand(uint8_t address, uint8_t *data, uint8_t len);
        bool receiveResponse(uint8_t address, uint8_t *data, uint8_t len);

        uint16_t calculareCRC(uint8_t *data, uint8_t len);

        uint8_t wePin;
};

ModBusMaster MBM;

/****************************************************************************************/

void ModBusMaster::setup(uint8_t we)
{
    wePin = we;
    SHW1.setup(9600);
    DP.write(wePin, false);
}

bool ModBusMaster::readSingleInputWP8024(uint8_t address, uint8_t input, bool *data)
{
    if (!(input >= 1 && input <= 8))
        return false;
    bool arrayData[8];
    if (!MBM.readMultipleInputWP8024(address, arrayData))
        return false;
    *data = arrayData[input-1];
    return true;
}

bool ModBusMaster::readMultipleInputWP8024(uint8_t address, bool *data)
{
    if (!(address >= 0x0001 && address <= 0x00FE))
        return false;
    return readMultipleInput(address, 0x0000, 0x0008, data);
}

bool ModBusMaster::writeSingleOutputWP8024(uint8_t address, uint8_t output, bool data)
{
    if (!(address >= 0x0001 && address <= 0x00FE))
        return false;
    if (!(output >= 1 && output <= 4))
        return false;
    return writeSingleOutput(address, output-1, (data) ? 0xFF00 : 0x0000);
}

bool ModBusMaster::writeMultipleOutputWP8024(uint8_t address, bool *data)
{
    if (!(address >= 0x0001 && address <= 0x00FE))
        return false;
    return writeMultipleOutput(address, 0x0000, 0x0004, data);
}

bool ModBusMaster::readSingleRegisterWP3066(uint8_t address, uint8_t input, float *data)
{
    if (!(address >= 0x0001 && address <= 0x00FE))
        return false;
    if (!(input >= 1 && input <= 8))
        return false;
    uint16_t buffer[8];
    if (!readMultipleRegister(address, input-1, 0x0001, buffer))
        return false;
    for (uint8_t i=0; i<8; i++)
    {
        if (buffer[i] == 65535)
            data[i] = -999.99;
        else if (buffer[i] < 10000)
            data[i] = (float)buffer[i] / 10;
        else if (buffer[i] > 10000)
            data[i] = -((float)buffer[i] - 10000) / 10;

    }
    return true;
}

bool ModBusMaster::readMultipleRegisterWP3066(uint8_t address, float *data)
{
    if (!(address >= 0x0001 && address <= 0x00FE))
        return false;
    uint16_t buffer[8];
    if (!readMultipleRegister(address, 0x0000, 0x0008, buffer))
        return false;
    for (uint8_t i=0; i<8; i++)
    {
        if (buffer[i] == 65535)
            data[i] = -999.99;
        else if (buffer[i] < 10000)
            data[i] = (float)buffer[i] / 10;
        else if (buffer[i] > 10000)
            data[i] = -((float)buffer[i] - 10000) / 10;

    }
    return true;
}

bool ModBusMaster::writeNewAddressWP8024(uint8_t newAddress)
{
    if (!(newAddress >= 0x0001 && newAddress <= 0x00FE))
        return false;
    return writeSingleRegister(0x00, 0x0064, newAddress);
}

bool ModBusMaster::writeNewAddressWP3066(uint8_t newAddress)
{
    if (!(newAddress >= 0x0001 && newAddress <= 0x00FE))
        return false;
    return writeSingleRegister(0x00, 0x0064, newAddress);
}

bool ModBusMaster::readMultipleInput(uint8_t address, uint16_t start, uint16_t quantity, bool *data)
{
    if (!(start >= 0x0000 && start <= 0xFFFF))
        return false;
    if (!(quantity >= 0x0001 && quantity <= 0x07D0))
        return false;
    uint8_t buffer_tx[1+2+2];
    buffer_tx[0] = 0x02;
    buffer_tx[1] = (uint8_t)(start >> 8);
    buffer_tx[2] = (uint8_t)(start & 0x00FF);
    buffer_tx[3] = (uint8_t)(quantity >> 8);
    buffer_tx[4] = (uint8_t)(quantity & 0x00FF);
    if (!sendCommand(address, buffer_tx, sizeof(buffer_tx)))
        return false;
    uint8_t buffer_rx[1+1+(quantity/8)];
    if (!receiveResponse(address, buffer_rx, sizeof(buffer_rx)))
        return false;
    for (uint8_t i=0; i<(quantity/8); i++)
    {
        for (uint8_t j=0; j<8; j++)
            data[(i*8)+j] = (buffer_rx[2+i] & (1 << j)) ? true : false;
    }
    return true;
}

bool ModBusMaster::writeSingleOutput(uint8_t address, uint16_t registry, bool data)
{
    if (!(registry >= 0x0000 && registry <= 0xFFFF))
        return false;
    uint8_t buffer_tx[1+2+2];
    buffer_tx[0] = 0x05;
    buffer_tx[1] = (uint8_t)(registry >> 8);
    buffer_tx[2] = (uint8_t)(registry & 0x00FF);
    buffer_tx[3] = (uint8_t)(((data) ? 0xFF00 : 0x000) >> 8);
    buffer_tx[4] = (uint8_t)(((data) ? 0xFF00 : 0x000) & 0x00FF);
    if (!sendCommand(address, buffer_tx, sizeof(buffer_tx)))
        return false;
    uint8_t buffer_rx[1+2+2];
    if (!receiveResponse(address, buffer_rx, sizeof(buffer_rx)))
        return false;
    for (uint8_t i=0; i<sizeof(buffer_rx); i++)
    {
        if (buffer_rx[i] != buffer_tx[i])
            return false;
    }
    return true;
}

bool ModBusMaster::writeMultipleOutput(uint8_t address, uint16_t start, uint16_t quantity, bool *data)
{
    if (!(start >= 0x0000 && start <= 0xFFFF))
        return false;
    if (!(quantity >= 0x0001 && quantity <= 0x07B0))
        return false;
    uint8_t buffer_tx[1+2+2+1+(1+quantity/8)];
    buffer_tx[0] = 0x0F;
    buffer_tx[1] = (uint8_t)(start >> 8);
    buffer_tx[2] = (uint8_t)(start & 0x00FF);
    buffer_tx[3] = (uint8_t)(quantity >> 8);
    buffer_tx[4] = (uint8_t)(quantity & 0x00FF);
    buffer_tx[5] = 1+(quantity/8);
    for (uint8_t i=0; i<1+(quantity/8); i++)
        buffer_tx[6+i] = 0x00;
    for (uint8_t i=0; i<quantity; i++)
        buffer_tx[6+(i/8)] |= ((data[i]) ? 1 : 0) << (i % 8);
    if (!sendCommand(address, buffer_tx, sizeof(buffer_tx)))
        return false;
    uint8_t buffer_rx[1+2+2];
    if (!receiveResponse(address, buffer_rx, sizeof(buffer_rx)))
        return false;
    for (uint8_t i=0; i<sizeof(buffer_rx); i++)
    {
        if (buffer_rx[i] != buffer_tx[i])
            return false;
    }
    return true;
}

bool ModBusMaster::readMultipleRegister(uint8_t address, uint16_t start, uint16_t quantity, uint16_t *data)
{
    if (!(start >= 0x0000 && start <= 0xFFFF))
        return false;
    if (!(quantity >= 0x0001 && quantity <= 0x007D))
        return false;
    uint8_t buffer_tx[1+2+2];
    buffer_tx[0] = 0x03;
    buffer_tx[1] = (uint8_t)(start >> 8);
    buffer_tx[2] = (uint8_t)(start & 0x00FF);
    buffer_tx[3] = (uint8_t)(quantity >> 8);
    buffer_tx[4] = (uint8_t)(quantity & 0x00FF);
    if (!sendCommand(address, buffer_tx, sizeof(buffer_tx)))
        return false;
    uint8_t buffer_rx[1+1+(quantity*2)];
    if (!receiveResponse(address, buffer_rx, sizeof(buffer_rx)))
        return false;
    for (uint8_t i=0; i<quantity; i++)
        data[i] = ((uint16_t)buffer_rx[2+(i*2)+0]) << 8 | ((uint16_t)buffer_rx[2+(i*2)+1]);
    return true;
}

bool ModBusMaster::writeSingleRegister(uint8_t address, uint16_t registry, uint16_t value)
{
    if (!(registry >= 0x0000 && registry <= 0xFFFF))
        return false;
    if (!(value >= 0x0000 && value <= 0xFFFF))
        return false;
    uint8_t buffer_tx[1+2+2];
    buffer_tx[0] = 0x06;
    buffer_tx[1] = (uint8_t)(registry >> 8);
    buffer_tx[2] = (uint8_t)(registry & 0x00FF);
    buffer_tx[3] = (uint8_t)(value >> 8);
    buffer_tx[4] = (uint8_t)(value & 0x00FF);
    if (!sendCommand(address, buffer_tx, sizeof(buffer_tx)))
        return false;
    uint8_t buffer_rx[1+2+2];
    if (!receiveResponse(address, buffer_rx, sizeof(buffer_rx)))
        return false;
    for (uint8_t i=0; i<sizeof(buffer_rx); i++)
    {
        if (buffer_rx[i] != buffer_tx[i])
            return false;
    }
    return true;
}

bool ModBusMaster::sendCommand(uint8_t address, uint8_t *data, uint8_t len)
{
    DP.write(wePin, true);
    SHW1.flush();
    uint8_t buffer[1+len+2];
    buffer[0] = address;
    for (uint8_t i=0; i<len; i++)
        buffer[1+i] = data[i];
    uint16_t crc = calculareCRC(buffer, 1+len);
    buffer[1+len+0] = crc & 0x00FF;
    buffer[1+len+1] = crc >> 8;
    SHW1.write(buffer, 1+len+2);
    DP.write(wePin, false);
    return true;
}

bool ModBusMaster::receiveResponse(uint8_t address, uint8_t *data, uint8_t len)
{
    uint8_t buffer[1+len+2];
    uint32_t time = ST.millisec();
    uint8_t idx = 0;
    while(true)
    {
        if (ST.time_diff(ST.millisec(), time) > 250)
            return false;
        if (SHW1.read(&buffer[idx]))
            idx++;
        if (idx >= 1+len+2)
            break;
    }
    int16_t crc = calculareCRC(buffer, 1+len);
    if (!(buffer[1+len+0] == (uint8_t)(crc & 0x00FF) && buffer[1+len+1] == (uint8_t)(crc >> 8)))
        return false;
    if (buffer[0] != address)
        return false;
    for (uint8_t i=0; i<len; i++)
        data[i] = buffer[1+i];
    return true;
}

uint16_t ModBusMaster::calculareCRC(uint8_t *data, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint8_t i=0; i<len; i++)
    {
        crc ^= (uint16_t)data[i];
        for (uint8_t j=8; j!=0; j--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

#endif
