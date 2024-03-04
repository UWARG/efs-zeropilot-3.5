
#include "CircularBuffer.hpp"

CircularBuffer::CircularBuffer() {
    // Constructor
}

CircularBuffer::~CircularBuffer() {
    // Destructor
}

int CircularBuffer::remainingMemory() { return -1; }

CircularBuffer::MAVLinkByte CircularBuffer::dequeue() { return 0; }

void CircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
}

int CircularBuffer::lastFullMessageEndIndex() { return -1; }

int CircularBuffer::currentIndex() { return -1; }
