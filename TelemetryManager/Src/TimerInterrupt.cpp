
#include "TimerInterrupt.hpp"

TimerInterrupt::TimerInterrupt(const char *taskName, int stackSize, UBaseType_t uxPriority,
                               TickType_t intervalMs, TelemetryManager &tm, Callback cbLambda)
    : tm(tm), cbLambda(cbLambda), xHandle(nullptr) {
        
    // Below is just an example of creating a task with the TaskTrampoline. This is not the actual
    // implementation. Delete it and implement the actual timer interrupt.
    xTaskCreate(&TimerInterrupt::TaskTrampoline, taskName, stackSize, this, uxPriority, &xHandle);

    // START: Implement the actual timer interrupt and call the callback function every intervalMs

    // END: Implement the actual timer interrupt and call the callback function every intervalMs
}

TimerInterrupt::~TimerInterrupt() {
    if (eTaskGetState(xHandle) != eDeleted) {
        vTaskDelete(xHandle);
    }
}
