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

#define SYS_CM (0x00 << 3)
#define SOCK_CM (0x01 << 3)
#define SOCK_TX (0x02 << 3)
#define SOCK_RX (0x03 << 3)

#define NAT 0 // NOT ASSIGNED TYPE
#define TCP 1
#define UDP 2

#define CLIENT 1
#define SERVER 2

#define NVS 0 // NOT VALID STATE
#define CLOSED 1
#define LISTEN 2
#define CONNECTED 3
#define WAIT_CLOSE 4

#define MIN_CLIENT_SOURCE_PORT 50000

class Ethernet
{
    public:
        bool setup(uint8_t ss /* ARDUINO_PIN_10 */, const char *mac = "00:11:22:33:44:55", const char *ip = "192.168.1.100", const char *netmask = "255.255.255.0", const char *gateway = "0.0.0.0");

        bool openServer(uint16_t port);
        bool openClient(const char *ip, uint16_t port, uint8_t type = TCP, uint16_t lPort = 0);

        uint8_t status();

        unsigned int available();
        bool read(uint16_t len, uint8_t *data);
        bool write(uint16_t len, uint8_t *data);
        bool close();

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
    uint8_t sockBuffSize[8] = {16, 0, 0, 0, 0, 0, 0, 0};
    for (uint8_t i=0; i<8; i++)
    {
        if (!setRegistry((1+(4*i))<<3, 0x001E, sockBuffSize[i]))
            return false;
        if (!setRegistry((1+(4*i))<<3, 0x001F, sockBuffSize[i]))
            return false;
    }
    uint8_t byteMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!IPAddress::MacToBytes(mac, byteMac))
        return false;
    if (!setRegistry(SYS_CM, 0x0009, 6, byteMac))
        return false;
    uint8_t byteIP[4] = {0, 0, 0, 0};
    if (!IPAddress::IPToBytes(ip, byteIP))
        return false;
    if (!setRegistry(SYS_CM, 0x000F, 4, byteIP))
        return false;
    if (!IPAddress::IPToBytes(netmask, byteIP))
        return false;
    if (!setRegistry(SYS_CM, 0x0005, 4, byteIP))
        return false;
    if (!IPAddress::IPToBytes(gateway, byteIP))
        return false;
    if (!setRegistry(SYS_CM, 0x0001, 4, byteIP))
        return false;
    return true;
}

bool Ethernet::openServer(uint16_t port)
{
    uint8_t bytePHY = 0x00;
    getRegistry(SYS_CM, 0x002E, &bytePHY);
    if ((bytePHY & 0b00000001) == 0b00000000)
        return false;
    incPort = port;
    sockType = TCP;
    close();
    if (!setRegistry(SOCK_CM, 0x0000, 0b00000001))
        return false;
    uint8_t bytesSrcPort[2] = {0, 0};
    IPAddress::PortToBytes(incPort, bytesSrcPort);
    if (!setRegistry(SOCK_CM, 0x0004, 2, bytesSrcPort))
        return false;
    setRegistry(SOCK_CM, 0x0001, 0x01);
    uint8_t byteStatus = 0x00;
    getRegistry(SOCK_CM, 0x0003, &byteStatus);
    if (byteStatus != 0x13)
    {
        close();
        return false;
    }
    setRegistry(SOCK_CM, 0x0001, 0x02);
    byteStatus = 0x00;
    getRegistry(SOCK_CM, 0x0003, &byteStatus);
    if (byteStatus != 0x14)
    {
        close();
        return false;
    }
    return true;
}

