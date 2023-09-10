#ifndef MAIN_H
#define MAIN_H

#include <cstdint>

typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

extern uint32_t SystemCoreClock;

typedef struct {
    uint32_t Period;
} TIM_Base_InitTypeDef;

typedef struct {
    TIM_Base_InitTypeDef Init;
} TIM_HandleTypeDef;

#define __HAL_TIM_SET_PRESCALER(x, y) ((void)x)
#define __HAL_TIM_SET_COMPARE(x, y, z) ((void)x)

HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);


#endif  // MAIN_H