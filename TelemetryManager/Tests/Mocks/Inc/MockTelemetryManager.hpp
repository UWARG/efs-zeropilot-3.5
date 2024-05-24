#include <gmock/gmock.h>
#include "TelemetryManager.hpp"

class MockTelemetryManager : public MockTelemetryManager
{
    public:
    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, update, ());
};