#include "../Inc/IndependentWatchdog.h";


/**
 * @brief Borrow the configuration of an already existing watchdog and set it 
 * to the prescaler and Reload values.
 * 
 */
IWDG_t::IWDG_t(IWDG_HandleTypeDef *watchdog) {
	IWDG_InitTypeDef *initDef = watchdog->Init;
	this->iwdg_prescaler = initDef->Prescaler;
	this->iwdg_reload = initDef->Reload;
}

/**
 * @brief Initalize the driver using custom IWDG_Prescaler and realod values. 
 * 
 */
IWDG_t::IWDG_t(uint8_t iwdg_prescaler, uint32_t iwdg_reload) {
  // assert(IS_IWDG_PRESCALER(iwdg_prescaler)) Should this be added since it is needed when reinitialzing the watchdog
  // assert(IS_IWDG_RELOAD(iwdg_reload))  
	this->iwdg_prescaler = iwdg_prescaler;
	this->iwdg_reload = iwdg_reload;
}

/**
 * @brief Reset the reload and counter values inside a watchdog
 * using the member variables of IWDG_t
 * 
 */
HAL_StatusTypeDef IWDG_t::InitWatchdog(IWDG_HandleTypeDef *watchdog) {
	watchdog->Init.Reload = this->iwdg_reload;
	watchdog->Init.Prescaler = this->iwdg_prescaler;
	HAL_StatusTypeDef refresh = HAL_IWDG_Init(watchdog);
	return refresh;
}

/**
 * @brief Refresh the watchdog in the parameter. It will reset the downcounter
 * to the value within the reload register.
 * 
 */
HAL_StatusTypeDef IWDG_t::RefreshWatchdog(IWDG_HandleTypeDef *watchdog) {
	return HAL_IWDG_Refresh(watchdog);
}