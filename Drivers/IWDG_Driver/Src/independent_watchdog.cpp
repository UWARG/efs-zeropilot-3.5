#include "independent_watchdog.h"

/**
 * @brief Borrow the configuration of an already existing watchdog and set it
 * to the prescaler and Reload values.
 *
 */
IndependentWatchdog::IndependentWatchdog(IWDG_HandleTypeDef *watchdog) {
	this->watchdog = watchdog;
}


/**
 * @brief Refreshes the watchdog that is a member variable of the class
 * @returns true on success, false on failure
 */

bool IndependentWatchdog::refreshWatchdog() {
	if (this->watchdog == nullptr) {
		return false;
	}
	return (HAL_IWDG_Refresh(watchdog) == HAL_OK);
}
