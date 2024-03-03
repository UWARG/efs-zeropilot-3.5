//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

class CircularBuffer
{

    using MAVLinkByte = unsigned char; // not 100% sure if this is the right type

public:

    /**
     * @brief Provides the number of bytes available in the queue
     * 
     * @return int The number of bytes available in the queue.
     */
    virtual int remainingMemory();

    /**
     * @brief Dequeue a byte from the queue
     *
     * @return MAVLinkByte The byte that was dequeued
     */
    virtual MAVLinkByte dequeue();

    /**
     * @brief Enqueue a byte into the queue
     * 
     * @param byte The byte to be enqueued
     */
    virtual void enqueue(MAVLinkByte byte);
};

#endif // CIRCULARQUEUE_H
