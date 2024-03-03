
#include "TimerInterrupt.hpp"
void TimerInterrupt::registerTimerInterrupt(int timeIntervalMs, void (*function)())
{
    // execute the function every timeIntervalMs using a timer interrupt on the STM32
    function();
}