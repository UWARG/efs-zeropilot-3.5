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

typedef struct {
    uint32_t Period;
} TIM_Base_InitTypeDef;

typedef struct {
    TIM_Base_InitTypeDef Init;
} TIM_HandleTypeDef;


#endif  // MAIN_H