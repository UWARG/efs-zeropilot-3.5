// #include "ZP_D_PWMChannel.hpp"

// PWMChannel::PWMChannel(TIM_HandleTypeDef* timer,
//                         uint16_t timerChannel) :
// 						timer(timer),
//                         TIMER_CHANNEL(timerChannel),
// 						MIN_SIGNAL(timer->Init.Period * 0.05),  // sets counts for 5% duty cycle
//                         MAX_SIGNAL(timer->Init.Period * 0.10)  // sets counts for 10% duty cycle
// {
// 	// Calculate new pre-scaler
// 	uint16_t prescaler = (SystemCoreClock / DESIRED_FREQUENCY
// 			/ timer->Init.Period) - 1;
// 	__HAL_TIM_SET_PRESCALER(timer, prescaler);
// 	HAL_TIM_PWM_Start(timer, TIMER_CHANNEL);
// }

// void PWMChannel::set(uint8_t percent) {
// 	/* Sets the duty cycle as a percent between 5 and 10%.
// 	 *
// 	 * Usage:
// 	 * 0% corresponds to a duty cycle of 5%
// 	 * 100% corresponds to a duty cycle of 10%
// 	 * 50% corresponds to a duty cycle of 7.5%*/
//     if (percent > 100) {
//         return;
//     }
//     uint32_t ticks = (percent * (MAX_SIGNAL - MIN_SIGNAL)) / 100 + MIN_SIGNAL;

//     __HAL_TIM_SET_COMPARE(timer, TIMER_CHANNEL, ticks);
// }

