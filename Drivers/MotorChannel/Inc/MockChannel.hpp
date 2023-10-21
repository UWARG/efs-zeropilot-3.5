#ifndef ZP_D_MOCK_CHANNEL_HPP_
#define ZP_D_MOCK_CHANNEL_HPP_

#include "main.h"

#include "ZP_D_MotorChannel.hpp"

namespace mock {
extern uint8_t *motorOutputs;
}

class MockChannel : public MotorChannel {
    public:
        MockChannel(uint8_t channel);

        void set(uint8_t percent);

        uint8_t channel_;
};

#endif // ZP_D_PWM_CHANNEL_HPP_
