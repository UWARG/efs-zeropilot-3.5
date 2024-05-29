#include "gmock/gmock.h"
#include "MavlinkTranslator.hpp"
#include "TMCircularBuffer.hpp"
#include "Official_Mavlink_2_Library/common/mavlink.h"

class MockMavlinkTranslator : MavlinkTranslator
{
    MOCK_METHOD(void, bytesToMavlinkMsg, (TMCircularBuffer buffer));
    MOCK_METHOD(void, addMavlinkMsgToByteQueue, (mavlink_message_t msg, TMCircularBuffer buffer));
};
