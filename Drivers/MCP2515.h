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

#ifndef MCP2515_H
#define MCP2515_H

#define LL 0
#define LI 1
#define HI 2
#define HH 3

#define NORMAL_MODE 0
#define LOOPBACK_MODE 1
#define LISTENONLY_MODE 2

#define MAX_BYTES_IN_MESSAGE 8

class CanBus
{
    public:
        bool setup(uint8_t ss /* ARDUINO_PIN_10 */, uint8_t mode, uint16_t speed, uint32_t mask = 0x000, uint32_t filter1 = 0x000, uint32_t filter2 = 0x000, uint32_t filter3 = 0x000, uint32_t filter4 = 0x000, uint32_t filter5 = 0x000, uint32_t filter6 = 0x000);

        bool read(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data);
        bool write(uint32_t id, uint8_t ext, uint8_t len, uint8_t *data, uint8_t priority = LL);

    private:
        void reset();

        bool enterConfigMode();
        bool enterListenOnlyMode();
        bool enterLoopbackMode();
        bool enterNormalMode();

        bool setupSpeed(uint16_t speed);
        bool setupFilter(uint32_t mask, uint32_t filter1, uint32_t filter2, uint32_t filter3, uint32_t filter4, uint32_t filter5, uint32_t filter6);

        void getRegistry(uint8_t addr, uint8_t *value);
        void getRegistry(uint8_t addr, uint8_t len, uint8_t *value);

        bool setRegistry(uint8_t addr, uint8_t value);
        bool setRegistry(uint8_t addr, uint8_t len, uint8_t *value);
        bool setRegistryWithMask(uint8_t addr, uint8_t mask, uint8_t value);

        uint8_t checkMessage();

        void readBuffer1(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data);
        void readBuffer2(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data);

        uint8_t ssPin;
};

CanBus CB;

/****************************************************************************************/

bool CanBus::setup(uint8_t ss, uint8_t mode, uint16_t speed, uint32_t mask, uint32_t filter1, uint32_t filter2, uint32_t filter3, uint32_t filter4, uint32_t filter5, uint32_t filter6)
{
    ssPin = ss;

    DP.write(ssPin, true);

    reset();

    if (!enterConfigMode())
        return false;
    if (!setupFilter(mask, filter1, filter2, filter3, filter4, filter5, filter6))
        return false;
    if (mode == NORMAL_MODE)
    {
        if (!setupSpeed(speed))
            return false;
        if (!enterNormalMode())
            return false;
    }
    else if (mode == LOOPBACK_MODE)
    {
        if (!setupSpeed(speed))
            return false;
        if (!enterLoopbackMode())
            return false;
    }
    else
    {
        if (!enterListenOnlyMode())
            return false;
    }
    return true;
}

bool CanBus::read(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data)
{
    uint8_t value = checkMessage();
    if (value == 0)
        return false;
    else if (value == 1)
    {
        readBuffer1(id, ext, len, data);
        return true;
    }
    else if (value == 2)
    {
        readBuffer2(id, ext, len, data);
        return true;
    }
}

bool CanBus::write(uint32_t id, uint8_t ext, uint8_t len, uint8_t *data, uint8_t priority)
{
    uint8_t tx_addr[3] = { 0x30, 0x40, 0x50 } ;
    for (uint8_t j=0; j<3; j++)
    {
        uint8_t value = 0xFF;
        getRegistry(tx_addr[j], &value);
        if ((value & 0b00001000 /*0x08*/) == 0b00000000)
        {
            setRegistry(tx_addr[j], 0b00000000);
            len = (len > MAX_BYTES_IN_MESSAGE) ? MAX_BYTES_IN_MESSAGE : len;
            setRegistry(tx_addr[j]+6, len, data);
            setRegistry(tx_addr[j]+5, len);
            uint8_t tmp_id[4];
            if (ext != 0)
            {
                tmp_id[0] = (id & 0b00011111111000000000000000000000 /*0x1FE00000*/) >> 21;
                tmp_id[1] = (id & 0b00000000000111000000000000000000 /*0x001C0000*/) >> 13;
                tmp_id[1] = tmp_id[1] | (id & 0b00000000000000110000000000000000 /*0x00030000*/) >> 16;
                tmp_id[1] = tmp_id[1] | 0b00000000000000000000000000001000 /*0x08*/;
                tmp_id[2] = (id & 0b00000000000000001111111100000000 /*0x0000FF00*/) >> 8;
                tmp_id[3] = (id & 0b00000000000000000000000011111111 /*0x000000FF*/);
            }
            else
            {
                tmp_id[0] = (id & 0b011111111000 /*0x07F8*/) >> 3;
                tmp_id[1] = (id & 0b000000000111 /*0x0007*/) << 5;
                tmp_id[2] = 0;
                tmp_id[3] = 0;
            }
            setRegistry(tx_addr[j]+1, 4, tmp_id);
            setRegistryWithMask(tx_addr[j], 0b00001011 /*0x0B*/, 0b00001000 | priority);
            for (uint8_t i=0; i<128; i++)
            {
                uint8_t value = 0xFF;
                getRegistry(tx_addr[j], &value);
                if ((value & 0b00010000 /*0x10*/) == 0b00010000)
                {
                    setRegistryWithMask(tx_addr[j], 0b00001000 /*0x08*/, 0b00000000 /*0x00*/);
                    return false;
                }
                if ((value & 0b00001000 /*0x08*/) == 0b00000000)
                {
                    setRegistryWithMask(tx_addr[j], 0b00001000 /*0x08*/, 0b00000000 /*0x00*/);
                    return true;
                }
            }
            setRegistryWithMask(tx_addr[j], 0b00001000 /*0x08*/, 0b00000000 /*0x00*/);
            return false;
        }
    }
    return false;
}

