#ifndef ZP_D_PWM_CHANNEL_HPP_
#define ZP_D_PWM_CHANNEL_HPP_

#include "main.h"

#include "ZP_D_MotorChannel.hpp"

class PWMChannel : public MotorChannel {
    public:
        PWMChannel(uint16_t pin_num, GPIO_TypeDef* port, TIM_HandleTypeDef* timer,
					uint16_t timer_channel,uint32_t clock_frequency);

        void setup(void);
        void set(uint8_t percent);
    private:
        uint16_t pin_num_;
        GPIO_TypeDef* port_;
        TIM_HandleTypeDef *timer_;     //handle to the timer
        uint16_t timer_channel_;       //channel of the timer
        uint32_t clock_frequency_;     //system clock frequency
        uint32_t period_ticks_;

        //set to min and max amount of counts in a duty cycle
        uint32_t min_signal;
        uint32_t max_signal;

        //values in microseconds(us)
        const uint16_t DESIRED_FREQUENCY = 50;


};

#endif // ZP_D_PWM_CHANNEL_HPP_
