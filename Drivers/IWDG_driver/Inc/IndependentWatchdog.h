/*
 * IndependentWatchdog.h
 *
 *  Created on: Jun. 22, 2023
 *      Author: pratyushmakkar
 */

#ifndef SRC_DRIVERS_INDEPENDENTWATCHDOG_H_
#define SRC_DRIVERS_INDEPENDENTWATCHDOG_H_


#include <stdint.h>
#include "main.h"

class IWDG_t {
	private:
		uint8_t iwdg_prescaler;
		uint16_t iwdg_reload;

	public:
		IWDG_t(IWDG_HandleTypeDef* watchdog);
		IWDG_t(uint8_t iwdg_prescaler, uint16_t iwdg_reload);
		HAL_StatusTypeDef InitWatchdog(IWDG_HandleTypeDef* watchdog);
		static HAL_StatusTypeDef RefreshWatchdog(IWDG_HandleTypeDef* watchdog);
};


#endif /* SRC_DRIVERS_INDEPENDENTWATCHDOG_H_ */

