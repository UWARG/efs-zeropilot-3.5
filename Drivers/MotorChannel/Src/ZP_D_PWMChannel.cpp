#include "ZP_D_PWMChannel.hpp"

PWMChannel::PWMChannel(TIM_HandleTypeDef* timer,
                        uint16_t timer_channel) :
                        TIMER_(timer),
                        TIMER_CHANNEL_(timer_channel)
{
	/*Sets up the PWM, pre-scaler, duty cycle ranges,
	 * and starts the timer*/
	PERIOD_TICKS_ = TIMER_->Init.Period;
	MIN_SIGNAL = PERIOD_TICKS_ * 0.05; // sets counts for 5% duty cycle
	MAX_SIGNAL = PERIOD_TICKS_ * 0.10; // sets counts for 10% duty cycle

	//Calculate new pre-scaler
	uint16_t prescaler_ = (CLOCK_FREQUENCY_ / DESIRED_FREQUENCY
			/ PERIOD_TICKS_) - 1;

	__HAL_TIM_SET_PRESCALER(TIMER_, prescaler_);
	HAL_TIM_PWM_Start(TIMER_, TIMER_CHANNEL_);


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
    uint32_t ticks = (percent * (MAX_SIGNAL - MIN_SIGNAL)) / 100 + MIN_SIGNAL;

    __HAL_TIM_SET_COMPARE(TIMER_, TIMER_CHANNEL_, ticks);
}

