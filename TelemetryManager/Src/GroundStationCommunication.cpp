

#include "GroundStationCommunication.hpp"

#include "drivers_config.hpp"

GroundStationCommunication::GroundStationCommunication(
    TMCircularBuffer& DMAReceiveBuffer, uint8_t *lowPriorityTransmitBuffer,
    uint8_t *highPriorityTransmitBuffer, int length):
    DMAReceiveBuffer(DMAReceiveBuffer),
    lowPriorityTransmitBuffer(lowPriorityTransmitBuffer, length),
    highPriorityTransmitBuffer(highPriorityTransmitBuffer, length) {

            
}

GroundStationCommunication::~GroundStationCommunication() {
    // Destructor
}

// ** Implement transmit first **

void GroundStationCommunication::transmit(TMCircularBuffer &transmissionBuffer) {
    // START: Send the bytes in transmissionBuffer to the ground station via RFD900
    int bytesToTransmit = transmissionBuffer.bytesUntilLastMessageEnd();

    for (int i {0}; i < bytesToTransmit; ++i) {
        internalBuffer_[i] = transmissionBuffer.dequeue();
    }
    pRFD900->transmit(internalBuffer_, bytesToTransmit);

    // END: Send the bytes in transmissionBuffer to the ground station via RFD900

    return;
}
