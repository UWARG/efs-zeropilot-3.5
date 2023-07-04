#include "sbus_receiver.hpp"
#include "drivers_config.hpp"

/*
Define driver callback functions here
*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == sbus_uart){
        sbus_pointer->parse();
        HAL_UART_Receive_DMA (huart, sbus_pointer->raw_sbus_, SBUS_FRAME_SIZE);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart == sbus_uart){
        HAL_UART_DMAStop(huart);
        HAL_UART_Receive_DMA(huart, sbus_pointer->raw_sbus_, SBUS_FRAME_SIZE);
    }
}