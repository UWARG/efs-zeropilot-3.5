#ifndef ZP_D_PWM_CHANNEL_HPP_
#define ZP_D_PWM_CHANNEL_HPP_

#include "main.h"

#include "ZP_D_MotorChannel.hpp"

class PWMChannel : public MotorChannel {
    public:
        PWMChannel(TIM_HandleTypeDef* timer,
					uint16_t timerChannel);

        void set(uint8_t percent);
};

#endif // ZP_D_PWM_CHANNEL_HPP_
