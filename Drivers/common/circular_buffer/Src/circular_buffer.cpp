#include "circular_buffer.hpp"
#include <cstring>

CircularBuffer::CircularBuffer(uint8_t* buf, uint16_t size) {
    buf_ = buf;
    size_ = size;
}

bool CircularBuffer::peek(uint8_t& byte, uint16_t index) {
    bool success = true;

    if (readPtr_ + index >= writePtr_) {
        success = false;
    } else {
        byte = buf_[(readPtr_ + index) % size_];
    }

    return success;
}

bool CircularBuffer::read(uint8_t* res, uint16_t dist) {
    bool success = true;

    if (dist + readPtr_ > writePtr_) {
        success = false;
    } else {
        if ((readPtr_ % size_) + dist >= size_) {
            // Two memcpys needed because we need to loop back to the
            // beginning of the buffer.
            uint16_t dist_to_end = size_ - (readPtr_ % size_);
            std::memcpy(res, &buf_[readPtr_ % size_], dist_to_end);
            readPtr_ += dist_to_end;
            dist -= dist_to_end;

            std::memcpy(&res[dist_to_end], &buf_[readPtr_ % size_], dist);
            readPtr_ += dist;
        } else {
            // One memcpy needed.
            std::memcpy(res, &buf_[readPtr_ % size_], dist);
            readPtr_ += dist;
        }
    }

    return success;
}

bool CircularBuffer::isFull() {
    // The buffer is full when the writePtr_ wraps around to the
    // readPtr_, so it's ahead of the readPtr_ by value, but overlaps
    // the readPtr_ by index aftering modding by the buffer size_.
    return (writePtr_ != readPtr_) &&
           (writePtr_ % size_ == readPtr_ % size_);
}

uint16_t CircularBuffer::getNumAvailBytes() {
    return writePtr_ - readPtr_;
}

bool CircularBuffer::write(uint8_t byte) {
    bool success = true;

    if (isFull()) {
        success = false;
    } else {
        buf_[writePtr_ % size_] = byte;
        writePtr_++;
    }

    return success;
}