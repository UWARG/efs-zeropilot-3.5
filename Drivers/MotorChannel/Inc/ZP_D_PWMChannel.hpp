#ifndef ZP_D_PWM_CHANNEL_HPP_
#define ZP_D_PWM_CHANNEL_HPP_

#include "main.h"
#include "ZP_D_MotorChannel.hpp"

class PWMChannel : public MotorChannel {
    public:
        PWMChannel(TIM_HandleTypeDef* timer,
					uint16_t timerChannel);

        void set(uint8_t percent);
    private:
        TIM_HandleTypeDef *timer;
        const uint16_t TIMER_CHANNEL;

    	// set to minimum and max amount of counts in a duty cycle
        const uint32_t MIN_SIGNAL;
        const uint32_t MAX_SIGNAL;
        const uint16_t DESIRED_FREQUENCY = 50; // PWM frequency in hz
};

#endif // ZP_D_PWM_CHANNEL_HPP_
