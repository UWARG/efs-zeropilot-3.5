#include "FreeRTOS_mock.hpp"

freertos::FreeRTOSMock FreeRTOSMockObj;

QueueHandle_t xQueueCreateMutex(const uint8_t ucQueueType) {
    return FreeRTOSMockObj.xQueueCreateMutex(ucQueueType);
}

BaseType_t xQueueSemaphoreTake(QueueHandle_t xQueue, TickType_t xTicksToWait) {
    return FreeRTOSMockObj.xQueueSemaphoreTake(xQueue, xTicksToWait);
}

BaseType_t xQueueGenericSend(QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition) {
    return FreeRTOSMockObj.xQueueGenericSend(xQueue, pvItemToQueue, xTicksToWait, xCopyPosition);
}
