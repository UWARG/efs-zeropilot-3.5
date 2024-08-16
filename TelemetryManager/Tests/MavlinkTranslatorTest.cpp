#include <gtest/gtest.h>

#include <MavlinkTranslator.hpp>

TEST(MavlinkTranslatorTest, EncodeThenDecodeTest) {
    MavlinkTranslator translator;

    mavlink_message_t globalPositionInt;
    mavlink_message_t attitude;
    mavlink_msg_global_position_int_pack(0, 0, &globalPositionInt, 0, 1, 2, 3, 0, 7, 8, 9, 0);
    mavlink_msg_attitude_pack(0, 0, &attitude, 0, 4, 5, 6, 0, 0, 0);

    uint8_t encoderBuffer[2 * MAVLINK_MAX_PACKET_LEN] = {0};
    TMCircularBuffer buffer(encoderBuffer, 2 * MAVLINK_MAX_PACKET_LEN);

    translator.addMavlinkMsgToByteQueue(globalPositionInt, buffer);
    translator.addMavlinkMsgToByteQueue(attitude, buffer);

    // for (size_t i = 0; i < actDataSize; i++) {
    //     buffer.enqueue(encoderBuffer[i]);
    // }

    int expectedNumberOfMessagesDecoded =
        1;  // should be one not two because we want to wait for the next message to determine the
            // start bit and therefore determine when the second message ends

    translator.bytesToMavlinkMsg(buffer);

    ASSERT_EQ(translator.decoder.messagesHandledSuccessfully, expectedNumberOfMessagesDecoded);
    ASSERT_EQ(translator.decodedMessages, expectedNumberOfMessagesDecoded);
}

TEST(MavlinkTranslatorTest, ParseBytesToMavlinkMsgsTest) {
    MavlinkTranslator translator;
    int expectedNumberOfMessagesDecoded = 1;

    mavlink_message_t globalPositionIntMsg, attitudeMsg;
    uint8_t globalPositionIntBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
    uint8_t attitudeBuffer[MAVLINK_MAX_PACKET_LEN] = {0};

    mavlink_msg_global_position_int_pack(0, 0, &globalPositionIntMsg, 0, 1, 2, 3, 0, 7, 8, 9, 0);
    size_t globalPositionIntBufferLen =
        mavlink_msg_to_send_buffer(globalPositionIntBuffer, &globalPositionIntMsg);

    mavlink_msg_attitude_pack(0, 0, &attitudeMsg, 0, 4, 5, 6, 0, 0, 0);
    size_t attitudeBufferLen = mavlink_msg_to_send_buffer(attitudeBuffer, &attitudeMsg);

    uint8_t outputBuffer[MAVLINK_MAX_PACKET_LEN * 2] = {0};

    memcpy(outputBuffer, globalPositionIntBuffer, globalPositionIntBufferLen);

    memcpy(outputBuffer + globalPositionIntBufferLen, attitudeBuffer, attitudeBufferLen);

    TMCircularBuffer buffer(outputBuffer, globalPositionIntBufferLen + attitudeBufferLen);

    buffer.enqueue(outputBuffer, globalPositionIntBufferLen + attitudeBufferLen);

    translator.bytesToMavlinkMsg(buffer);

    ASSERT_EQ(translator.decoder.messagesHandledSuccessfully, expectedNumberOfMessagesDecoded);
    ASSERT_EQ(translator.decodedMessages, expectedNumberOfMessagesDecoded);
}

TEST(MavlinkTranslatorTest, AddMavlinkMsgToByteQueueTest) {
    MavlinkTranslator translator;
    uint8_t _buffer[256];
    TMCircularBuffer buffer(_buffer, 256);

    // Create a mavlink message
    mavlink_message_t msg;
    mavlink_msg_global_position_int_pack(0, 0, &msg, 0, 1, 2, 3, 0, 7, 8, 9, 0);

    // Add the message to the byte queue
    translator.addMavlinkMsgToByteQueue(msg, buffer);

    // Create a buffer to hold the message data
    uint8_t expectedBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
    size_t expectedBufferLen = mavlink_msg_to_send_buffer(expectedBuffer, &msg);

    // Verify the data in the byte queue matches the expected buffer
    for (size_t i = 0; i < expectedBufferLen; i++) {
        ASSERT_EQ(buffer.dequeue(), expectedBuffer[i]);
    }
}