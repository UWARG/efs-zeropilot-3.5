/**
 * @file MavlinkEncoder.hpp
 * @brief This file is responsible for decoding MAVLink messages received from the ground station.
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version Milestone 2
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef MAVLINKDECODER_H
#define MAVLINKDECODER_H

#include <functional>
// #include <iostream>
// #include <unordered_map>

#include "Official_Mavlink_2_Library/common/mavlink.h"

/**
 * Macro for registering a decoder function for a specific message type.
 * @param msgId (int) - the message id of the message type to be decoded.
 * @param baseName (string) - the base name of the message type to be decoded. Ex. for
 * mavlink_attitude_t, the base name is attitude. Essentially, whatever is in between mavlink_ and
 * _t form the message you would like to register. The base name is used to generate the decoder
 * function name. Ex. for mavlink_attitude_t, the decoder function name is
 * mavlink_msg_attitude_decode.
 * @param postProcessor ([](mavlink_**baseName**_t &message)) - the post processor function to be
 * called after the message. This function must take in a reference to the message type that is
 * being decoded. You can then use this reference to access the message fields. Ex. message.roll,
 * message.pitch, etc. Now do what you want with them! :)
 * @example REGISTER_DECODER(MAVLINK_MSG_ID_ATTITUDE, attitude, [](mavlink_attitude_t &message) {
 *     //print out the message
 *     std::cout << "ATTITUDE" << std::endl;
 *     std::cout << "roll: " << message.roll << std::endl;
 *     std::cout << "pitch: " << message.pitch << std::endl;
 *     std::cout << "yaw: " << message.yaw << std::endl;
 *     std::cout << "rollspeed: " << message.rollspeed << std::endl;
 *     std::cout << "pitchspeed: " << message.pitchspeed << std::endl;
 *     };
 */
// #define REGISTER_DECODER(msgId, baseName, postProcessor)    \
//     decodingFunctions[msgId] = [](mavlink_message_t &msg) { \
//         mavlink_##baseName##_t msgType;                     \
//         mavlink_msg_##baseName##_decode(&msg, &msgType);    \
//         postProcessor(msgType);                             \
//     };

class MavlinkDecoder {
   public:
    // The number of messages that have been decoded - used for testing purposes.
    long messagesHandledSuccessfully = 0;

    // std::unordered_map<int, std::function<void(mavlink_message_t &)>> decodingFunctions;
    /**
     * Default constructor.
     */
    MavlinkDecoder();

    /**
     * Default destructor.
     */
    ~MavlinkDecoder();

    /**
     * Unpacks the message, triggering the respective callback set in the switch statement.
     * @param msg - The preformatted mavlink message, ready for decoding.
     */

    bool decodeMsg(mavlink_message_t &msg);

    /**
     * Transforms a byte sequence into mavlink messages, then activates the respective callbacks.
     * @param buffer - Byte sequence for parsing.
     * @param bufferSize - Length of the byte sequence.
     */
    void parseBytesToMavlinkMsgs(uint8_t *buffer, std::size_t bufferSize);
};

#endif