void CanBus::reset()
{
    DP.write(ssPin, false);
    SPI.transfer(0b11000000);
    DP.write(ssPin, true);
    ST.wait_millisec(50);
}

bool CanBus::enterConfigMode()
{
    if (!setRegistryWithMask(0x0F, 0b11100000 /*0xE0*/, 0b10000000 /*0x80*/))
        return false;
    return true;
}

bool CanBus::enterListenOnlyMode()
{
    if (!setRegistryWithMask(0x0F, 0b11100000 /*0xE0*/, 0b01100000 /*0x60*/))
        return false;
    return true;
}

bool CanBus::enterLoopbackMode()
{
    if (!setRegistryWithMask(0x0F, 0b11100000 /*0xE0*/, 0b01000000 /*0x40*/))
        return false;
    return true;
}

bool CanBus::enterNormalMode()
{
    if (!setRegistryWithMask(0x0F, 0b11100000 /*0xE0*/, 0b00000000 /*0x00*/))
        return false;
    return true;
}

bool CanBus::setupSpeed(uint16_t speed)
{
    // https://www.kvaser.com/support/calculators/bit-timing-calculator/
    // BTQ: 8 - SP: 75% - SJW: 1 - ERR: 0%
    switch (speed)
    {
        case (125):
        {
            if (!setRegistry(0x2A,  0x07))
                return false;
            if (!setRegistry(0x29,  0x91))
                return false;
            if (!setRegistry(0x28,  0x01))
                return false;
        }
        break;
        case (250):
        {
            if (!setRegistry(0x2A,  0x03))
                return false;
            if (!setRegistry(0x29,  0x91))
                return false;
            if (!setRegistry(0x28,  0x01))
                return false;
        }
        break;
        case (500):
        {
            if (!setRegistry(0x2A,  0x01))
                return false;
            if (!setRegistry(0x29,  0x91))
                return false;
            if (!setRegistry(0x28,  0x01))
                return false;
        }
        break;
        case (1000):
        {
            if (!setRegistry(0x2A,  0x00))
                return false;
            if (!setRegistry(0x29,  0x91))
                return false;
            if (!setRegistry(0x28,  0x01))
                return false;
        }
        break;
        default:
        {
            return false;
        }
        break;
    }
    return true;
}

bool CanBus::setupFilter(uint32_t mask, uint32_t filter1, uint32_t filter2, uint32_t filter3, uint32_t filter4, uint32_t filter5, uint32_t filter6)
{
    if (mask == 0x000)
    {
        if (!setRegistryWithMask(0x60, 0b01100100 /*0x64*/, 0b01100100 /*0x24*/))
            return false;
        if (!setRegistryWithMask(0x70, 0b01100000 /*0x60*/, 0b01100000 /*0x20*/))
            return false;
        return true;
    }
    if (!setRegistryWithMask(0x60, 0b01100100 /*0x64*/, 0b00100100 /*0x24*/))
        return false;
    if (!setRegistryWithMask(0x70, 0b01100000 /*0x60*/, 0b00100000 /*0x20*/))
        return false;
    uint8_t tmp_id[4];
    tmp_id[0] = (mask & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (mask & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x20, 4, tmp_id))
        return false;
    if (!setRegistry(0x24, 4, tmp_id))
        return false;
    tmp_id[0] = (filter1 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter1 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x00, 4, tmp_id))
        return false;
    tmp_id[0] = (filter2 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter2 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x04, 4, tmp_id))
        return false;
    tmp_id[0] = (filter3 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter3 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x08, 4, tmp_id))
        return false;
    tmp_id[0] = (filter4 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter4 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x10, 4, tmp_id))
        return false;
    tmp_id[0] = (filter5 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter5 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x14, 4, tmp_id))
        return false;
    tmp_id[0] = (filter6 & 0b011111111000 /*0x07F8*/) >> 3;
    tmp_id[1] = (filter6 & 0b000000000111 /*0x0007*/) << 5;
    tmp_id[2] = 0;
    tmp_id[3] = 0;
    if (!setRegistry(0x18, 4, tmp_id))
        return false;
    return true;
}

