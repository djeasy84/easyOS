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

#ifndef W5100_H
#define W5100_H

#include <stdio.h>
#include <string.h>

class IPAddress
{
    public:
        static bool MacToBytes(const char *mac, uint8_t *bytes);
        static bool IPToBytes(const char *ip, uint8_t *bytes);
        static bool PortToBytes(const uint16_t port, uint8_t *bytes);
};

/****************************************************************************************/

bool IPAddress::MacToBytes(const char *mac, uint8_t *bytes)
{
    return (sscanf(mac,"%x:%x:%x:%x:%x:%x", &bytes[0], &bytes[1], &bytes[2], &bytes[3], &bytes[4], &bytes[5]) == 6) ? true : false;
}

bool IPAddress::IPToBytes(const char *ip, uint8_t *bytes)
{
    uint16_t data[4] = {0x00, 0x00, 0x00, 0x00};
    if (sscanf(ip,"%u.%u.%u.%u", &data[0], &data[1], &data[2], &data[3]) != 4)
        return false;
    for (uint8_t i=0; i<4; i++)
    {
        if (data[i] > 255)
            return false;
        bytes[i] = (uint8_t)data[i];
    }
    return true;
}

bool IPAddress::PortToBytes(const uint16_t port, uint8_t *bytes)
{
    bytes[0] = (uint8_t)(port >> 8);
    bytes[1] = (uint8_t)(port & 0x00FF);
}

/****************************************************************************************/
/****************************************************************************************/

#define NAT 0 // NOT ASSIGNED TYPE
#define TCP 1
#define UDP 2

#define CLIENT 1
#define SERVER 2

#define MIN_CLIENT_SOURCE_PORT 50000

class Ethernet
{
    public:
        int setup(uint8_t ss /* ARDUINO_PIN_10 */, const char *mac = "00:11:22:33:44:55", const char *ip = "192.168.1.100", const char *netmask = "255.255.255.0", const char *gateway = "0.0.0.0");

    private:
        void reset();

        void getRegistry(uint8_t block, uint16_t addr, uint8_t *value);
        void getRegistry(uint8_t block, uint16_t addr, uint16_t len, uint8_t *value);

        bool setRegistry(uint8_t block, uint16_t addr, uint8_t value);
        bool setRegistry(uint8_t block, uint16_t addr, uint16_t len, uint8_t *value);

        uint16_t getSourcePort();

        uint8_t ssPin;
        uint8_t sockType;
        uint16_t srcPort;
        uint16_t incPort;
        uint8_t destIPUDP[4];
        uint8_t destPortUDP[2];
};

Ethernet ETH;

/****************************************************************************************/

int Ethernet::setup(uint8_t ss, const char *mac, const char *ip, const char *netmask, const char *gateway)
{
    incPort = 0;
    sockType = NAT;
    srcPort = MIN_CLIENT_SOURCE_PORT;
    memset(destIPUDP, 0, 4);
    memset(destPortUDP, 0, 2);
    ssPin = ss;
    DP.write(ssPin, true);
    reset();
    uint8_t sockBuffSize[8] = {16, 0, 0, 0, 0, 0, 0, 0};
    for (uint8_t i=0; i<8; i++)
    {
        if (!setRegistry(i+1, 0x001E, sockBuffSize[i]))
            return false;
        if (!setRegistry(i+1, 0x001F, sockBuffSize[i]))
            return false;
    }
    uint8_t byteMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!IPAddress::MacToBytes(mac, byteMac))
        return false;
    if (!setRegistry(0, 0x0009, 6, byteMac))
        return false;
    uint8_t byteIP[4] = {0, 0, 0, 0};
    if (!IPAddress::IPToBytes(ip, byteIP))
        return false;
    if (!setRegistry(0, 0x000F, 4, byteIP))
        return false;
    if (!IPAddress::IPToBytes(netmask, byteIP))
        return false;
    if (!setRegistry(0, 0x0005, 4, byteIP))
        return false;
    if (!IPAddress::IPToBytes(gateway, byteIP))
        return false;
    if (!setRegistry(0, 0x0001, 4, byteIP))
        return false;
    return true;
}

void Ethernet::reset()
{
    DP.write(ssPin, false);
    SPI.transfer(0x0000);
    SPI.transfer(0x0000);
    SPI.transfer(0b00000100);
    SPI.transfer(0b10000000);
    DP.write(ssPin, true);
    ST.wait_millisec(50);
}

void Ethernet::getRegistry(uint8_t block, uint16_t addr, uint8_t *value)
{
    DP.write(ssPin, false);
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)(addr & 0x00FF));
    uint8_t ctrlByte = 0b00000000;
    if (block != 0)
        ctrlByte |= (1 + 4 * (block - 1));
    SPI.transfer(ctrlByte);
    *value = SPI.transfer(0x00);
    DP.write(ssPin, true);
}

void Ethernet::getRegistry(uint8_t block, uint16_t addr, uint16_t len, uint8_t *value)
{
    for (uint16_t i=0; i<len; i++)
        getRegistry(block, addr+i, value+i);
}

bool Ethernet::setRegistry(uint8_t block, uint16_t addr, uint8_t value)
{
    DP.write(ssPin, false);
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)(addr & 0x00FF));
    uint8_t ctrlByte = 0b00000100;
    if (block != 0)
        ctrlByte |= (1 + 4 * (block - 1));
    SPI.transfer(ctrlByte);
    SPI.transfer(value);
    DP.write(ssPin, true);
    uint8_t saved_value = 0x00;
    getRegistry(block, addr, &saved_value);
    if (saved_value != value)
        return false;
    return true;
}

bool Ethernet::setRegistry(uint8_t block, uint16_t addr, uint16_t len, uint8_t *value)
{
    bool result = true;
    for (uint16_t i=0; i<len; i++)
    {
        if (!setRegistry(block, addr+i, value[i]))
           result = false;
    }
    return result;
}

uint16_t Ethernet::getSourcePort()
{
    if (incPort != 0)
        return incPort;
    uint16_t newPort = srcPort;
    if (srcPort++ < MIN_CLIENT_SOURCE_PORT)
        srcPort = MIN_CLIENT_SOURCE_PORT;
    return newPort;
}

#endif
