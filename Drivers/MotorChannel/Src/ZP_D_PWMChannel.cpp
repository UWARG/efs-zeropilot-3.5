#include "ZP_D_PWMChannel.hpp"

PWMChannel::PWMChannel(uint16_t pin_num, GPIO_TypeDef* port, TIM_HandleTypeDef* timer,
                        uint16_t timer_channel, uint32_t clock_frequency) :    pin_num_(pin_num),
                                                    port_(port),
                                                    timer_(timer),
                                                    timer_channel_(timer_channel),clock_frequency_(clock_frequency) {}

void PWMChannel::setup(void) {
	period_ticks_ = timer_->Init.Period;

	uint16_t prescaler_ = (clock_frequency_ / DESIRED_FREQUENCY / PWM_PERIOD) - 1;

	//set timer settings and start pwm
	__HAL_TIM_SET_PRESCALER(timer_, prescaler_);
	__HAL_TIM_SET_COUNTER(timer_, PWM_PERIOD);
	HAL_TIM_PWM_Start(timer_, timer_channel_);


	isSetup = true;
}


void PWMChannel::set(uint8_t percent) {
    if (percent > 100 || !isSetup) {
        return;
    }

    uint32_t us = (percent * (MAX_SIGNAL - MIN_SIGNAL)) / 100 + MIN_SIGNAL;
    uint32_t ticks = static_cast<uint32_t>((static_cast<float>(us) / static_cast<float>(PWM_PERIOD))
                        * static_cast<float>(period_ticks_));

    __HAL_TIM_SET_COMPARE(timer_, timer_channel_, ticks);
}

