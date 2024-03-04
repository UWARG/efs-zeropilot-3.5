
#include "TimerInterrupt.hpp"

TimerInterrupt::TimerInterrupt()
{
    // Constructor
}

TimerInterrupt::~TimerInterrupt()
{
    // Destructor
}


void TimerInterrupt::registerTimerInterrupt(int timeIntervalMs, void (*function)())
{
    // execute the function every timeIntervalMs using a timer interrupt on the STM32
    //likely some STM32 specific code here or FreeRTOS code. Not 100% sure what the best way to do this is.
    function();
}