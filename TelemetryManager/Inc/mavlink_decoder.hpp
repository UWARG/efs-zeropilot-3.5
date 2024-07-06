//
// Created by Yarema Dzulynsky on 2023-07-10.
//

#ifndef WARGMAVLINKSUPPORT_MAVLINKDECODER_H
#define WARGMAVLINKSUPPORT_MAVLINKDECODER_H

#include <functional>
#include <iostream>
#include <unordered_map>

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
#define REGISTER_DECODER(msgId, baseName, postProcessor)    \
    decodingFunctions[msgId] = [](mavlink_message_t &msg) { \
        mavlink_##baseName##_t msgType;                     \
        mavlink_msg_##baseName##_decode(&msg, &msgType);    \
        MavlinkDecoder::decodedMessages++;                  \
        postProcessor(msgType);                             \
    };


class MavlinkDecoder {
   public:

    std::unordered_map<int, std::function<void(mavlink_message_t &)>> decodingFunctions;
    /**
     * The number of decoded messages. This is used for test cases
     */
    static long decodedMessages;

   public:
    /**
     * Default constructor.
     * Register message types with their decoders and post-decoding callbacks here.
     * Without using REGISTER_DECODER, the decoder map remains unpopulated.
     */
    MavlinkDecoder();

    /**
     * Default destructor.
     * No specific cleanup is required since no dynamic memory is utilized.
     */
    ~MavlinkDecoder();

    /**
     * Unpacks the message, triggering the respective callback set during construction.
     * @param msg - The preformatted mavlink message, ready for decoding.
     * @param isMessageDecoded - Flag that indicates successful decoding.
     */
    void decodeMsg(mavlink_message_t &msg, bool &isMessageDecoded);

    /**
     * Transforms a byte sequence into mavlink messages, then activates the respective callbacks.
     * @param buffer - Byte sequence for parsing.
     * @param bufferSize - Length of the byte sequence.
     */
    void parseBytesToMavlinkMsgs(uint8_t *buffer, std::size_t bufferSize);
};

#endif  // WARGMAVLINKSUPPORT_MAVLINKDECODER_H