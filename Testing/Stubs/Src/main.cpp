#include "main.h"
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel) {return HAL_OK;}
TIM_HandleTypeDef htim1 = {.Init = {.Period = 64000}};
uint32_t SystemCoreClock = 8000000;