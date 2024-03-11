/**
 * @file TimerInterrupt.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 * - Don't touch TaskTrampoline. Took a while to get right and is confusing.
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 * @author Rahul Ramkumar: implementation
 * @author Derek Tang: implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#include <functional>

#include "FreeRTOS.h"
#include "TM.hpp"
#include "task.h"
#ifndef TIMERINTURUPT_H
#define TIMERINTURUPT_H

class TimerInterrupt {
    // Simple alias for a lambda function that takes a TelemetryManager reference as an argument.
    using Callback = std::function<void(TelemetryManager&)>;

   public:

    //TM instance reference to be used in the callback.
    TelemetryManager& tm;
    //Callback function to be called when the timer interrupt is triggered.
    Callback cbLambda;
    //Handle to the task. Or should this be something else? Since we are doing a timer interrupt?
    TaskHandle_t xHandle;

    /**
     * @brief Construct a new Timer Interrupt object which will call the given lambda function
     * at the given interval. This is an abstraction of the software timer interrupt functionality
     * in FreeRTOS. This will allow easier scheduling of data transfer or any TM tasks which need
     * to happen at a regular interval.
     *
     * @param taskName - The name of the task.
     * @param stackSize - The size of the stack for the task.
     * @param uxPriority - The priority of the task.
     * @param intervalMs - The time interval in milliseconds at which the interrupt should be
     * triggered.
     * @param tm - The TelemetryManager object.
     * @param cbLambda - The callback function to be called when the timer interrupt is triggered.
     */
    TimerInterrupt(const char* taskName, int stackSize, UBaseType_t uxPriority,
                   TickType_t intervalMs, TelemetryManager& tm, Callback cbLambda);

    /**
     * @brief Destroy the Timer Interrupt object.
     *
     */
    ~TimerInterrupt();

    /**
     * @brief This is essentially a compatibility/wrapper function that allows us to use a lambda
     * function which has a class instance as an argument as a callback for the timer interrupt. 
     * This allows us to access TM within the lambda function. 
     *
     * @param pvParameters - The TimerInterrupt object.
     */
    static void TaskTrampoline(void* pvParameters) {
        auto* context = static_cast<TimerInterrupt*>(pvParameters);
        context->cbLambda(context->tm);
    }
};

#endif
