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

        //values in microseconds(us)
        static constexpr uint32_t PWM_PERIOD = 20000;
        static constexpr uint32_t MIN_SIGNAL = 950; //standard for 50hz pwm signal, with a 100 Hz margin added to ensure we can reach the extreme values.
        static constexpr uint32_t MAX_SIGNAL = 2050;
        const uint16_t DESIRED_FREQUENCY = 50;


};

#endif // ZP_D_PWM_CHANNEL_HPP_
