

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

// ** Implement transmit first **

void GroundStationCommunication::transmit(TMCircularBuffer &transmissionBuffer) {
    // bool noMoreMessages = false;
    // START: Send the bytes in transmissionBuffer to the ground station via RFD900

    while (transmissionBuffer.messagesInQueue >= 0) {
        // HAL_Delay(1000);
        int bytesToTransmit = transmissionBuffer.bytesUntilLastMessageEnd();

        // while (!noMoreMessages) {
        // HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
        // HAL_Delay(1000);
        if (bytesToTransmit > RFD900_BUF_SIZE) {
            bytesToTransmit = RFD900_BUF_SIZE;
        }

        for (int i{0}; i < bytesToTransmit; ++i) {
            internalBuffer_[i] = transmissionBuffer.dequeue();
        }
        pRFD900->transmit(internalBuffer_, bytesToTransmit);
        transmissionBuffer.messagesInQueue--;
    }

    // bytesToTransmit = transmissionBuffer.bytesUntilLastMessageEnd(&noMoreMessages);
    // }

    // END: Send the bytes in transmissionBuffer to the ground station via RFD900

    return;
}
