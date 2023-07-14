#ifndef SRC_DRIVERS_INDEPENDENTWATCHDOG_H_
#define SRC_DRIVERS_INDEPENDENTWATCHDOG_H_

#include <stdint.h>
#include "main.h"

class IWDG_t {
	private:
		IWDG_HandleTypeDef* watchdog;
		uint8_t iwdg_prescaler;
		uint16_t iwdg_reload;

	public:
		IWDG_t(IWDG_HandleTypeDef* watchdog);
		void SetIWDGWatchdog(IWDG_HandleTypeDef* watchdog);
		IWDG_HandleTypeDef* GetIWDGWatchdog(IWDG_HandleTypeDef* watchdog);
		bool SetWatchdogParameters(uint8_t prescaler, uint16_t reload);
		bool RefreshWatchdog();
};


#endif /* SRC_DRIVERS_INDEPENDENTWATCHDOG_H_ */
