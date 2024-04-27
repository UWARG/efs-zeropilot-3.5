#ifndef DRIVERS_CONFIG_HPP_
#define DRIVERS_CONFIG_HPP_

#include "usart.h"
#include "sbus_receiver.hpp"
#include "uart_device.hpp"

/*
    UART Mapping
*/

#define sbus_uart       &huart2

extern SBUSReceiver* sbus_pointer;

#define RFD900_UART       &huart3

extern UARTDevice* pRFD900;

#endif