#include <gtest/gtest.h>
#include "circular_buffer.hpp"

TEST(CircularBuffer, BasicTest) {
    const size_t bufSize = 8;
    uint8_t buf[bufSize];

    // Has space initially
    CircularBuffer circularBuf(buf, bufSize);
    EXPECT_EQ(circularBuf.isFull(), false) << "The circular buffer does not have space initially!";
    EXPECT_EQ(circularBuf.getNumAvailBytes(), 0) << "The circular buffer has data!";

    const size_t data1Size = 4;
    const uint8_t data1[data1Size] = {'A', 'B', 'C', 'D'};
    for (size_t i = 0; i < data1Size; ++i) {
        circularBuf.write(data1[i]);
    }

    EXPECT_EQ(circularBuf.isFull(), false) << "The circular buffer is full despite at half capacity!";
    EXPECT_EQ(circularBuf.getNumAvailBytes(), data1Size) << "The circular buffer doesn't have the right amount of data!";

    // Peek all elements
    uint8_t peekVal ,peekRes;
    for (size_t i = 0; i < data1Size; ++i) {
        peekRes = circularBuf.peek(peekVal, i);
        EXPECT_EQ(peekRes, 1) << "Couldn't peek byte #" << i;
        EXPECT_EQ(peekVal, data1[i]) << "Expected byte #" << i << " to be " << data1[i] << ". Got " << peekVal << ".";
    }

    // Write and read to end of buf
    for (size_t i = 0; i < data1Size; ++i) {
        circularBuf.write(data1[i]);
    }

    uint8_t readBuf1[data1Size];
    uint8_t readRes = 0;

    // Read first half
    readRes = circularBuf.read(readBuf1, data1Size);
    uint8_t cmpRes = memcmp(readBuf1, data1, data1Size);
    EXPECT_EQ(readRes, 1) << "Read failed (half buf read #1)";
    EXPECT_EQ(cmpRes, 0) << "Read data not equal to write data! (half buf read #1)";

    // Read second half
    readRes = circularBuf.read(readBuf1, data1Size);
    cmpRes = memcmp(readBuf1, data1, data1Size);
    EXPECT_EQ(readRes, 1) << "Read failed (half buf read #2)";
    EXPECT_EQ(cmpRes, 0) << "Read data not equal to write data! (half buf read #2)";

    // Write and read to half of buf for next text
    for (size_t i = 0; i < data1Size; ++i) {
        circularBuf.write(data1[i]);
    }
    circularBuf.read(readBuf1, data1Size);

    // Write and read past end
    const size_t data2Size = bufSize;
    uint8_t data2[data2Size] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    for (size_t i = 0; i < data2Size; ++i) {
        circularBuf.write(data2[i]);
    }

    EXPECT_EQ(circularBuf.isFull(), true) << "The circular buffer has space!";
    EXPECT_EQ(circularBuf.getNumAvailBytes(), data2Size) << "The circular buffer doesn't have the right amount of data!";

    uint8_t readBuf2[data2Size];
    readRes = circularBuf.read(readBuf2, data2Size);
    cmpRes = memcmp(readBuf2, data2, data2Size);
    EXPECT_EQ(readRes, 1) << "Read failed (full buf read)";
    EXPECT_EQ(cmpRes, 0) << "Read data not equal to write data! (full buf read)";

    // Read past write pointer
    uint8_t readByte = 'x';
    readRes = circularBuf.read(&readByte, 1);
    EXPECT_EQ(readRes, 0) << "Read succeeded when reading past wptr!";
    EXPECT_EQ(readByte, 'x') << "Read byte is changed!";

    EXPECT_EQ(circularBuf.isFull(), false) << "The circular buffer doesn't have space!";
    EXPECT_EQ(circularBuf.getNumAvailBytes(), 0) << "The circular buffer has data!";
}