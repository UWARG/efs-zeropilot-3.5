#include "MavlinkTranslator.hpp"

MavlinkTranslator::MavlinkTranslator() {
    // Constructor
}

MavlinkTranslator::~MavlinkTranslator() {
    // Destructor
}



void MavlinkTranslator::bytesToMavlinkMsg(TMCircularBuffer &rxFromGroundByteQueue) {
    // Take bytes in rxFromGroundByteQueue and convert them to Mavlink messages
    MavlinkDecoder decoder;
    bool success = true;
    uint8_t bytesUntilEnd = 0;

    while (success) {
        bytesUntilEnd = rxFromGroundByteQueue.bytesUntilLastMessageEnd(&success);

        if (success) {
            if (bytesUntilEnd != 0) {
                uint8_t rxByte[bytesUntilEnd];

                for (uint8_t i = 0; i < bytesUntilEnd; i++) {
                    rxByte[i] = rxFromGroundByteQueue.dequeue();
                }
                decoder.parseBytesToMavlinkMsgs(rxByte, bytesUntilEnd);
                decodedMessages++;
            }
        }
    }
}

void MavlinkTranslator::addMavlinkMsgToByteQueue(mavlink_message_t &msg,
                                                 TMCircularBuffer &txToGroundByteQueue) {

    switch(msg.msgid) {
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
            uint8_t globalPositionIntBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
            size_t globalPositionIntBufferLen = mavlink_msg_to_send_buffer(globalPositionIntBuffer, &msg);
            for (size_t i = 0; i < globalPositionIntBufferLen; i++) {
                txToGroundByteQueue.enqueue(globalPositionIntBuffer[i]);
            }
            break;
        }
        case MAVLINK_MSG_ID_ATTITUDE: {
            uint8_t attitudeBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
            size_t attitudeBufferLen = mavlink_msg_to_send_buffer(attitudeBuffer, &msg);
            for (size_t i = 0; i < attitudeBufferLen; i++) {
                txToGroundByteQueue.enqueue(attitudeBuffer[i]);
            }
            break;
        }
        case MAVLINK_MSG_ID_HEARTBEAT: {
            uint8_t heartbeatBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
            size_t heartbeatBufferLen = mavlink_msg_to_send_buffer(heartbeatBuffer, &msg);
            for (size_t i = 0; i < heartbeatBufferLen; i++) {
                txToGroundByteQueue.enqueue(heartbeatBuffer[i]);
            }
            break;
        }
        default:
            throw std::invalid_argument("Message ID not supported!");
            break;
    }


}