#ifndef FREE_RTOS_MOCK
#define FREE_RTOS_MOCK

#include "FreeRTOS.h"
#include "semphr.h"

#include "gmock/gmock.h"

namespace freertos {

class FreeRTOSInterface
{
    public:
        virtual ~FreeRTOSInterface() {}

        virtual QueueHandle_t xQueueCreateMutex(const uint8_t ucQueueType) = 0;

        virtual BaseType_t xQueueSemaphoreTake(QueueHandle_t xQueue, TickType_t xTicksToWait) = 0;

        virtual BaseType_t xQueueGenericSend(QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition) = 0;
};

class FreeRTOSMock : public FreeRTOSInterface
{
    public:
        MOCK_METHOD(QueueHandle_t, xQueueCreateMutex, (const uint8_t ucQueueType), (override));
        MOCK_METHOD(BaseType_t, xQueueSemaphoreTake, (QueueHandle_t xQueue, TickType_t xTicksToWait), (override));
        MOCK_METHOD(BaseType_t, xQueueGenericSend, (QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition), (override));
};

} // namespace freertos

#endif // FREE_RTOS_MOCK
