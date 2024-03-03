#include "MavlinkTranslator.hpp"

void MavlinkTranslator::bytesToMavlinkMsg(CircularBuffer &rxFromGroundByteQueue)
{

    // Take bytes in rxFromGroundByteQueue and convert them to Mavlink messages
}

void MavlinkTranslator::addMavlinkMsgToByteQueue(mavlink_message_t &msg, CircularBuffer &txToGroundByteQueue)
{

    // Take Mavlink messages in txToGroundByteQueue and convert them to bytes
}