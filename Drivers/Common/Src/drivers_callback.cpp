#include "drivers_config.hpp"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size){
    if (pRFD900->matchUART(huart)) {
        // Receive data
    	pRFD900->callback(size);
    }
}

 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
    if(pRFD900->matchUART(huart)){
        // Should never enter this callback
    }
 }

 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
 {
    if(pRFD900->matchUART(huart)){
        // Clear error and receive data
        HAL_UART_DMAStop(huart);
        pRFD900->callback(UARTDevice::BUFFER_SIZE);
    }
 }
 