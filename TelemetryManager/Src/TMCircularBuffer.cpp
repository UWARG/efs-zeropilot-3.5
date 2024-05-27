
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size): CircularBuffer(*buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer* buf): CircularBuffer(*buf) {
    // Constructor
}

TMCircularBuffer::~TMCircularBuffer() {
    // Destructor
    delete[] buf;
    buf = nullptr;
 }

TMCircularBuffer::bool TMCircularBuffer::dequeue(MAVLinkByte &byte) {
    if(getNumAvailBytes() - size_  == 0){
        return false;
    else{
        byte = buf[(readPtr_)];
        readPtr_ = (readPtr_ +1) % size_;
        --size_;
        return true;
    }


  }

void TMCircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
    if(!isFull()){
        buf_[writePtr_ % size_] = byte;
        writePtr_++;
    }
}

int TMCircularBuffer::bytesUntilLastMessageEnd() {
    /*
    Rahul: This one is a bit tricky because you need to know the structure of the MAVLink message. 
    I can help you with this one if you want.
    */
    uint16_t index = readPtr_;
    uint16_t count = 0;
    while(count < size_){
        if(buf_[index] ==0xFD){
            return index;
        }
        index = (index + 1) % size_;
        count ++;
    }
    return -1;
}

int TMCircularBuffer::currentIndex() { return  readPtr_; }
