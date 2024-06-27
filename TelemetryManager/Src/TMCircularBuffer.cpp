
#include "TMCircularBuffer.hpp"

TMCircularBuffer::TMCircularBuffer(uint8_t* buf, uint16_t size): CircularBuffer(*buf, size) {
    // Constructor
}

TMCircularBuffer::TMCircularBuffer(CircularBuffer* buf ): CircularBuffer(*buf) {
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
        return true;
    }
     return false;
}

int TMCircularBuffer::bytesUntilLastMessageEnd() {
    /*
    Rahul: This one is a bit tricky because you need to know the structure of the MAVLink message. 
    I can help you with this one if you want.
    */
   int index = -1;
   uint16_t count = 0;
   uint16_t size = size_;
   while(count < size){
      if( buf_[index] == 0xFD){
        return index;
      }
      
      index = (index + 1) % size;
      count ++;
   }
   return index;
   
}

uint16_t TMCircularBuffer::currentIndex() {
    return readPtr_;
 }
