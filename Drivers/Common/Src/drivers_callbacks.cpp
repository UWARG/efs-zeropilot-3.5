#include "sbus_receiver.hpp"
#include "drivers_config.hpp"

/*
Define driver callback functions here
*/

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size){
    if (pRFD900->matchUART(huart)) {
        // Receive data
    	pRFD900->callback(size);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == sbus_uart){
        sbus_pointer->parse();
        HAL_UART_Receive_DMA (huart, sbus_pointer->raw_sbus_, SBUS_FRAME_SIZE);
        sbus_pointer->is_data_new_ = false;
    }

    if(pRFD900->matchUART(huart)){
        // Should never enter this callback
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart == sbus_uart){
        HAL_UART_DMAStop(huart);
        HAL_UART_Receive_DMA(huart, sbus_pointer->raw_sbus_, SBUS_FRAME_SIZE);
    }

    if(pRFD900->matchUART(huart)){
        // Clear error and receive data
        HAL_UART_DMAStop(huart);
        pRFD900->callback(UARTDevice::BUFFER_SIZE);
    }
}