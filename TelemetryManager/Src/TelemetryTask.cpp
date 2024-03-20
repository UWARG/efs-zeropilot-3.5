
#include "TelemetryTask.hpp"

TelemetryTask::TelemetryTask(const char *taskName, int stackSize, UBaseType_t uxPriority, TelemetryManager &tm, Callback cbLambda)
    : tm(tm), cbLambda(cbLambda), xHandle(nullptr) {
    xTaskCreate(&TelemetryTask::TaskTrampoline, taskName, stackSize, this, uxPriority, &xHandle);
}

void TelemetryTask::TaskTrampoline(void *pvParameters) {
        auto* context = static_cast<TelemetryTask*>(pvParameters);
        context->cbLambda(context->tm);
    }

TelemetryTask::~TelemetryTask() {
    if (eTaskGetState(xHandle) != eDeleted) {
        vTaskDelete(xHandle);
    }
}
