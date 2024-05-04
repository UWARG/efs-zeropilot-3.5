/**
 * @file MavlinkTranslator.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#include "CircularBuffer.hpp"
#include "Official_Mavlink_2_Library/common/mavlink.h"
#ifndef MAVLINKTRANSLATOR_H
#define MAVLINKTRANSLATOR_H

class MavlinkTranslator {
   public:
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
    void bytesToMavlinkMsg(CircularBuffer &rxFromGroundByteQueue);

    /**
     * @brief Converts Mavlink messages to bytes and adds them to the txToGroundByteQueue.
     *
     * @param msg The Mavlink message to be converted to bytes and added to the txToGroundByteQueue.
     * @param txToGroundByteQueue The CircularBuffer containing the bytes to be sent to the ground
     * station.
     */
    void addMavlinkMsgToByteQueue(mavlink_message_t &msg,
                                          CircularBuffer &txToGroundByteQueue);
};

#endif  // MAVLINKTRANSLATOR_H
