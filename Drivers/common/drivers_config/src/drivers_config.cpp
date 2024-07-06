#include "drivers_config.hpp"

/*
    creating sbusReceiver instance
*/
SBUSReceiver sbus_instance(sbus_uart);
SBUSReceiver* sbus_pointer = &sbus_instance;

/*
    creating RFD900 instance
*/
uint8_t rfd900_buf[RFD900_BUF_SIZE];
CircularBuffer rfd900_circular_buffer_inst(rfd900_buf, RFD900_BUF_SIZE);

CircularBuffer* rfd900_circular_buffer = &rfd900_circular_buffer_inst;

UARTDevice rfd900Instance(RFD900_UART, rfd900_circular_buffer);
UARTDevice* pRFD900 = &rfd900Instance;
