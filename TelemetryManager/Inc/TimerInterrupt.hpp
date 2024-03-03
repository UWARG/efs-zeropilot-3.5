//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#ifndef TIMERINTURUPT_H
#define TIMERINTURUPT_H

class TimerInterrupt
{
public:
    void registerTimerInterrupt(int timeIntervalMs, void (*function)());
};

#endif
