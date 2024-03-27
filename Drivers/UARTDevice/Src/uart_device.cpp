#include "uart_device.hpp"
#include "main.h"

UARTDevice::UARTDevice(UART_HandleTypeDef* huart)
    :
    uart_handle_(huart), circular_buf_(buf_, BUFFER_SIZE) {}

void UARTDevice::init() {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle_, it_recv_buf_, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(uart_handle_->hdmarx, DMA_IT_HT);
}

void UARTDevice::callback(uint16_t data_size) {
    HAL_UARTEx_ReceiveToIdle_DMA(uart_handle_, it_recv_buf_, BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(uart_handle_->hdmarx, DMA_IT_HT);

    for (uint16_t i = 0; i < data_size; ++i) {
        circular_buf_.write(it_recv_buf_[i]);
    }
}

bool UARTDevice::matchUART(UART_HandleTypeDef* huart) {
    bool match = false;

    if(uart_handle_ != NULL && huart != NULL) {
        match = (uart_handle_ == huart);
    }

    return match;
}

void UARTDevice::transmit(uint8_t* buf, uint16_t size) {
    HAL_UART_Transmit(uart_handle_, buf, size, TRANSMIT_TIMEOUT);
}

uint16_t UARTDevice::getAvailDataSize() {
    return circular_buf_.getNumAvailBytes();
}

bool UARTDevice::read(uint8_t* buf, uint16_t size) {
    return circular_buf_.read(buf, size);
}