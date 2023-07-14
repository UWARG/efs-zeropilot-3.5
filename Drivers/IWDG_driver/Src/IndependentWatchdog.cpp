#include "IndependentWatchdog.h"
#include "main.h"

/**
 * @brief Borrow the configuration of an already existing watchdog and set it
 * to the prescaler and Reload values.
 *
 */
IWDG_t::IWDG_t(IWDG_HandleTypeDef *watchdog) {
	this->watchdog = watchdog;
	IWDG_InitTypeDef initDef = watchdog->Init;
	this->iwdg_prescaler = initDef.Prescaler;
	this->iwdg_reload = initDef.Reload;
}


/**
 * @brief Reset the reload and counter values inside a watchdog
 * using the member variables of IWDG_t
 *
 */
bool IWDG_t::SetWatchdogParameters(uint8_t prescaler, uint16_t reload) {
	this->iwdg_prescaler = prescaler;
	this->iwdg_reload = reload;

	if (!IS_IWDG_PRESCALER(prescaler) || !IS_IWDG_RELOAD(reload)) return false;
	watchdog->Init.Reload = reload;
	watchdog->Init.Prescaler = prescaler;
	HAL_StatusTypeDef refresh = HAL_IWDG_Init(watchdog);
	return (refresh==HAL_OK);
}


/**
 * @brief Refreshes the watchdog that is a member variable of the class
 *
 */

bool IWDG_t::RefreshWatchdog() {
	if (!this->watchdog) {
		return false;
	}
	return (HAL_IWDG_Refresh(watchdog) == HAL_OK);
}

/**
 * @brief Setter function for the watchdog
 *
 */
void IWDG_t::SetIWDGWatchdog(IWDG_HandleTypeDef* watchdog) {
	this->watchdog = watchdog;
}

/**
 * @brief Getter function for the watchdog
 *
 */
IWDG_HandleTypeDef* IWDG_t::GetIWDGWatchdog(IWDG_HandleTypeDef* watchdog) {
	return this->watchdog;
}