void CanBus::getRegistry(uint8_t addr, uint8_t *value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x03);
    SPI.transfer(addr);
    *value = SPI.transfer(0x00);
    DP.write(ssPin, true);
}

void CanBus::getRegistry(uint8_t addr, uint8_t len, uint8_t *value)
{
    for (uint8_t i=0; i<len; i++)
        getRegistry(addr+i, value+i);
}

bool CanBus::setRegistry(uint8_t addr, uint8_t value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x02);
    SPI.transfer(addr);
    SPI.transfer(value);
    DP.write(ssPin, true);
    uint8_t saved_value = 0x00;
    getRegistry(addr, &saved_value);
    if (saved_value != value)
        return false;
    return true;
}

bool CanBus::setRegistry(uint8_t addr, uint8_t len, uint8_t *value)
{
    for (uint8_t i=0; i<len; i++)
    {
        if (!setRegistry(addr+i, value[i]))
            return false;
    }
    return true;
}

bool CanBus::setRegistryWithMask(uint8_t addr, uint8_t mask, uint8_t value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x05);
    SPI.transfer(addr);
    SPI.transfer(mask);
    SPI.transfer(value);
    DP.write(ssPin, true);
    uint8_t saved_value = 0x00;
    getRegistry(addr, &saved_value);
    if ((saved_value & mask) != value)
        return false;
    return true;
}

uint8_t CanBus::checkMessage()
{
    DP.write(ssPin, false);
    SPI.transfer(0xA0);
    uint8_t value = SPI.transfer(0x00);
    DP.write(ssPin, true);
    if ((value & 0b00000011 /*0x03*/) == 0b00000000 /*0x00*/)
        return 0;
    else if ((value & 0b00000001 /*0x01*/) == 0b00000001 /*0x01*/)
        return 1;
    else if ((value & 0b00000010 /*0x02*/) == 0b00000010 /*0x02*/)
        return 2;
}

void CanBus::readBuffer1(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data)
{
    uint8_t tmp_buffer[4];
    getRegistry(0x61, 4, tmp_buffer);
    if ((tmp_buffer[1] & 0b00001000 /*0x08*/) == 0b00001000 /*0x08*/)
    {
        *ext = 1;
        *id = tmp_buffer[3];
        *id = *id + ((uint32_t)tmp_buffer[2] << 8);
        *id = *id + ((uint32_t)(tmp_buffer[1] & 0b00000011) << 16) + ((uint32_t)(tmp_buffer[1] & 0b11100000) << 13);
        *id = *id + ((uint32_t)tmp_buffer[0] << 21);
    }
    else
    {
        *ext = 0;
        *id = 0x00000000 + (tmp_buffer[0] << 3) + (tmp_buffer[1] >> 5);
    }
    uint8_t value = 0x00;
    getRegistry(0x65, &value);
    *len = (value & 0b00001111 /*0x0F*/);
    *len = (*len > MAX_BYTES_IN_MESSAGE) ? MAX_BYTES_IN_MESSAGE : *len;
    getRegistry(0x66, *len, data);
    setRegistryWithMask(0x2C, 0b00000001 /*0x01*/, 0b00000000 /*0x00*/);
}

void CanBus::readBuffer2(uint32_t *id, uint8_t *ext, uint8_t *len, uint8_t *data)
{
    uint8_t tmp_buffer[4];
    getRegistry(0x71, 4, tmp_buffer);
    if ((tmp_buffer[1] & 0b00001000 /*0x08*/) == 0b00001000 /*0x08*/)
    {
        *ext = 1;
        *id = tmp_buffer[3];
        *id = *id + ((uint32_t)tmp_buffer[2] << 8);
        *id = *id + ((uint32_t)(tmp_buffer[1] & 0b00000011) << 16) + ((uint32_t)(tmp_buffer[1] & 0b11100000) << 13);
        *id = *id + ((uint32_t)tmp_buffer[0] << 21);
    }
    else
    {
        *ext = 0;
        *id = (tmp_buffer[0] << 3) + (tmp_buffer[1] >> 5);
    }
    uint8_t value = 0x00;
    getRegistry(0x75, &value);
    *len = (value & 0b00001111 /*0x0F*/);
    *len = (*len > MAX_BYTES_IN_MESSAGE) ? MAX_BYTES_IN_MESSAGE : *len;
    getRegistry(0x76, *len, data);
    setRegistryWithMask(0x2C, 0b00000010 /*0x02*/, 0b00000000 /*0x00*/);
}

#endif
