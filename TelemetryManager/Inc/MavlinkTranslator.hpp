//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#include "CircularBuffer.hpp"
#include "official_mavlink_2_library/common/mavlink.h"
#ifndef MAVLINKTRANSLATOR_H
#define MAVLINKTRANSLATOR_H

class MavlinkTranslator
{
    /**
     * @brief Construct a new MavlinkTranslator object. Do whatever needs to be done here.
     * 
     */
    MavlinkTranslator();


public:
/**
 * @brief Ingests bytes from the rxFromGroundByteQueue and converts them to Mavlink messages. The Mavlink 
 * message, associated callbacks are then triggered.
 * 
 * @param rxFromGroundByteQueue The CircularBuffer containing the bytes received from the ground station.
 * waiting to be decoded.
 */
    virtual void bytesToMavlinkMsg(CircularBuffer &rxFromGroundByteQueue);

/**
 * @brief Converts Mavlink messages to bytes and adds them to the txToGroundByteQueue.
 * 
 * @param msg The Mavlink message to be converted to bytes and added to the txToGroundByteQueue.
 * @param txToGroundByteQueue The CircularBuffer containing the bytes to be sent to the ground station.
 */
    virtual void addMavlinkMsgToByteQueue(mavlink_message_t& msg, CircularBuffer &txToGroundByteQueue);

};

#endif // MAVLINKTRANSLATOR_H
