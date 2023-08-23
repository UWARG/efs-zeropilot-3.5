//
// Created by Yarema Dzulynsky on 2023-07-10.
//

#ifndef WARGMAVLINKSUPPORT_MAVLINKDECODER_H
#define WARGMAVLINKSUPPORT_MAVLINKDECODER_H


#include "../c_library_v2/common/mavlink.h"
#include "iostream"

/**
 * Macro for registering a decoder function for a specific message type.
 * @param msgId (int) - the message id of the message type to be decoded.
 * @param baseName (string) - the base name of the message type to be decoded. Ex. for mavlink_attitude_t, the base
 * name is attitude. Essentially, whatever is in between mavlink_ and _t form the message you would like to register.
 * The base name is used to generate the decoder function name. Ex. for mavlink_attitude_t, the
 * decoder function name is mavlink_msg_attitude_decode.
 * @param postProcessor ([](mavlink_**baseName**_t &message)) - the post processor function to be called after the
 * message. This function must take in a reference to the message type that is being decoded. You can then use this
 * reference to access the message fields. Ex. message.roll, message.pitch, etc. Now do what you want with them! :)
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
#define REGISTER_DECODER(msgId, baseName, postProcessor) decodingFunctions[msgId] = [](mavlink_message_t &msg) { \
                                                                                                                 \
        mavlink_ ## baseName ## _t msgType;                         \
        mavlink_msg_ ## baseName ## _decode(&msg, &msgType);                                      \
        postProcessor(msgType);\
};

class MavlinkDecoder {

public:

    //the current message being decoded
    mavlink_message_t currentMessage;
    //The decoding status of the current message being decoded
    mavlink_status_t currentMessageDecodingStatus;

    std::unordered_map<int, std::function<void(mavlink_message_t &)>> decodingFunctions;


public:
    /**
     * Constructor
     * Any new message type with its decoder and/or custom callback (after decoded)
     * should be registered here using the REGISTER_DECODER macro. Otherwise, no messages
     * will be decoded as the unorderedMap which maps Mavlink message ids corresponding
     * with the corresponding decoder will be empty.
     */
    MavlinkDecoder();

    /**
     * Destructor.
     * Currently no dynamic memory is being allocated, so no need to do anything here.
     */
    ~MavlinkDecoder();

    /**
     * Decodes the message and calls the corresponding callback function which you
     * specify within the constructor.
     * @param msg - the mavlink message (passed by reference) to be decoded.
     * This message has already been extracted from an array of bytes and
     * is ready to be decoded using the decoder functions specified in the
     * constructor.
     * @param isMessageDecoded - a boolean flag which is set to true if the message
     * was successfully decoded.
     */
    void decodeMsg(mavlink_message_t &msg, bool &isMessageDecoded);

    /**
     * Parses an array of bytes into mavlink messages and calls the corresponding
     * callback functions for each message.
     * @param buffer - the array of bytes to be parsed.
     * @param bufferSize - the size of the buffer.
     */
    void parseBytesToMavlinkMsgs(u_int8_t *buffer, std::size_t bufferSize);
};


#endif //WARGMAVLINKSUPPORT_MAVLINKDECODER_H
