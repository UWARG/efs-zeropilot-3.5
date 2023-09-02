//
// Created by Yarema Dzulynsky on 2023-07-24.
//

#ifndef WARGMAVLINKSUPPORT_BASICMAVLINKENCODER_H
#define WARGMAVLINKSUPPORT_BASICMAVLINKENCODER_H

#include <iostream>
#include "c_library_v2/common/mavlink.h"
#include "IncomingData.h"

/**
 * @brief ENCODE_MESSAGE macro for MAVLink message packing
 *
 * This macro is used to pack MAVLink messages of various types. It takes the base name of the MAVLink
 * message and a variable number of arguments specific to the message type. It creates an anonymous lambda
 * function that packs the provided arguments into a MAVLink message and converts it to a byte buffer.
 *
 * @param baseName The base name of the MAVLink message (e.g., HEARTBEAT, COMMAND_LONG, etc.).
 * @param ...      Variable number of arguments specific to the MAVLink message type. These arguments
 *                 are used for populating the fields of the message.
 *
 * @return std::size_t The length of the byte buffer containing the packed MAVLink message.
 *
 * @note The ENCODE_MESSAGE macro uses lambda functions to encapsulate the packing process for different
 * MAVLink message types. The resulting lambda function can be used to efficiently pack MAVLink messages
 * and obtain the length of the packed buffer.
 *
 * @note The ENCODE_MESSAGE macro is used in the following way:
 *      ENCODE_MESSAGE(HEARTBEAT, 1, 2, 3, 4, 5, 6, 7, 8, 9)(msg, buffer);
 *      where HEARTBEAT is the base name of the MAVLink message and 1, 2, 3, 4, 5, 6, 7, 8, 9 are the
 *      arguments specific to the HEARTBEAT message type.
 *      The parameters msg and buffer are passed by reference to the lambda function and are used to
 *      store the packed MAVLink message and the packed buffer, respectively.
 *
 */
#define ENCODE_MESSAGE(baseName, ...) \
    ([&](mavlink_message_t& msg, uint8_t* buffer) -> std::size_t { \
        mavlink_msg_##baseName##_pack(0, 0, &msg, __VA_ARGS__); \
        std::size_t bufferLen = mavlink_msg_to_send_buffer(buffer, &msg); \
        return bufferLen; \
    })


/**
 * Setting a predefined buffer size, derived from the maximum packet length allowed by MAVLink.
 * @note IMPORTANT: IF THE THE MAX_ARRAY_BUFFER_SIZE < SIZE OF BYTES INCOMING MEMORY MAY
 * BE OVERWRITTEN!
 */
#define MAX_ARRAY_BUFFER_SIZE (10 * MAVLINK_MAX_PACKET_LEN)

/**
 * @brief Class dedicated to MAVLink message encoding.
 *
 * The 'MavlinkEncoder' class provides an interface and underlying functionality for MAVLink message
 * encoding, using associated parameters and defined message types.
 */
class MavlinkEncoder {
   public:
    // The message object that's currently targeted for encoding.
    mavlink_message_t currentMessage;

    // Default constructor for the MavlinkEncoder class.
    MavlinkEncoder();

    /**
     * @brief Identifies the appropriate encoding function based on the incoming data.
     *
     * This function seeks the suitable encoding mechanism in line with the data specifics
     * and subsequently populates the output buffer.
     *
     * @param data Incoming data to be encoded.
     * @param outputBuffer The buffer to store the encoded data.
     * @param maxBufferSize The maximum size the output buffer can occupy.
     *
     * @return Returns the size (in bytes) used in the output buffer.
     */
    size_t packIntoMavlinkByteArray(IncomingData& data, uint8_t* outputBuffer, size_t maxBufferSize);
};

#endif //WARGMAVLINKSUPPORT_BASICMAVLINKENCODER_H
