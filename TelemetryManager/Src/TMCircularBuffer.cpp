
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size): CircularBuffer(buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer *buf): CircularBuffer(*buf) {
    // Constructor
}

TMCircularBuffer::~TMCircularBuffer() {
    // Destructor
}

TMCircularBuffer::MAVLinkByte TMCircularBuffer::dequeue() { return 0; }

void TMCircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
}

int TMCircularBuffer::lastFullMessageEndIndex() {
    /*
    Rahul: This one is a bit tricky cause you need to know the structure of the MAVLink message. 
    I can help you with this one if you want.
    */
    return -1;
}

int TMCircularBuffer::currentIndex() { return -1; }
