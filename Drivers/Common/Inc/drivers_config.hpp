#ifndef DRIVERS_CONFIG_HPP_
#define DRIVERS_CONFIG_HPP_

#include "usart.h"
#include "sbus_receiver.hpp"

/*
    UART Mapping
*/

#define sbus_uart       &huart2

extern SBUSReceiver* sbus_pointer;

#endif