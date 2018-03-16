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
        static void MacToBytes(const char *mac, uint8_t *bytes);
        static void IPToBytes(const char *ip, uint8_t *bytes);
        static void PortToBytes(const uint16_t port, uint8_t *bytes);
};

/****************************************************************************************/

void IPAddress::MacToBytes(const char *mac, uint8_t *bytes)
{
    uint8_t data[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sscanf(mac,"%x:%x:%x:%x:%x:%x", &data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
    for (uint8_t i=0; i<6; i++)
        bytes[i] = data[i];
}

void IPAddress::IPToBytes(const char *ip, uint8_t *bytes)
{
    uint8_t data[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sscanf(ip,"%d.%d.%d.%d", &data[0], &data[1], &data[2], &data[3]);
    for (uint8_t i=0; i<4; i++)
        bytes[i] = data[i];
}

void IPAddress::PortToBytes(const uint16_t port, uint8_t *bytes)
{
    bytes[0] = port >> 8;
    bytes[1] = port & 0x00FF;
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
        bool setup(uint8_t ss /* ARDUINO_PIN_10 */, const char *mac, const char *ip, const char *netmask, const char *gateway = "");

        bool open(const char *ip, uint16_t port, uint8_t type = TCP, uint16_t lPort = 0, uint8_t mode = CLIENT);
        bool connected();
        uint16_t available();
        bool read(uint16_t len, uint8_t *data);
        bool write(uint16_t len, uint8_t *data);
        bool close();

    private:
        void reset();

        void getRegistry(uint16_t addr, uint8_t *value);
        void getRegistry(uint16_t addr, uint16_t len, uint8_t *value);

        bool setRegistry(uint16_t addr, uint8_t value);
        bool setRegistry(uint16_t addr, uint16_t len, uint8_t *value);

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

bool Ethernet::setup(uint8_t ss, const char *mac, const char *ip, const char *netmask, const char *gateway)
{
    incPort = 0;
    sockType = NAT;
    srcPort = MIN_CLIENT_SOURCE_PORT;
    memset(destIPUDP, 0, 4);
    memset(destPortUDP, 0, 2);
    ssPin = ss;
    DP.write(ssPin, true);
    reset();
    uint8_t byteMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    IPAddress::MacToBytes(mac, byteMac);
    if (!setRegistry(0x0009, 6, byteMac))
        return false;
    uint8_t byteIP[4] = {0x00, 0x00, 0x00, 0x00};
    IPAddress::IPToBytes(ip, byteIP);
    if (!setRegistry(0x000F, 4, byteIP))
        return false;
    IPAddress::IPToBytes(netmask, byteIP);
    if (!setRegistry(0x0005, 4, byteIP))
        return false;
    IPAddress::IPToBytes(gateway, byteIP);
    if (!setRegistry(0x0001, 4, byteIP))
        return false;
    if (!setRegistry(0x001A, 0b00000011))
        return false;
    if (!setRegistry(0x001B, 0b00000011))
        return false;
    if (!setRegistry(0x0400, 0x00))
        return false;
    if (!setRegistry(0x0500, 0x00))
        return false;
    if (!setRegistry(0x0600, 0x00))
        return false;
    if (!setRegistry(0x0700, 0x00))
        return false;
    return true;
}

bool Ethernet::open(const char *ip, uint16_t port, uint8_t type, uint16_t lPort, uint8_t mode)
{
    incPort = lPort;
    sockType = type;
    if (mode == CLIENT)
    {
        if (sockType == TCP)
        {
            close();
            if (!setRegistry(0x0400, 0b00000001))
                return false;
            uint8_t bytesSrcPort[2] = {0, 0};
            IPAddress::PortToBytes(getSourcePort(), bytesSrcPort);
            if (!setRegistry(0x0404, 2, bytesSrcPort))
                return false;
            setRegistry(0x0401, 0x01);
            uint8_t byteStatus = 0x00;
            getRegistry(0x0403, &byteStatus);
            if (byteStatus != 0x13)
            {
                close();
                return false;
            }
            uint8_t bytesDestIP[4] = {0x00, 0x00, 0x00, 0x00};
            IPAddress::IPToBytes(ip, bytesDestIP);
            setRegistry(0x040C, 4, bytesDestIP);
            uint8_t bytesDestPort[2] = {0, 0};
            IPAddress::PortToBytes(port, bytesDestPort);
            setRegistry(0x0410, 2, bytesDestPort);
            setRegistry(0x0401, 0x04);
            getRegistry(0x0403, &byteStatus);
            while (byteStatus != 0x17)
            {
                getRegistry(0x0403, &byteStatus);
                if (byteStatus == 0x00)
                {
                    close();
                    return false;
                }
            }
            return true;
        }
        else if (sockType == UDP)
        {
            close();
            if (!setRegistry(0x0400, 0b00000010))
                return false;
            uint8_t bytesSrcPort[2] = {0, 0};
            IPAddress::PortToBytes(getSourcePort(), bytesSrcPort);
            if (!setRegistry(0x0404, 2, bytesSrcPort))
                return false;
            setRegistry(0x0401, 0x01);
            uint8_t byteStatus = 0x00;
            getRegistry(0x0403, &byteStatus);
            if (byteStatus != 0x22)
            {
                close();
                return false;
            }
            IPAddress::IPToBytes(ip, destIPUDP);
            IPAddress::PortToBytes(port, destPortUDP);
            return true;
        }
        return false;
    }
    else if (mode == SERVER)
    {
        return false;
    }
    return false;
}

bool Ethernet::connected()
{
    if (sockType == TCP)
    {
        uint8_t byte = 0x00;
        getRegistry(0x0403, &byte);
        if (byte != 0x17)
            return false;
        return true;
    }
    else if (sockType == UDP)
    {
        uint8_t byte = 0x00;
        getRegistry(0x0403, &byte);
        if (byte != 0x22)
            return false;
        return true;
    }
    return false;
}

uint16_t Ethernet::available()
{
    if (sockType == TCP)
    {
        uint16_t rx_data_len = 0x0000;
        getRegistry(0x0426, (uint8_t *)&rx_data_len+1);
        getRegistry(0x0427, (uint8_t *)&rx_data_len+0);
        return rx_data_len;
    }
    else if (sockType == UDP)
    {
        uint16_t rx_data_len = 0x0000;
        getRegistry(0x0426, (uint8_t *)&rx_data_len+1);
        getRegistry(0x0427, (uint8_t *)&rx_data_len+0);
        return (rx_data_len < 8)?0:(rx_data_len - 8);
    }
    return false;
}

bool Ethernet::read(uint16_t len, uint8_t *data)
{
    if (sockType == TCP)
    {
        uint16_t rx_size = available();
        if (rx_size == 0)
            return false;
        if (rx_size < len)
            return false;
        uint16_t rx_rd = 0x0000;
        getRegistry(0x0428, (uint8_t *)&rx_rd+1);
        getRegistry(0x0429, (uint8_t *)&rx_rd+0);
        uint16_t rx_offset = rx_rd & 0x1FFF;
        uint16_t rx_mem_address = 0x6000 + rx_offset;
        if (rx_offset + len > 8192)
        {
            uint16_t rx_size_last = 8192 - rx_offset;
            getRegistry(rx_mem_address, rx_size_last, data);
            getRegistry(0x6000, len - rx_size_last, data+rx_size_last);
        }
        else
        {
            getRegistry(rx_mem_address, len, data);
        }
        rx_rd += len;
        setRegistry(0x0428, (uint8_t)(rx_rd >> 8));
        setRegistry(0x0429, (uint8_t)(rx_rd & 0x00FF));
        setRegistry(0x0401, 0x40);
        return true;
    }
    else if (sockType == UDP)
    {
        uint16_t rx_size = available() + 8;
        if (rx_size == 0)
            return false;
        if (rx_size - 8 < len)
            return false;
        uint16_t rx_rd = 0x0000;
        getRegistry(0x0428, (uint8_t *)&rx_rd+1);
        getRegistry(0x0429, (uint8_t *)&rx_rd+0);
        uint16_t rx_offset = rx_rd & 0x1FFF;
        uint16_t rx_mem_address = 0x6000 + rx_offset;
        uint8_t header_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        if (rx_offset + 8 > 8192)
        {
            uint16_t rx_size_last = 8192 - rx_offset;
            getRegistry(rx_mem_address, rx_size_last, header_data);
            getRegistry(0x6000, 8 - rx_size_last, header_data+rx_size_last);
            rx_offset = 8 - rx_size_last;
        }
        else
        {
            getRegistry(rx_mem_address, 8, header_data);
            rx_offset += 8;
        }
        uint16_t rx_packet_size = (uint16_t)((header_data[6]<<8)|(header_data[7]&0x00FF));
        rx_mem_address = 0x6000 + rx_offset;
        if (rx_offset + rx_packet_size > 8192)
        {
            uint16_t rx_size_last = 8192 - rx_offset;
            getRegistry(rx_mem_address, rx_size_last, data);
            getRegistry(0x6000, rx_packet_size - rx_size_last, data+rx_size_last);
        }
        else
        {
            getRegistry(rx_mem_address, rx_packet_size, data);
        }
        rx_rd += 8 + rx_packet_size;
        setRegistry(0x0428, (uint8_t)(rx_rd >> 8));
        setRegistry(0x0429, (uint8_t)(rx_rd & 0x00FF));
        setRegistry(0x0401, 0x40);
        return true;
    }
    return false;
}

bool Ethernet::write(uint16_t len, uint8_t *data)
{
    if (sockType == TCP)
    {
        if (len > 8192)
            return false;
        uint16_t tx_size = 0x0000;
        getRegistry(0x0420, (uint8_t *)&tx_size+1);
        getRegistry(0x0421, (uint8_t *)&tx_size+0);
        while (tx_size < len)
        {
            getRegistry(0x0420, (uint8_t *)&tx_size+1);
            getRegistry(0x0421, (uint8_t *)&tx_size+0);
        }
        uint16_t tx_wr = 0x0000;
        getRegistry(0x0424, (uint8_t *)&tx_wr+1);
        getRegistry(0x0425, (uint8_t *)&tx_wr+0);
        uint16_t tx_offset = tx_wr & 0x1FFF;
        uint16_t tx_mem_address = 0x4000 + tx_offset;
        if (tx_offset + len > 8192)
        {
            uint16_t tx_size_last = 8192 - tx_offset;
            setRegistry(tx_mem_address, tx_size_last, data);
            setRegistry(0x4000, len - tx_size_last, data+tx_size_last);
        }
        else
        {
            setRegistry(tx_mem_address, len, data);
        }
        tx_wr += len;
        setRegistry(0x0424, (uint8_t)(tx_wr >> 8));
        setRegistry(0x0425, (uint8_t)(tx_wr & 0x00FF));
        setRegistry(0x0401, 0x20);
        uint8_t byteInterrupt = 0x00;
        getRegistry(0x0402, &byteInterrupt);
        while ((byteInterrupt & 0b00010000) != 0b00010000)
        {
            uint8_t byteStatus = 0x00;
            getRegistry(0x0403, &byteStatus);
            if (byteStatus == 0x00)
            {
                close();
                return false;
            }
        }
        return true;
    }
    else if (sockType == UDP)
    {
        if (len > 8192)
            return false;
        uint16_t tx_size = 0x0000;
        getRegistry(0x0420, (uint8_t *)&tx_size+1);
        getRegistry(0x0421, (uint8_t *)&tx_size+0);
        while (tx_size < len)
        {
            getRegistry(0x0420, (uint8_t *)&tx_size+1);
            getRegistry(0x0421, (uint8_t *)&tx_size+0);
        }
        setRegistry(0x040C, 4, destIPUDP);
        setRegistry(0x0410, 2, destPortUDP);
        uint16_t tx_wr = 0x0000;
        getRegistry(0x0424, (uint8_t *)&tx_wr+1);
        getRegistry(0x0425, (uint8_t *)&tx_wr+0);
        uint16_t tx_offset = tx_wr & 0x1FFF;
        uint16_t tx_mem_address = 0x4000 + tx_offset;
        if (tx_offset + len > 8192)
        {
            uint16_t tx_size_last = 8192 - tx_offset;
            setRegistry(tx_mem_address, tx_size_last, data);
            setRegistry(0x4000, len - tx_size_last, data+tx_size_last);
        }
        else
        {
            setRegistry(tx_mem_address, len, data);
        }
        tx_wr += len;
        setRegistry(0x0424, (uint8_t)(tx_wr >> 8));
        setRegistry(0x0425, (uint8_t)(tx_wr & 0x00FF));
        setRegistry(0x0401, 0x20);
        uint8_t byteStatus = 0x00;
        getRegistry(0x0401, &byteStatus);
        while (byteStatus);
        return true;
    }
    return false;
}

bool Ethernet::close()
{
    if (sockType == TCP)
    {
        setRegistry(0x0401, 0x08);
        uint8_t byte = 0x00;
        getRegistry(0x0403, &byte);
        while (byte != 0x18 && byte != 0x1A && byte != 0x1B && byte != 0x1D)
            getRegistry(0x0403, &byte);
        setRegistry(0x0401, 0x10);
        byte = 0x00;
        getRegistry(0x0403, &byte);
        while (byte != 0x00)
            getRegistry(0x0403, &byte);
        return true;
    }
    else if (sockType == UDP)
    {
        setRegistry(0x0401, 0x10);
        uint8_t byte = 0x00;
        getRegistry(0x0403, &byte);
        while (byte != 0x00)
            getRegistry(0x0403, &byte);
        return true;
    }
    return false;
}

void Ethernet::reset()
{
    DP.write(ssPin, false);
    SPI.transfer(0xF0);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0b10000000);
    DP.write(ssPin, true);
    ST.wait_millisec(50);
}

void Ethernet::getRegistry(uint16_t addr, uint8_t *value)
{
    DP.write(ssPin, false);
    SPI.transfer(0x0F);
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)(addr & 0x00FF));
    *value = SPI.transfer(0x00);
    DP.write(ssPin, true);
}

void Ethernet::getRegistry(uint16_t addr, uint16_t len, uint8_t *value)
{
    for (uint16_t i=0; i<len; i++)
        getRegistry(addr+i, value+i);
}

bool Ethernet::setRegistry(uint16_t addr, uint8_t value)
{
    DP.write(ssPin, false);
    SPI.transfer(0xF0);
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)(addr & 0x00FF));
    SPI.transfer(value);
    DP.write(ssPin, true);
    uint8_t saved_value = 0x00;
    getRegistry(addr, &saved_value);
    if (saved_value != value)
        return false;
    return true;
}

bool Ethernet::setRegistry(uint16_t addr, uint16_t len, uint8_t *value)
{
    bool result = true;
    for (uint16_t i=0; i<len; i++)
    {
        if (!setRegistry(addr+i, value[i]))
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
