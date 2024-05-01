#ifndef DRIVERS_CONFIG_HPP_
#define DRIVERS_CONFIG_HPP_

#include "usart.h"
#include "sbus_receiver.hpp"
#include "dma_uart_device.hpp"
/*
    UART Mapping
*/

#define sbus_uart       &huart2

extern SBUSReceiver* sbus_pointer;

#define RFD900_UART       &huart3

extern UARTDevice* pRFD900;
extern CircularBuffer *rfd900_circular_buffer;

// const uint8_t RFD900_BUF_SIZE = 280;
// uint8_t rfd900_buf[RFD900_BUF_SIZE];
// CircularBuffer rfd900_circular_buffer_inst(rfd900_buf, RFD900_BUF_SIZE);


#endif