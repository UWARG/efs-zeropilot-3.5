/**
 * @file TelemetryTask.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 * - Don't touch TaskTrampoline. Took a while to get right and is confusing.
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 * @author Derek Tang: implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TELEMETRYTASK_H
#define TELEMETRYTASK_H

#include <functional>

#ifndef TESTING
#include "FreeRTOS.h"
#include "task.h"
#endif
#include "TelemetryManager.hpp"

class TelemetryTask {
    // Simple alias for a lambda function that takes a TelemetryManager reference as an argument.
    using Callback = std::function<void(TelemetryManager&)>;

   private:
    // TM instance reference to be used in the callback.
    TelemetryManager& tm;
    // Callback function to be called when the timer interrupt is triggered.
    Callback cbLambda;
    // Handle to the task. Or should this be something else? Since we are doing a timer interrupt?
    TaskHandle_t xHandle;

    /**
     * @brief This is essentially a compatibility/wrapper function that allows us to use a lambda
     * function which has a class instance as an argument as a callback for the timer interrupt.
     * This allows us to access TM within the lambda function.
     *
     * @param pvParameters - The TelemetryTask object.
     */
    static void TaskTrampoline(void* pvParameters);

   public:
    /**
     * @brief Construct a new Timer TelemetryTask object which will call the given lambda function
     * at the given interval. This will allow easier scheduling of data transfer or any TM tasks
     * which need to happen at a regular interval with FreeRTOS.
     *
     * @param taskName - The name of the task.
     * @param stackSize - The size of the stack for the task.
     * @param uxPriority - The priority of the task.
     * @param tm - The TelemetryManager object.
     * @param cbLambda - The callback function to be called when the timer interrupt is triggered.
     */
    TelemetryTask(const char* taskName, int stackSize, UBaseType_t uxPriority, TelemetryManager& tm,
                  Callback cbLambda);

    /**
     * @brief Destroy the Timer Interrupt object.
     *
     */
    ~TelemetryTask();
};

#endif
