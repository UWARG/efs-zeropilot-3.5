#include "independent_watchdog.h"

#define MAX_PR 			6
#define RELOAD_LENGTH 	12
#define WINDOW_LENGTH 	12
#define LSI_SPEED 		32000
#define PR_OFFSET		2
#define MAX_TIMEOUT_MS		(((1 << RELOAD_LENGTH)*4*(1 << MAX_PR)*1000) / LSI_SPEED) // should be about 32768

/*	A general watchdog, the window feature is not used
	If it fails, no one could notice it, which is not really desired
	@param: timeout - timeout in ms
*/
IndependentWatchdog::IndependentWatchdog(uint32_t timeout){
	if (timeout >= MAX_TIMEOUT_MS) {
		return;
	}

	if (counterCalculation(timeout, this->prescaler_, this->reload_) == false){
		// Error_Handler();
		return;
	}
	this->window_ = this->reload_;
	this->watchdog_ = &hiwdg;

	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = this->prescaler_;
	hiwdg.Init.Window = this->window_;
	hiwdg.Init.Reload = this->reload_;
	if (HAL_IWDG_Init(this->watchdog_) != HAL_OK)
	{
		Error_Handler();
	}
}

/*	I don't know why STM added this feature, but you turn IWDG into a WWDG which is confusing
	Now the watchdog triggers the reset when refresh comes too early or when downcounter reaches zero

					|------- Refresh triggers reset -------- | ------- Refresh Allowed -------- |--- reset ---|
					^                                        ^									^
	counter value: reload                                  window                  				0

	@param: counter_timeout - in ms
	@param:	window_timeout  - in ms
*/
IndependentWatchdog::IndependentWatchdog(uint32_t counter_timeout, uint32_t window_timeout){
	if (counter_timeout >= MAX_TIMEOUT_MS || window_timeout >= MAX_TIMEOUT_MS) {
		return;
	}

	if (counterCalculation(counter_timeout, this->prescaler_, this->reload_) == false){
		// Error_Handler();
		return;
	}
	if (windowCalculation(window_timeout, this->prescaler_, this->window_) == false){
		// Error_Handler();
		return;
	}
	this->watchdog_ = &hiwdg;

	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = this->prescaler_;
	hiwdg.Init.Window = this->window_;
	hiwdg.Init.Reload = this->reload_;
	if (HAL_IWDG_Init(this->watchdog_) != HAL_OK)
	{
		Error_Handler();
	}
}



/**
 * @brief Refreshes the watchdog that is a member variable of the class
 * @returns true on success, false on failure
 */
bool IndependentWatchdog::refreshWatchdog() {
	if (this->watchdog_ == nullptr) {
		return false;
	}
	return (HAL_IWDG_Refresh(this->watchdog_) == HAL_OK);
}


/* 	Helper function
	Updates the prescaler and reload depending on the timeout
 */
bool IndependentWatchdog::counterCalculation(uint32_t timeout, uint32_t &prescaler, uint32_t &counter){
	int prescalerPR = 0; // start prescaler = 4
	const uint32_t MAX_COUNTER = (1 << RELOAD_LENGTH);

	for (; prescalerPR <= MAX_PR; prescalerPR++){
		prescaler = prescalerPR;
		uint32_t prescalerValue = 1 << (prescalerPR + PR_OFFSET);
		counter = ((timeout * LSI_SPEED) / (prescalerValue * 1000)) - 1;
		if (counter <=  MAX_COUNTER) {
			return true;
		}
	}

	return false;
}

/* 	Helper function
	Only update the window depending on the timeout
*/
bool IndependentWatchdog::windowCalculation(uint32_t timeout, uint32_t prescaler, uint32_t &window){
	const uint32_t MAX_WINDOW = (1 << WINDOW_LENGTH);

	uint32_t prescalerValue = 1 << (prescaler + PR_OFFSET);
	window = ((timeout * LSI_SPEED) / (prescalerValue * 1000)) - 1;
	if (window <=  MAX_WINDOW) {
		return true;
	}

	return false;
}
