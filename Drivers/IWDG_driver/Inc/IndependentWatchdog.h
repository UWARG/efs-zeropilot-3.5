/*
 * IndependentWatchdog.h
 *
 *  Created on: Jun. 22, 2023
 *      Author: pratyushmakkar
 */

#ifndef SRC_DRIVERS_INDEPENDENTWATCHDOG_H_
#define SRC_DRIVERS_INDEPENDENTWATCHDOG_H_

#include <stdint.h>
#include "../../boardfiles/discoveryl562qe/Drivers/STM32L5xx_HAL_Driver/Inc/stm32l5xx_hal_iwdg.h"

class IWDG_t {
	private:
		uint8_t iwdg_prescaler;
		uint32_t iwdg_reload;

	public:
		IWDG_t(IWDG_HandleTypeDef* watchdog);
		IWDG_t(uint8_t iwdg_prescaler, uint32_t iwdg_reload);
		HAL_StatusTypeDef InitWatchdog(IWDG_HandleTypeDef* watchdog, IWDG_t* cfg);
		static HAL_StatusTypeDef RefreshWatchdog(IWDG_HandleTypeDef* watchdog);
};


#endif /* SRC_DRIVERS_INDEPENDENTWATCHDOG_H_ */
