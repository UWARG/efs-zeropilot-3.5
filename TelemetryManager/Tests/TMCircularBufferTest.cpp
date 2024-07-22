#include <gtest/gtest.h>

#include "TMCircularBuffer.hpp"

TEST(TMCircularBufferTest, EnqueueDequeue) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);

    TMCircularBuffer::MAVLinkByte byte = 0x42;  // Arbitrary byte value
    EXPECT_TRUE(circularBuffer.enqueue(byte));
    EXPECT_EQ(circularBuffer.dequeue(), byte);
}

TEST(TMCircularBufferTest, DequeueEmptyBuffer) {
    static const uint16_t bufferSize = 0;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);
    bool success = true;
    circularBuffer.dequeue(&success);

    EXPECT_FALSE(success);
}

TEST(TMCircularBufferTest, BytesUntilLastMessageEndEmptyBuffer) {  // failing
    static const uint16_t bufferSize = 0;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);
    bool success = true;
    circularBuffer.bytesUntilMessageEnd(&success);

    EXPECT_FALSE(success);
}

TEST(TMCircularBufferTest, CurrentIndex) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);

    EXPECT_EQ(circularBuffer.currentIndex(), 0);  // Assuming initial index is 0
    TMCircularBuffer::MAVLinkByte byte = 0x42;    // Arbitrary byte value
    circularBuffer.enqueue(byte);
    circularBuffer.dequeue();
    EXPECT_EQ(circularBuffer.currentIndex(), 1);
}

TEST(TMCircularBufferTest, EnqueueDequeueMultipleBytes) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);
    TMCircularBuffer::MAVLinkByte bytes[] = {0x01, 0x02, 0x03, 0x04};
    for (auto byte : bytes) {
        EXPECT_TRUE(circularBuffer.enqueue(byte));
    }
    for (auto byte : bytes) {
        EXPECT_EQ(circularBuffer.dequeue(), byte);
    }
}

TEST(TMCircularBufferTest, EnqueueDequeueFullBuffer) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);

    for (uint16_t i = 0; i < bufferSize; ++i) {
        EXPECT_TRUE(circularBuffer.enqueue(static_cast<TMCircularBuffer::MAVLinkByte>(i)));
    }
    for (uint16_t i = 0; i < bufferSize; ++i) {
        EXPECT_EQ(circularBuffer.dequeue(), static_cast<TMCircularBuffer::MAVLinkByte>(i));
    }
}

TEST(TMCircularBufferTest, WrapAround) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);
    for (uint16_t i = 0; i < bufferSize / 2; ++i) {
        EXPECT_TRUE(circularBuffer.enqueue(static_cast<TMCircularBuffer::MAVLinkByte>(i)));
    }
    for (uint16_t i = 0; i < bufferSize / 2; ++i) {
        EXPECT_EQ(circularBuffer.dequeue(), static_cast<TMCircularBuffer::MAVLinkByte>(i));
    }
    for (uint16_t i = bufferSize / 2; i < bufferSize; ++i) {
        EXPECT_TRUE(circularBuffer.enqueue(static_cast<TMCircularBuffer::MAVLinkByte>(i)));
    }
    for (uint16_t i = bufferSize / 2; i < bufferSize; ++i) {
        EXPECT_EQ(circularBuffer.dequeue(), static_cast<TMCircularBuffer::MAVLinkByte>(i));
    }
}

TEST(TMCircularBufferTest, BytesUntilLastMessageEnd) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);
    // Assuming 0xFD marks the end of a message
    TMCircularBuffer::MAVLinkByte bytes[] = {0x01, 0x02, 0x03, 0x04, 0xFD, 0x05};
    for (auto byte : bytes) {
        circularBuffer.enqueue(byte);
    }
    EXPECT_EQ(circularBuffer.bytesUntilMessageEnd(), 4);  // The index of the second 0xFD
}

TEST(TMCircularBufferTest, EnqueueWhenFull) {
    static const uint16_t bufferSize = MAX_MAVLINK_MSG_SIZE;
    uint8_t buffer[bufferSize];
    TMCircularBuffer circularBuffer(buffer, bufferSize);

    for (uint16_t i = 0; i < bufferSize; ++i) {
        EXPECT_TRUE(circularBuffer.enqueue(static_cast<TMCircularBuffer::MAVLinkByte>(i)));
    }
    // Attempt to enqueue when buffer is full
    EXPECT_FALSE(circularBuffer.enqueue(0xFF));
}