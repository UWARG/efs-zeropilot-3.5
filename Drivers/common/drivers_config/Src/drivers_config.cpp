#include "drivers_config.hpp"

/*
    creating sbusReceiver instance
*/
SBUSReceiver sbus_instance(sbus_uart);
SBUSReceiver* sbus_pointer = &sbus_instance;

UARTDevice rfd900Instance(RFD900_UART);
UARTDevice* pRFD900 = &rfd900Instance;
