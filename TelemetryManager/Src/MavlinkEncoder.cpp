// This file was created by Yarema Dzulynsky on 2023-07-24.
#include "MavlinkEncoder.hpp"

// Default constructor for the MavlinkEncoder class.
MavlinkEncoder::MavlinkEncoder() = default;

// Default destructor for the MavlinkEncoder class.
MavlinkEncoder::~MavlinkEncoder() = default;

/**
 * @brief Packs a mavlink message into a buffer.
 * 
 * @param msg The Mavlink message to be packed into the buffer.
 * @param txToGroundByteQueue The buffer to pack the message into.
 * @return bool True if the message was successfully packed into the buffer, false otherwise. 
 */
bool MavlinkEncoder::encodeMsgIntoBuffer(mavlink_message_t &msg,
                                      TMCircularBuffer &txToGroundByteQueue) {
    uint8_t messageBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
    size_t messageBufferLen = mavlink_msg_to_send_buffer(messageBuffer, &msg);
    bool success = txToGroundByteQueue.enqueue(messageBuffer, messageBufferLen);
    return success;
}
