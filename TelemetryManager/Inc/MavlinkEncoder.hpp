/**
 * @file MavlinkEncoder.hpp
 * @brief This file is responsible for encoding MAVLink messages to send to the ground station.
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version Milestone 2
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef MAVLINKENCODER_H
#define MAVLINKENCODER_H

#include <iostream>

#include "Official_Mavlink_2_Library/common/mavlink.h"
#include "TMCircularBuffer.hpp"

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

    // Default destructor for the MavlinkEncoder class.
    ~MavlinkEncoder();

    /**
     * @brief Helper function to pack a mavlink message into a buffer.
     *
     * @param msg The mavlink message to be packed into the buffer.
     * @param txToGroundByteQueue The buffer to pack the message into.
     */
    bool encodeMsgIntoBuffer(mavlink_message_t& msg, TMCircularBuffer& txToGroundByteQueue);
};

#endif