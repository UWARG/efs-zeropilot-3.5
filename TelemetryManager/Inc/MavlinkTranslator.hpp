/**
 * @file MavlinkTranslator.hpp
 * @brief This file is responsible for decoding/encoding MAVLink messages received_from/sent_to the ground station.
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version Milestone 2
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#include "MavlinkDecoder.hpp"
#include "MavlinkEncoder.hpp"
#include "Official_Mavlink_2_Library/common/mavlink.h"
#include "TMCircularBuffer.hpp"
#ifndef MAVLINKTRANSLATOR_H
#define MAVLINKTRANSLATOR_H

class MavlinkTranslator {
      public:
    // The decoder and encoder objects.
    MavlinkDecoder decoder;
    MavlinkEncoder encoder;
    // The number of messages that have been decoded - used for testing purposes.
    long decodedMessages = 0;
    /**
     * @brief Construct a new MavlinkTranslator object. Do whatever needs to be done here.
     *
     */
    MavlinkTranslator();

    /**
     * @brief Destroy and cleanup memory. Do whatever
     * needs to be done here.
     *
     */
    ~MavlinkTranslator();

    /**
     * @brief Ingests bytes from the rxFromGroundByteQueue and converts them to Mavlink messages.
     * The Mavlink message, associated callbacks are then triggered.
     *
     * @param rxFromGroundByteQueue The CircularBuffer containing the bytes received from the ground
     * station. waiting to be decoded.
     */
    void bytesToMavlinkMsg(TMCircularBuffer &rxFromGroundByteQueue);

    /**
     * @brief Converts Mavlink messages to bytes and adds them to the txToGroundByteQueue.
     *
     * @param msg The Mavlink message to be converted to bytes and added to the txToGroundByteQueue.
     * @param txToGroundByteQueue The CircularBuffer containing the bytes to be sent to the ground
     * station.
     */
    void addMavlinkMsgToByteQueue(mavlink_message_t &msg, TMCircularBuffer &txToGroundByteQueue);
};

#endif  // MAVLINKTRANSLATOR_H
