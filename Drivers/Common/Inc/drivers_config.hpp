#ifndef DRIVERS_CONFIG_HPP_
#define DRIVERS_CONFIG_HPP_

#include "usart.h"
#include "sbus_receiver.hpp"
#include "iwdg.h"

/*
    UART Mapping
*/

#define sbus_uart       &huart2
#define watchdog_reference &hiwdg

extern SBUSReceiver* sbus_pointer;

#endif