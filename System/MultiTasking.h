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

#ifndef MULTI_TASKING_H
#define MULTI_TASKING_H

#include "./Time.h"

#define MAX_TASK 8

struct taskInfo
{
    void (*taskLoop)(void);
    uint32_t lastRun;
    uint32_t timeInterval;
};

class MultiTasking
{
    public:
        MultiTasking();

        bool addTask(void (*pLoop)(void), uint32_t tInterval = 0x00000000);

        void runTask();

    private:
        taskInfo taskList[MAX_TASK];
};

MultiTasking MT;

/****************************************************************************************/

MultiTasking::MultiTasking()
{
    for (uint8_t i=0; i<MAX_TASK; i++)
    {
        taskList[i].taskLoop = 0x0000;
        taskList[i].lastRun = 0x00000000;
        taskList[i].timeInterval = 0x00000000;
    }
}

bool MultiTasking::addTask(void (*pLoop)(void), uint32_t tInterval)
{
    for (uint8_t i=0; i<MAX_TASK; i++)
    {
        if (taskList[i].taskLoop == 0x0000)
        {
            taskList[i].taskLoop = pLoop;
            taskList[i].lastRun = 0x00000000;
            taskList[i].timeInterval = tInterval;
            return true;
        }
    }
    return false;
}

void MultiTasking::runTask()
{
    for (uint8_t i=0; i<MAX_TASK && taskList[i].taskLoop!=0x0000; i++)
    {
        if (labs(ST.microsec()-taskList[i].lastRun) >= taskList[i].timeInterval)
        {
            taskList[i].taskLoop();
            taskList[i].lastRun = ST.microsec();
        }
    }
}

/****************************************************************************************/

bool init();

int main()
{
    if (!init())
        return 0;

    while(true)
    {
        MT.runTask();

        ST.watchdog_reset();
    }

    return 0;
}

#endif
