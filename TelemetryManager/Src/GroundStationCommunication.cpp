

#include "GroundStationCommunication.hpp"
#include "drivers_config.hpp"

GroundStationCommunication::GroundStationCommunication() {
    // Constructor
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

void GroundStationCommunication::updateDMAReceiveBuffer() { // call this method in drivers_callbacks.cpp

    int bytesReceived = pRFD900->getAvailDataSize();

    // if GSC.DMAReceiveBuffer has enough space for the new data add it
    // otherwise discard the data
    if(DMAReceiveBuffer.remainingMemory() > bytesReceived) {
        // add the new data to GSC.DMAReceiveBuffer
        pRFD900->read(internalBuffer_, bytesReceived);
        for (int i = 0; i < bytesReceived; ++i) {
            DMAReceiveBuffer.enqueue(internalBuffer_[i]);
        }
    }
    else{
        // discard the new data
        //not a great way to handle this, but it's a start
        pRFD900->read(internalBuffer_, bytesReceived);
    }

    // end of ISR
    return;
}