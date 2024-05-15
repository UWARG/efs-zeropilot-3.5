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
    
        
        //WE NEED TO SOMEHOW GET THE MESSAGE TYPE TO USE THE CORRECT PACK FUNCTION
        //
        mavlink_msg_##baseName##_pack(0, 0, &msg, __VA_ARGS__);
        std::size_t bufferLen = mavlink_msg_to_send_buffer(buffer, &msg);
        
        for (std::size_t i = 0; i < bufferLen; i++) {
            txToGroundByteQueue.enqueue(buffer[i]);
        }

}