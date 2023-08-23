//
// Created by Yarema Dzulynsky on 2023-07-10.
//

#include "../Inc/MavlinkDecoder.h"

MavlinkDecoder::MavlinkDecoder() {
    // Existing message types
    REGISTER_DECODER(MAVLINK_MSG_ID_ATTITUDE, attitude,
                     [](mavlink_attitude_t &message) {
                         std::cout << "ATTITUDE" << std::endl;
                         std::cout << "roll: " << message.roll << std::endl;
                         std::cout << "pitch: " << message.pitch << std::endl;
                         std::cout << "yaw: " << message.yaw << std::endl;
                         std::cout << "rollspeed: " << message.rollspeed << std::endl;
                         std::cout << "pitchspeed: " << message.pitchspeed << std::endl << std::endl;


                         //something here has to be done

                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, global_position_int,
                     [](mavlink_global_position_int_t &message) {

                         std::cout << "GLOBAL_POSITION_INT" << std::endl;
                         std::cout << "lat: " << message.lat << std::endl;
                         std::cout << "lon: " << message.lon << std::endl;
                         std::cout << "alt: " << message.alt << std::endl;
                         std::cout << "relative_alt: " << message.relative_alt << std::endl;
                         std::cout << "vx: " << message.vx << std::endl;
                         std::cout << "vy: " << message.vy << std::endl;
                         std::cout << "vz: " << message.vz << std::endl;
                         std::cout << "hdg: " << message.hdg << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_HEARTBEAT, heartbeat,
                     [](mavlink_heartbeat_t &message) {
                         std::cout << "HEARTBEAT" << std::endl;
                         std::cout << "type: " << message.type << std::endl;
                         std::cout << "autopilot: " << message.autopilot << std::endl;
                         std::cout << "base_mode: " << message.base_mode << std::endl;
                         std::cout << "custom_mode: " << message.custom_mode << std::endl;
                         std::cout << "system_status: " << message.system_status << std::endl;
                         std::cout << "mavlink_version: " << message.mavlink_version << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_SYS_STATUS, sys_status,
                     [](mavlink_sys_status_t &message) {
                         std::cout << "SYS_STATUS" << std::endl;
                         std::cout << "onboard_control_sensors_present: " << message.onboard_control_sensors_present
                                   << std::endl;
                         std::cout << "onboard_control_sensors_enabled: " << message.onboard_control_sensors_enabled
                                   << std::endl;
                         std::cout << "onboard_control_sensors_health: " << message.onboard_control_sensors_health
                                   << std::endl;
                         std::cout << "load: " << message.load << std::endl;
                         std::cout << "voltage_battery: " << message.voltage_battery << std::endl;
                         std::cout << "current_battery: " << message.current_battery << std::endl;
                         std::cout << "battery_remaining: " << message.battery_remaining << std::endl;
                         std::cout << "drop_rate_comm: " << message.drop_rate_comm << std::endl;
                         std::cout << "errors_comm: " << message.errors_comm << std::endl;
                         std::cout << "errors_count1: " << message.errors_count1 << std::endl;
                         std::cout << "errors_count2: " << message.errors_count2 << std::endl;
                         std::cout << "errors_count3: " << message.errors_count3 << std::endl;
                         std::cout << "errors_count4: " << message.errors_count4 << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_GPS_RAW_INT, gps_raw_int,
                     [](mavlink_gps_raw_int_t &message) {
                         std::cout << "GPS_RAW_INT" << std::endl;
                         std::cout << "time_usec: " << message.time_usec << std::endl;
                         std::cout << "fix_type: " << message.fix_type << std::endl;
                         std::cout << "lat: " << message.lat << std::endl;
                         std::cout << "lon: " << message.lon << std::endl;
                         std::cout << "alt: " << message.alt << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_MISSION_CURRENT, mission_current,
                     [](mavlink_mission_current_t &message) {
                         std::cout << "MISSION_CURRENT" << std::endl;
                         std::cout << "seq: " << message.seq << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT, nav_controller_output,
                     [](mavlink_nav_controller_output_t &message) {
                         std::cout << "NAV_CONTROLLER_OUTPUT" << std::endl;
                         std::cout << "nav_roll: " << message.nav_roll << std::endl;
                         std::cout << "nav_pitch: " << message.nav_pitch << std::endl;
                         std::cout << "nav_bearing: " << message.nav_bearing << std::endl;
                         std::cout << "target_bearing: " << message.target_bearing << std::endl;
                         std::cout << "wp_dist: " << message.wp_dist << std::endl << std::endl;
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_RC_CHANNELS, rc_channels,
                     [](mavlink_rc_channels_t &message) {
                         std::cout << "RC_CHANNELS" << std::endl;
                         std::cout << "chan1_raw: " << message.chan1_raw << std::endl;
                         std::cout << "chan2_raw: " << message.chan2_raw << std::endl;
                         std::cout << "chan3_raw: " << message.chan3_raw << std::endl;
                         std::cout << "chan4_raw: " << message.chan4_raw << std::endl;
                         std::cout << "chan5_raw: " << message.chan5_raw << std::endl << std::endl;

                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_ALTITUDE, altitude,
                     [](mavlink_altitude_t &message) {
                         std::cout << "ALTITUDE" << std::endl;
                         std::cout << "altitude_terrain: " << message.altitude_terrain << std::endl;
                         std::cout << "altitude_amsl: " << message.altitude_amsl << std::endl;
                         std::cout << "altitude_local: " << message.altitude_local << std::endl;
                         std::cout << "altitude_relative: " << message.altitude_relative << std::endl << std::endl;
                         // and so on...
                     });

    REGISTER_DECODER(MAVLINK_MSG_ID_BATTERY_STATUS, battery_status,
                     [](mavlink_battery_status_t &message) {
                         std::cout << "BATTERY_STATUS" << std::endl;
                         std::cout << "id: " << message.id << std::endl;
                         std::cout << "battery_function: " << message.battery_function << std::endl;
                         std::cout << "type: " << message.type << std::endl;
                         std::cout << "temperature: " << message.temperature << std::endl << std::endl;
                         // and so on...
                     });

}


MavlinkDecoder::~MavlinkDecoder() {

}

void MavlinkDecoder::parseBytesToMavlinkMsgs(u_int8_t *buffer, std::size_t bufferSize) {

    //The index of the current byte in the buffer.
    std::size_t currentBufferIndex = 0;

    //Continue parsing until the end of the buffer is reached
    while (currentBufferIndex < bufferSize) {

        /* If the mavlinkParseChar function returns true, then the previous bytes in the buffer do correspond to a
         * generic mavlink message. Note, since currentMessage is passed by reference, the mavlinkParseChar function will
         * pack the bytes into the currentMessage variable therefore building the generic message from the bytes. This
         * generic message can then be decoded into its corresponding specific message type. Ex. mavlink_attitude_t.
         */
        if (mavlink_parse_char(MAVLINK_COMM_0, buffer[currentBufferIndex], &currentMessage,
                               &currentMessageDecodingStatus)) {
            bool isMessageDecoded;
            //Decode the generic message into its corresponding specific message type and callback the registered
            // decoder specified in the constructor.
            decodeMsg(currentMessage, isMessageDecoded);
        }
        //Increment the index of the current byte in the buffer.
        currentBufferIndex++;
    }
}


void MavlinkDecoder::decodeMsg(mavlink_message_t &msg, bool &isMessageDecoded) {

    //Get the message id from the generic message.
    int messageId = msg.msgid;

    //Find the decoding function we specified in the constructor for the message id within the decoding functions map.
    auto it = decodingFunctions.find(messageId);

    //If the decoding function is found, then call the decoding function and pass in the generic message.
    if (it != decodingFunctions.end()) {
        auto decodingFunctionName = it->second;

        //Call the decoding function and return true.
        decodingFunctionName(msg);
        isMessageDecoded = true;
    } else {
        //If the decoding function is not found, then return false.
        isMessageDecoded = false;
    }

}

