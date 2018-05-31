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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "./Time.h"

#define MAX_STATE 8

struct stateInfo
{
    void (*stateTask)(void);
    uint32_t startTime;
};

class StateMachine
{
    public:
        StateMachine();

        bool addState(uint8_t stateName, void (*pTask)(void));

        uint32_t timeState();
        bool changeState(uint8_t stateName);

        void runState();

    private:
        uint8_t currState;
        bool changedStated;
        stateInfo stateList[MAX_STATE];
};

/****************************************************************************************/

StateMachine::StateMachine()
{
    currState = 0;
    changedStated = true;
    for (uint8_t i=0; i<MAX_STATE; i++)
    {
        stateList[i].stateTask = 0x0000;
        stateList[i].startTime = 0x00000000;
    }
}

bool StateMachine::addState(uint8_t stateName, void (*pTask)(void))
{
    if (stateName >= MAX_STATE)
        return false;
    if (stateList[stateName].stateTask == 0x0000)
    {
        stateList[stateName].stateTask = pTask;
        stateList[stateName].startTime = 0x00000000;
        return true;
    }
    return false;
}

uint32_t StateMachine::timeState()
{
    return labs(ST.millisec() - stateList[currState].startTime);
}

bool StateMachine::changeState(uint8_t stateName)
{
    if (stateList[stateName].stateTask != 0x0000)
    {
        changedStated = true;
        currState = stateName;
        return true;
    }
    return false;
}

void StateMachine::runState()
{
    if (stateList[currState].stateTask != 0x0000)
    {
        if (changedStated == true)
        {
            changedStated = false;
            stateList[currState].startTime = ST.millisec();
        }
        stateList[currState].stateTask();
    }
}

#endif
