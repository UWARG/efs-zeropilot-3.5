//
// Created by Yarema Dzulynsky on 2023-08-24.
//


#include "CircularBuffer.hpp"

    int CircularBuffer::remainingMemory()
    {
        return -1;
    }

    CircularBuffer::MAVLinkByte CircularBuffer::dequeue()
    {
        return 0;
    }

    void CircularBuffer::enqueue(MAVLinkByte byte)
    {
        // Enqueue the byte
    }

    int CircularBuffer::lastFullMessageEndIndex()
    {
        return -1;
    }

    int CircularBuffer::currentIndex()
    {
        return -1;
    }

    