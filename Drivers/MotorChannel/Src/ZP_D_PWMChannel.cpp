#include "ZP_D_PWMChannel.hpp"

PWMChannel::PWMChannel(uint16_t pin_num, GPIO_TypeDef* port, TIM_HandleTypeDef* timer,
                        uint16_t timer_channel, uint32_t clock_frequency) :    pin_num_(pin_num),
                                                    port_(port),
                                                    timer_(timer),
                                                    timer_channel_(timer_channel),clock_frequency_(clock_frequency) {}

void PWMChannel::setup(void) {
	/*Sets up the PWM, prescaler, duty cycle ranges,
	 * and starts the timer*/
	period_ticks_ = timer_->Init.Period;
	min_signal = period_ticks_ * 0.05; // sets counts for 5% duty cycle
	max_signal = period_ticks_ * 0.10; // sets counts for 10% duty cycle

	//Calculate new prescaler
	uint16_t prescaler_ = (clock_frequency_ / DESIRED_FREQUENCY
			/ period_ticks_) - 1;

	__HAL_TIM_SET_PRESCALER(timer_, prescaler_);
	HAL_TIM_PWM_Start(timer_, timer_channel_);


	isSetup = true;
}


void PWMChannel::set(uint8_t percent) {
	/*Sets the duty cycle as a percent between 5 and 10%.
	 *
	 * Usage:
	 * 0% corresponds to a duty cycle of 5%
	 * 100% corresponds to a duty cycle of 10%
	 * 50% corresponds to a duty cycle of 7.5%*/
    if (percent > 100 || !isSetup) {
        return;
    }
    uint32_t ticks = (percent * (max_signal - min_signal)) / 100 + min_signal;

    __HAL_TIM_SET_COMPARE(timer_, timer_channel_, ticks);
}

