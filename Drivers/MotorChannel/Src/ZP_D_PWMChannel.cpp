#include "ZP_D_PWMChannel.hpp"

PWMChannel::PWMChannel(TIM_HandleTypeDef* timer,
                        uint16_t timerChannel) :
                        TIMER(timer),
                        TIMER_CHANNEL(timerChannel)
{
	/* Sets up the PWM, pre-scaler, duty cycle ranges,
	 * and starts the timer*/
	PERIOD_TICKS = TIMER->Init.Period;
	minSignal= PERIOD_TICKS * 0.05; // sets counts for 5% duty cycle
	maxSignal = PERIOD_TICKS * 0.10; // sets counts for 10% duty cycle

	// Calculate new pre-scaler
	uint16_t prescaler = (CLOCK_FREQUENCY / DESIRED_FREQUENCY
			/ PERIOD_TICKS) - 1;

	__HAL_TIM_SET_PRESCALER(TIMER, prescaler);
	HAL_TIM_PWM_Start(TIMER, TIMER_CHANNEL);


	isSetup = true;
}

void PWMChannel::set(uint8_t percent) {
	/* Sets the duty cycle as a percent between 5 and 10%.
	 *
	 * Usage:
	 * 0% corresponds to a duty cycle of 5%
	 * 100% corresponds to a duty cycle of 10%
	 * 50% corresponds to a duty cycle of 7.5%*/
    if (percent > 100 || !isSetup) {
        return;
    }
    uint32_t ticks = (percent * (maxSignal - minSignal)) / 100 + minSignal;

    __HAL_TIM_SET_COMPARE(TIMER, TIMER_CHANNEL, ticks);
}

