#ifndef ZP_D_MOCK_CHANNEL_HPP_
#define ZP_D_MOCK_CHANNEL_HPP_

#include "main.h"
#include "ZP_D_MotorChannel.hpp"
#include <gmock/gmock.h>

class MockChannel : public MotorChannel {
    public:
        MockChannel();

        MOCK_METHOD(void, set, (uint8_t));
};

#endif // ZP_D_PWM_CHANNEL_HPP_
