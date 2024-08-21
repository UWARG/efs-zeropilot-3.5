
#ifndef INDEPENDENT_WATCHDOG_H
#define INDEPENDENT_WATCHDOG_H

#include "watchdog.h"
#include "iwdg.h"

/* 	Reminder for the correct usage of this driver:
	Don't initialize more than one instance of iwdg.
	There is only one iwdg in more STM, so it won't work.

	Also, don't call MX_IWDG_Init() in main!
 */
class IndependentWatchdog : public Watchdog {
	private:
		IWDG_HandleTypeDef* watchdog_;
		uint32_t prescaler_;
		uint32_t window_;
		uint32_t reload_;

		bool counterCalculation(uint32_t timeout, uint32_t &prescaler, uint32_t &counter);
		bool windowCalculation(uint32_t timeout, uint32_t prescaler, uint32_t &window);
	public:
		IndependentWatchdog();

		/* Time out in ms */
		IndependentWatchdog(uint32_t timeout);

		/* This is very similiar to a Window Watchdog */
		IndependentWatchdog(uint32_t counter_timeout, uint32_t window_timeout);

		/* Feed the dog and makes it happy */
		bool refreshWatchdog() override ;
};



#endif /* SRC_DRIVERS_INDEPENDENTWATCHDOG_H_ */

