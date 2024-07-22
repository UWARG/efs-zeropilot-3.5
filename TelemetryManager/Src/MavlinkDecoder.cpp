//
// Created by Yarema Dzulynsky on 2023-07-10.
//

#include "MavlinkDecoder.hpp"

MavlinkDecoder::MavlinkDecoder() {}

MavlinkDecoder::~MavlinkDecoder() {
    // no cleanup needed
}

void MavlinkDecoder::parseBytesToMavlinkMsgs(uint8_t *buffer, std::size_t bufferSize) {
    // Process the bytes in the buffer to construct MAVLink messages

    // MAVLink message structure
    mavlink_message_t currentMessage;

    // Status of the MAVLink message decoding process
    mavlink_status_t currentMessageDecodingStatus;

    std::size_t currentBufferIndex = 0;  // Index to traverse the buffer

    while (currentBufferIndex < bufferSize) {
        // Try to parse the current byte and see if it corresponds to a MAVLink message
        if (mavlink_parse_char(MAVLINK_COMM_0, buffer[currentBufferIndex], &currentMessage,
                               &currentMessageDecodingStatus)) {
            // Attempt to decode the constructed MAVLink message
            bool isMessageDecoded = decodeMsg(currentMessage);
        }
        currentBufferIndex++;
    }
}

// must return true if the message was handled successfully
bool handle_attitude(mavlink_message_t &msg) {
    mavlink_attitude_t payload;
    mavlink_msg_attitude_decode(&msg, &payload);

    // do what you want with the payload

    return true;
}

// must return true if the message was handled successfully
bool handle_global_position_int(mavlink_message_t &msg) {
    mavlink_global_position_int_t payload;
    mavlink_msg_global_position_int_decode(&msg, &payload);

    // do what you want with the payload
    return true;
}

// must return true if the message was handled successfully
bool handle_heartbeat(mavlink_message_t &msg) {
    mavlink_heartbeat_t payload;
    mavlink_msg_heartbeat_decode(&msg, &payload);

    // do what you want with the payload
    return true;
}

bool MavlinkDecoder::decodeMsg(mavlink_message_t &msg) {
    int messageId = msg.msgid;  // Extract message ID

    switch (messageId) {
        case MAVLINK_MSG_ID_ATTITUDE:
            bool handledSuccessfully = handle_attitude(msg);
            handledSuccessfully ? messagesHandledSuccessfully++ : messagesHandledSuccessfully;
            return handledSuccessfully;

            break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
            bool handledSuccessfully = handle_global_position_int(msg);
            handledSuccessfully ? messagesHandledSuccessfully++ : messagesHandledSuccessfully;
            return handledSuccessfully;

            break;
        case MAVLINK_MSG_ID_HEARTBEAT:
            bool handledSuccessfully = handle_heartbeat(msg);
            handledSuccessfully ? messagesHandledSuccessfully++ : messagesHandledSuccessfully;
            return handledSuccessfully;
            
            break;
        default:
            break;
    }
    return false;
}