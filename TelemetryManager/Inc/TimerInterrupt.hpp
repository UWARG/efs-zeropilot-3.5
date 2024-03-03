//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#ifndef TIMERINTURUPT_H
#define TIMERINTURUPT_H


class TimerInterrupt
{
public:
/**
 * @brief This function allows us to "register" a timer interrupt when the program starts. 
 * The function will be called every time the timer interrupt is triggered. This is 
 * simply an abstraction of the timer interrupt functionality on the STM32 and will allow
 * easier scheduling of data transfer or any TM tasks which need to happen at a regular 
 * interval.
 * 
 * @param timeIntervalMs The time interval in milliseconds at which the interrupt should
 * be triggered and function should be called.
 * @param function The function to be called when the timer interrupt is triggered.
 */
    void registerTimerInterrupt(int timeIntervalMs, void (*function)());
};

#endif
