#include "MavlinkTranslator.hpp"

MavlinkTranslator::MavlinkTranslator()
{
    // Constructor
    this->decoder = MavlinkDecoder();
    this->encoder = MavlinkEncoder();
    
}

MavlinkTranslator::~MavlinkTranslator()
{
    // Destructor
}

void MavlinkTranslator::bytesToMavlinkMsg(TMCircularBuffer &rxFromGroundByteQueue)
{
    // Take bytes in rxFromGroundByteQueue and convert them to Mavlink messages
    decoder.parseBytesToMavlinkMsgs(rxFromGroundByteQueue.getBuffer(), rxFromGroundByteQueue.getSize());

}

void MavlinkTranslator::addMavlinkMsgToByteQueue(mavlink_message_t &msg, TMCircularBuffer &txToGroundByteQueue)
{
    // Take Mavlink messages in txToGroundByteQueue and convert them to bytes

}