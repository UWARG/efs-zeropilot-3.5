#ifndef UNIT_TESTING_USART_H
#define UNIT_TESTING_USART_H

#include <cstdint>

typedef struct __UART_HandleTypeDef{
}UART_HandleTypeDef;

typedef enum{
} HAL_StatusTypeDef;

HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif