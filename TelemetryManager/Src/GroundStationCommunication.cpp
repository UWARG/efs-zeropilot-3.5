

#include "GroundStationCommunication.hpp"

#include "uart_device.hpp"

GroundStationCommunication::GroundStationCommunication() {
    // Constructor
    uint8_t *buf = new uint8_t[280];
    TMCircularBuffer tmcb(buf, 280);
    UARTDevice *something = new UARTDevice(&huart1, tmcb);
}

GroundStationCommunication::~GroundStationCommunication() {
    // Destructor
}

// ** Implement transmit first **

void GroundStationCommunication::transmit(TMCircularBuffer &transmissionBuffer) {
    // START: Send the bytes in transmissionBuffer to the ground station via RFD900

    // END: Send the bytes in transmissionBuffer to the ground station via RFD900

    return;
}

void GroundStationCommunication::receiveInterruptServiceRoutine() {
    int bytesReceived = 0;  // replace with actual number of bytes received

    // if GSC.DMAReceiveBuffer has enough space for the new data add it
    // otherwise discard the data
    if (DMAReceiveBuffer.remainingMemory() > bytesReceived) {
        // add the new data to GSC.DMAReceiveBuffer
    } else {
        // discard the new data
        // not a great way to handle this, but it's a start
    }

    // end of ISR
    return;
}