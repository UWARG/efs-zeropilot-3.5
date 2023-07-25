
#ifndef INDEPENDENT_WATCHDOG_H
#define INDEPENDENT_WATCHDOG_H

#include <stdint.h>
#include "watchdog.h"
#include "iwdg.h"

class IndependentWatchdog : public Watchdog {
	private:
		IWDG_HandleTypeDef* watchdog;

	public:
		IndependentWatchdog(IWDG_HandleTypeDef* watchdog);
		bool refreshWatchdog() override ;
};


#endif /* SRC_DRIVERS_INDEPENDENTWATCHDOG_H_ */

