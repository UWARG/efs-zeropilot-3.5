#ifndef ZP_D_PWM_CHANNEL_HPP_
#define ZP_D_PWM_CHANNEL_HPP_

#include "main.h"

#include "ZP_D_MotorChannel.hpp"

class PWMChannel : public MotorChannel {
    public:
        PWMChannel(TIM_HandleTypeDef* timer,
					uint16_t timer_channel);

        void set(uint8_t percent);
    private:
    	//set to minimum and max amount of counts in a duty cycle
        uint32_t MIN_SIGNAL;
        uint32_t MAX_SIGNAL;
        uint32_t PERIOD_TICKS_;  
        TIM_HandleTypeDef *TIMER_;     //handle to the timer
        const uint16_t TIMER_CHANNEL_;   //channel of the timer

        const uint32_t CLOCK_FREQUENCY_ = SystemCoreClock;    //system clock frequency
        const uint16_t DESIRED_FREQUENCY = 50; //PWM frequency in hz
};

#endif // ZP_D_PWM_CHANNEL_HPP_
