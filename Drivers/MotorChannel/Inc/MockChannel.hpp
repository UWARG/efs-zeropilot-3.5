#ifndef ZP_D_MOCK_CHANNEL_HPP_
#define ZP_D_MOCK_CHANNEL_HPP_

#include "main.h"

#include "ZP_D_MotorChannel.hpp"

class MockChannel : public MotorChannel {
    public:
        MockChannel();

        void set(uint8_t percent);

        uint8_t percent_;
};

#endif // ZP_D_PWM_CHANNEL_HPP_
