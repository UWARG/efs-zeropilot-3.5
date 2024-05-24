#include <gmock/gmock.h>
#include "TMCircularBuffer.hpp"

class MockTMCircularBuffer : public TMCircularBuffer
{
    public:

    MOCK_METHOD(unsigned char, dequeue, ());
    MOCK_METHOD(void, enqueue, (unsigned char));
    MOCK_METHOD(int, lastFullMessageEndIndex, ());
    MOCK_METHOD(int, current_index, ());
};