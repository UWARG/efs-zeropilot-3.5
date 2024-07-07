
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size) : CircularBuffer(buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer* buf) : CircularBuffer(*buf) {
    // Constructor
}

TMCircularBuffer::~TMCircularBuffer() {
    // Destructor
}

TMCircularBuffer::MAVLinkByte TMCircularBuffer::dequeue(bool* success) {
    MAVLinkByte res = 0;
    if (read(&res, 1)) {
        if (success != nullptr) {
            *success = true;
        }
        return res;
    }
    if (success != nullptr) {
        *success = false;
    }
    return 0;
}

bool TMCircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
    if (write(byte)) {
        return true;
    }
    return false;
}

bool TMCircularBuffer::enqueue(MAVLinkByte* bytes, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (!enqueue(bytes[i])) {
            return false;
        }
    }
    return true;
}

int TMCircularBuffer::bytesUntilLastMessageEnd(bool* success) {
    int index = 0;
    uint8_t byte = 0;
    uint16_t size = writePtr_ - readPtr_;

    while (index < size) {
        if (peek(byte, index)) {
            if (byte == 0xFD) {  // 0xFD is the start flag for a MAVLink message
                if (success != nullptr) {
                    *success = true;
                }
                if (index != 0) {
                    return index;
                }
            }
        }
        index++;
    }
    if (success != nullptr) {
        *success = false;
    }
    return index;
}

uint16_t TMCircularBuffer::currentIndex() { return readPtr_; }