bool Ethernet::openClient(const char *ip, uint16_t port, uint8_t type, uint16_t lPort)
{
    uint8_t bytePHY = 0x00;
    getRegistry(SYS_CM, 0x002E, &bytePHY);
    if ((bytePHY & 0b00000001) == 0b00000000)
        return false;
    incPort = lPort;
    sockType = type;
    if (sockType == TCP)
    {
        close();
        if (!setRegistry(SOCK_CM, 0x0000, 0b00000001))
            return false;
        uint8_t bytesSrcPort[2] = {0, 0};
        IPAddress::PortToBytes(getSourcePort(), bytesSrcPort);
        if (!setRegistry(SOCK_CM, 0x0004, 2, bytesSrcPort))
            return false;
        setRegistry(SOCK_CM, 0x0001, 0x01);
        uint8_t byteStatus = 0x00;
        getRegistry(SOCK_CM, 0x0003, &byteStatus);
        if (byteStatus != 0x13)
        {
            close();
            return false;
        }
        uint8_t bytesDestIP[4] = {0x00, 0x00, 0x00, 0x00};
        IPAddress::IPToBytes(ip, bytesDestIP);
        setRegistry(SOCK_CM, 0x000C, 4, bytesDestIP);
        uint8_t bytesDestPort[2] = {0, 0};
        IPAddress::PortToBytes(port, bytesDestPort);
        setRegistry(SOCK_CM, 0x0010, 2, bytesDestPort);
        setRegistry(SOCK_CM, 0x0001, 0x04);
        getRegistry(SOCK_CM, 0x0003, &byteStatus);
        while (byteStatus != 0x17)
        {
            getRegistry(SOCK_CM, 0x0003, &byteStatus);
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
        if (!setRegistry(SOCK_CM, 0x0000, 0b00000010))
            return false;
        uint8_t bytesSrcPort[2] = {0, 0};
        IPAddress::PortToBytes(65000, bytesSrcPort);
        if (!setRegistry(SOCK_CM, 0x0004, 2, bytesSrcPort))
            return false;
        setRegistry(SOCK_CM, 0x0001, 0x01);
        uint8_t byteStatus = 0x00;
        getRegistry(SOCK_CM, 0x0003, &byteStatus);
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

uint8_t Ethernet::status()
{
    uint8_t byteStatus = 0x00;
    getRegistry(SOCK_CM, 0x0003, &byteStatus);
    switch (byteStatus)
    {
        case 0x00:
            return CLOSED;
        break;
        case 0x1C:
            return WAIT_CLOSE;
        break;
        case 0x14:
            return LISTEN;
        break;
        case 0x17:
            return CONNECTED;
        break;
    }
    return NVS;
}

unsigned int Ethernet::available()
{
    if (sockType == TCP)
    {
        uint16_t rx_data_len = 0;
        getRegistry(SOCK_CM, 0x0026, (uint8_t *)&rx_data_len+1);
        getRegistry(SOCK_CM, 0x0027, (uint8_t *)&rx_data_len+0);
        return rx_data_len;
    }
    else if (sockType == UDP)
    {
        uint16_t rx_data_len = 0;
        getRegistry(SOCK_CM, 0x0026, (uint8_t *)&rx_data_len+1);
        getRegistry(SOCK_CM, 0x0027, (uint8_t *)&rx_data_len+0);
        if (rx_data_len >= 8)
        {
            uint16_t rx_rd = 0x0000;
            getRegistry(SOCK_CM, 0x0028, (uint8_t *)&rx_rd+1);
            getRegistry(SOCK_CM, 0x0029, (uint8_t *)&rx_rd+0);
            uint16_t rx_offset = rx_rd & 0x3FFF;
            uint16_t rx_mem_address = 0x0000+rx_offset;
            getRegistry(SOCK_RX, rx_mem_address+6, 1, (uint8_t *)&rx_data_len+1);
            getRegistry(SOCK_RX, rx_mem_address+7, 1, (uint8_t *)&rx_data_len+0);
            return rx_data_len;
        }
        return 0;
    }
    return 0;
}

bool Ethernet::read(uint16_t len, uint8_t *data)
{
    if (sockType == TCP)
    {
		/*uint8_t byteStatus = 0x00;
		getRegistry(SOCK_CM, 0x0003, &byteStatus);
		if (byteStatus != 0x17)
		{
			close();
			return false;
		}*/
        uint16_t rx_size = available();
        if (rx_size == 0)
            return false;
        if (rx_size < len)
            return false;
        uint16_t rx_rd = 0x0000;
        getRegistry(SOCK_CM, 0x0028, (uint8_t *)&rx_rd+1);
        getRegistry(SOCK_CM, 0x0029, (uint8_t *)&rx_rd+0);
        uint16_t rx_offset = rx_rd & 0x3FFF;
        uint16_t rx_mem_address = 0x0000+rx_offset;
        if (rx_offset+len > 16384)
        {
            uint16_t rx_size_last = 16384-rx_offset;
            getRegistry(SOCK_RX, rx_mem_address, rx_size_last, data);
            getRegistry(SOCK_RX, 0x0000, len-rx_size_last, data+rx_size_last);
        }
        else
        {
            getRegistry(SOCK_RX, rx_mem_address, len, data);
        }
        rx_rd += len;
        setRegistry(SOCK_CM, 0x0028, (uint8_t)(rx_rd >> 8));
        setRegistry(SOCK_CM, 0x0029, (uint8_t)(rx_rd & 0x00FF));
        setRegistry(SOCK_CM, 0x0001, 0x40);
        return true;
    }
    else if (sockType == UDP)
    {
		/*uint8_t byteStatus = 0x00;
		getRegistry(SOCK_CM, 0x0003, &byteStatus);
		if (byteStatus != 0x22)
		{
			close();
			return false;
		}*/
        uint16_t rx_size = available();
        if (rx_size == 0)
            return false;
        if (rx_size < len)
            return false;
        uint16_t rx_rd = 0x0000;
        getRegistry(SOCK_CM, 0x0028, (uint8_t *)&rx_rd+1);
        getRegistry(SOCK_CM, 0x0029, (uint8_t *)&rx_rd+0);
        uint16_t rx_offset = rx_rd & 0x3FFF;
        uint16_t rx_mem_address = 0x0000+rx_offset+8;
        if (rx_offset+rx_size > 16384)
        {
            uint16_t rx_size_last = 16384-rx_offset;
            getRegistry(rx_mem_address, rx_size_last, data);
            getRegistry(SOCK_RX, 0x0000, rx_size-rx_size_last, data+rx_size_last);
        }
        else
        {
            getRegistry(SOCK_RX, rx_mem_address, len, data);
        }
        rx_rd += 8+rx_size;
        setRegistry(SOCK_CM, 0x0028, (uint8_t)(rx_rd >> 8));
        setRegistry(SOCK_CM, 0x0029, (uint8_t)(rx_rd & 0x00FF));
        setRegistry(SOCK_CM, 0x0001, 0x40);
        return true;
    }
    return false;
}

bool Ethernet::write(uint16_t len, uint8_t *data)
{
	if (sockType == TCP)
    {
		uint8_t byteStatus = 0x00;
		getRegistry(SOCK_CM, 0x0003, &byteStatus);
		if (byteStatus != 0x17)
		{
            close();
			return false;
		}
        if (len > 16384)
            return false;
        uint16_t tx_size = 0x0000;
        getRegistry(SOCK_CM, 0x0020, (uint8_t *)&tx_size+1);
        getRegistry(SOCK_CM, 0x0021, (uint8_t *)&tx_size+0);
        if (tx_size < len)
			return false;
        setRegistry(SOCK_CM, 0x0002, 0b00011000);
        uint16_t tx_wr = 0x0000;
        getRegistry(SOCK_CM, 0x0024, (uint8_t *)&tx_wr+1);
        getRegistry(SOCK_CM, 0x0025, (uint8_t *)&tx_wr+0);
        uint16_t tx_offset = tx_wr & 0x3FFF;
        uint16_t tx_mem_address = 0x0000+tx_offset;
        if (tx_offset+len > 16384)
        {
            uint16_t tx_size_last = 16384-tx_offset;
            setRegistry(SOCK_TX, tx_mem_address, tx_size_last, data);
            setRegistry(SOCK_TX, 0x0000, len-tx_size_last, data+tx_size_last);
        }
        else
        {
            setRegistry(SOCK_TX, tx_mem_address, len, data);
        }
        tx_wr += len;
        setRegistry(SOCK_CM, 0x0024, (uint8_t)(tx_wr >> 8));
        setRegistry(SOCK_CM, 0x0025, (uint8_t)(tx_wr & 0x00FF));
        setRegistry(SOCK_CM, 0x0001, 0x20);
        uint8_t byteInterrupt = 0x00;
        getRegistry(SOCK_CM, 0x0002, &byteInterrupt);
        while ((byteInterrupt & 0b00010000) != 0b00010000)
        {
            byteStatus = 0x00;
            getRegistry(SOCK_CM, 0x0003, &byteStatus);
            if (byteStatus == 0x00)
            {
                close();
                return false;
            }
            byteInterrupt = 0x00;
			getRegistry(SOCK_CM, 0x0002, &byteInterrupt);
            if ((byteInterrupt & 0b00001000) != 0b00001000)
            {
                close();
                return false;
            }
        }
        return true;
    }
    else if (sockType == UDP)
    {
		uint8_t byteStatus = 0x00;
		getRegistry(SOCK_CM, 0x0003, &byteStatus);
		if (byteStatus != 0x22)
		{
			close();
			return false;
		}
        if (len > 16384)
            return false;
        uint16_t tx_size = 0x0000;
        getRegistry(SOCK_CM, 0x0020, (uint8_t *)&tx_size+1);
        getRegistry(SOCK_CM, 0x0021, (uint8_t *)&tx_size+0);
        while (tx_size < len)
        {
            getRegistry(SOCK_CM, 0x0020, (uint8_t *)&tx_size+1);
            getRegistry(SOCK_CM, 0x0021, (uint8_t *)&tx_size+0);
        }
        setRegistry(SOCK_CM, 0x000C, 4, destIPUDP);
        setRegistry(SOCK_CM, 0x0010, 2, destPortUDP);
        uint16_t tx_wr = 0x0000;
        getRegistry(SOCK_CM, 0x0024, (uint8_t *)&tx_wr+1);
        getRegistry(SOCK_CM, 0x0025, (uint8_t *)&tx_wr+0);
        uint16_t tx_offset = tx_wr & 0x3FFF;
        uint16_t tx_mem_address = 0x0000+tx_offset;
        if (tx_offset+len > 16384)
        {
            uint16_t tx_size_last = 16384-tx_offset;
            setRegistry(SOCK_TX, tx_mem_address, tx_size_last, data);
            setRegistry(SOCK_TX, 0x0000, len-tx_size_last, data+tx_size_last);
        }
        else
        {
            setRegistry(SOCK_TX, tx_mem_address, len, data);
        }
        tx_wr += len;
        setRegistry(SOCK_CM, 0x0024, (uint8_t)(tx_wr >> 8));
        setRegistry(SOCK_CM, 0x0025, (uint8_t)(tx_wr & 0x00FF));
        setRegistry(SOCK_CM, 0x0001, 0x20);
        return true;
    }
    return false;
}

bool Ethernet::close()
{
    if (sockType == TCP)
        setRegistry(SOCK_CM, 0x0001, 0x08);
    else if (sockType == UDP)
        setRegistry(SOCK_CM, 0x0001, 0x10);
    else
        return false;
    uint8_t byte = 0x00;
    getRegistry(SOCK_CM, 0x0003, &byte);
    while (byte != 0x00)
        getRegistry(SOCK_CM, 0x0003, &byte);
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
    SPI.transfer(block | 0b00000000);
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
    SPI.transfer(block | 0b00000100);
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
