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

// DO NOT NEED FOR M2
void MavlinkTranslator::bytesToMavlinkMsg(TMCircularBuffer &rxFromGroundByteQueue)
{
    // Take bytes in rxFromGroundByteQueue and convert them to Mavlink messages

    // decoder.parseBytesToMavlinkMsgs(rxFromGroundByteQueue.buf_, rxFromGroundByteQueue.getSize());

}

// DO NOT NEED FOR M2
void MavlinkTranslator::addMavlinkMsgToByteQueue(mavlink_message_t &msg, TMCircularBuffer &txToGroundByteQueue)
{
    // Take Mavlink messages in txToGroundByteQueue and convert them to bytes
    
        
        //WHAT WILL WE DO IF THE TXTOGROUND BYTE QUEUE CANNOT FIT THE MESSAGE?
     
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

        std::size_t bufferLen = mavlink_msg_to_send_buffer(buffer, &msg);
        
        for (std::size_t i = 0; i < bufferLen; i++) {
            txToGroundByteQueue.enqueue(buffer[i]);
        }

}