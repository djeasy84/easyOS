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

#ifndef TC1602_H
#define TC1602_H

#define CMD_DISPLAY_CLEAR 0x01
#define CMD_CURSOR_HOME 0x02
#define CMD_DISPLAY_ENTRY 0x04
#define CMD_DISPLAY_CONTROL 0x08
#define CMD_CURSOR_ADDRESS 0x80

#include <stdio.h>

class Display
{
    public:
        bool setup(uint8_t dc /*ARDUINO_PIN_8*/, uint8_t en /*ARDUINO_PIN_9*/, uint8_t data0 /*ARDUINO_PIN_4*/, uint8_t data1 /*ARDUINO_PIN_5*/, uint8_t data2 /*ARDUINO_PIN_6*/, uint8_t data3 /*ARDUINO_PIN_7*/, uint8_t led /*ARDUINO_PIN_10*/, uint8_t rows, uint8_t cols);

        void write(const char *row0=0, const char *row1=0, const char *row2=0, const char *row3=0);

        void printf(uint8_t row, const char *format, ...);

    private:
        void data(uint8_t value);
        void command(uint8_t cmd, uint8_t val=0);

        void send(uint8_t value);

        uint8_t dcPin;
        uint8_t enPin;
        uint8_t dataPin[4];
        uint8_t ledPin;

        uint8_t nRows;
        uint8_t nCols;

        uint8_t rowsOffset[4];
};

Display LCD;

/****************************************************************************************/

bool Display::setup(uint8_t dc, uint8_t en, uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t led, uint8_t rows, uint8_t cols)
{
    if (rows < 1 || rows > 4)
        return false;

    rowsOffset[0] = 0x00;  rowsOffset[1] = 0x40;  rowsOffset[2] = 0x14;  rowsOffset[3] = 0x54;

    nRows = rows;
    nCols = cols;

    dcPin = dc;
    enPin = en;
    dataPin[0] = data0;
    dataPin[1] = data1;
    dataPin[2] = data2;
    dataPin[3] = data3;
    ledPin = led;

    DP.write(dcPin, false);
    DP.write(enPin, false);
    DP.write(dataPin[0], false);
    DP.write(dataPin[1], false);
    DP.write(dataPin[2], false);
    DP.write(dataPin[3], false);
    DP.write(ledPin, true);

    send(0b00000011);
    _delay_us(5000);
    send(0b00000011);
    _delay_us(5000);
    send(0b00000011);
    _delay_us(250);
    send(0b00000010);

    command(CMD_DISPLAY_CONTROL, 0b00000100);
    command(CMD_DISPLAY_CLEAR);
    command(CMD_CURSOR_HOME);

    return true;
}

void Display::write(const char *row0, const char *row1, const char *row2, const char *row3)
{
    const char *tmpData[4] = {row0, row1, row2, row3};
    command(CMD_CURSOR_HOME);
    for (uint8_t i=0; i<nRows; i++)
    {
        if (tmpData[i] == 0)
            continue;
        command(CMD_CURSOR_ADDRESS, rowsOffset[i]);
        bool foundTerminator = false;
        for (uint8_t j=0; j<nCols; j++)
        {
            if (tmpData[i][j] == 0 || tmpData[i][j] == '\0')
                foundTerminator = true;
            if (foundTerminator)
                data(' ');
            else
                data(tmpData[i][j]);
        }
    }
}

void Display::printf(uint8_t row, const char *format, ...)
{
    if (row >= nRows)
        return;
    char *tmpRows[4] = {0, 0, 0, 0};
    char buffer[nCols+1];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, nCols+1, format, args);
    tmpRows[row] = buffer;
    write(tmpRows[0], tmpRows[1], tmpRows[2], tmpRows[3]);
    va_end(args);
}

void Display::data(uint8_t value)
{
    DP.write(dcPin, true);
    _delay_us(5);

    send(value >> 4);
    send(value >> 0);
    _delay_us(1500);
}

void Display::command(uint8_t cmd, uint8_t val)
{
    uint8_t value = cmd | val;

    DP.write(dcPin, false);
    _delay_us(5);

    send(value >> 4);
    send(value >> 0);
    _delay_us(1500);
}

void Display::send(uint8_t value)
{
    DP.write(enPin, false);
    _delay_us(5);

    for (uint8_t i=0; i<4; i++)
        DP.write(dataPin[i], (value >> i) & 0x01);
    _delay_us(5);

    DP.write(enPin, true);
    _delay_us(5);

    DP.write(enPin, false);
    _delay_us(75);
}

#endif
