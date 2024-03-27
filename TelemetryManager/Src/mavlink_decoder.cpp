//
// Created by Yarema Dzulynsky on 2023-07-10.
//

#include "../Inc/mavlink_decoder.h"

long MavlinkDecoder::decodedMessages = 0;

MavlinkDecoder::MavlinkDecoder() {
    /*
    Register message types with their decoders and post-decoding callbacks here. This is just a
    placeholder.
    */
    REGISTER_DECODER(MAVLINK_MSG_ID_ATTITUDE, attitude, [](mavlink_attitude_t &message) {

    });

    REGISTER_DECODER(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, global_position_int,
                     [](mavlink_global_position_int_t &message) {

                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_HEARTBEAT, heartbeat, [](mavlink_heartbeat_t &message) {
        std::cout << "HEARTBEAT" << std::endl;
        std::cout << "type: " << message.type << std::endl;
        std::cout << "autopilot: " << message.autopilot << std::endl;
        std::cout << "base_mode: " << message.base_mode << std::endl;
        std::cout << "custom_mode: " << message.custom_mode << std::endl;
        std::cout << "system_status: " << message.system_status << std::endl;
        std::cout << "mavlink_version: " << message.mavlink_version << std::endl << std::endl;
    });

    REGISTER_DECODER(MAVLINK_MSG_ID_SYS_STATUS, sys_status, [](mavlink_sys_status_t &message) {

    });

    REGISTER_DECODER(MAVLINK_MSG_ID_GPS_RAW_INT, gps_raw_int, [](mavlink_gps_raw_int_t &message) {

    });

    REGISTER_DECODER(MAVLINK_MSG_ID_MISSION_CURRENT, mission_current,
                     [](mavlink_mission_current_t &message) {

                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT, nav_controller_output,
                     [](mavlink_nav_controller_output_t &message) {

                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_RC_CHANNELS, rc_channels, [](mavlink_rc_channels_t &message) {

    });

    REGISTER_DECODER(MAVLINK_MSG_ID_ALTITUDE, altitude, [](mavlink_altitude_t &message) {

    });

    REGISTER_DECODER(MAVLINK_MSG_ID_BATTERY_STATUS, battery_status,
                     [](mavlink_battery_status_t &message) {

                     });
}

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
            bool isMessageDecoded;
            // Attempt to decode the constructed MAVLink message
            decodeMsg(currentMessage, isMessageDecoded);
        }
        currentBufferIndex++;
    }
}

void MavlinkDecoder::decodeMsg(mavlink_message_t &msg, bool &isMessageDecoded) {
    // Attempt to decode the MAVLink message using the registered decoding functions

    int messageId = msg.msgid;  // Extract message ID

    // Try to find a decoding function for the given message ID
    auto it = decodingFunctions.find(messageId);

    if (it != decodingFunctions.end()) {
        // Found a decoder: Call it
        auto decodingFunctionName = it->second;
        decodingFunctionName(msg);
        isMessageDecoded = true;
    } else {
        // Decoder not found: Indicate failure
        isMessageDecoded = false;
    }
}
