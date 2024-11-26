

#include "GroundStationCommunication.hpp"

#include "drivers_config.hpp"

GroundStationCommunication::GroundStationCommunication(TMCircularBuffer &DMAReceiveBuffer,
                                                       uint8_t *lowPriorityTransmitBuffer,
                                                       uint8_t *highPriorityTransmitBuffer,
                                                       int length)
    : DMAReceiveBuffer(DMAReceiveBuffer),
      lowPriorityTransmitBuffer(lowPriorityTransmitBuffer, length),
      highPriorityTransmitBuffer(highPriorityTransmitBuffer, length) {}

GroundStationCommunication::~GroundStationCommunication() {
    // Destructor
}

void GroundStationCommunication::transmit(TMCircularBuffer &transmissionBuffer) {
    // START: Send the bytes in transmissionBuffer to the ground station via RFD900

    // While there are still messages in the transmissionBuffer, send em
    while (transmissionBuffer.messagesInQueue >= 0) {
        int bytesToTransmit = transmissionBuffer.bytesUntilMessageEnd();

        // Make sure we don't overflow the buffer
        if (bytesToTransmit > RFD900_BUF_SIZE) {
            bytesToTransmit = RFD900_BUF_SIZE;
        }

        // Dequeue the bytes from the transmissionBuffer and put them in the internal buffer
        for (int i{0}; i < bytesToTransmit; ++i) {
            internalBuffer_[i] = transmissionBuffer.dequeue();
        }
        // Send the bytes in the internal buffer to the ground station via RFD900
        pRFD900->transmit(internalBuffer_, bytesToTransmit);

        // Decrement the number of messages in the transmissionBuffer
        transmissionBuffer.messagesInQueue--;
    }

    // END: Send the bytes in transmissionBuffer to the ground station via RFD900

    return;
}
