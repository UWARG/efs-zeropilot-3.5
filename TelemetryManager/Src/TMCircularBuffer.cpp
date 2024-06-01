
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size): CircularBuffer(*buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer* buf): CircularBuffer(*buf) {
    // Constructor
}

TMCircularBuffer::~TMCircularBuffer() {
    // Destructor
   
 }

TMCircularBuffer::MAVLinkByte  TMCircularBuffer::dequeue() {
    MAVLinkByte* res;
    if(read(res,1)){
        return *res;
    }
    return 0;
}

bool TMCircularBuffer::enqueue(MAVLinkByte byte) {
    // Enqueue the byte
    if(write(byte)){
        index_ = (index_ + 1) % getSize();
        return true;
    }
     return false;
}

int TMCircularBuffer::bytesUntilLastMessageEnd(uint8_t* buf) {
    /*
    Rahul: This one is a bit tricky because you need to know the structure of the MAVLink message. 
    I can help you with this one if you want.
    */
   uint16_t index = -1;
   uint16_t count = 0;
   uint16_t size = getSize();
   while(count < size){
      if( buf[index] == 0xFD){
        return index;
      }
      
      index = (index + 1) % size;
      count ++;
   }
   return index;
   
}

int TMCircularBuffer::currentIndex() {
    return index_;
 }
