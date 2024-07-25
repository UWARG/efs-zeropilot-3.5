/**
 * @file TMCircularBuffer.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version Milestone 2
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure
 * @author Rahul Ramkumar: implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TMCIRCULARBUFFER_H
#define TMCIRCULARBUFFER_H
#define MAX_MAVLINK_MSG_SIZE 280

#include "circular_buffer.hpp"

class TMCircularBuffer : public CircularBuffer {
   public:
    using MAVLinkByte = unsigned char;  // not 100% sure if this is the right type
    long messagesInQueue = 0;
    /**
     * @brief Construct a new Circular Buffer object. Do whatever needs to be done here.
     * @param buf The uint8_t buffer to be used by the TMCircularBuffer
     * @param size The size of the buffer
     */
    TMCircularBuffer(uint8_t* buf, uint16_t size);

    /**
     * @brief Construct a new Circular Buffer object. Do whatever needs to be done here.
     * @param buf The CircularBuffer base class object to be used by the TMCircularBuffer
     *
     */
    TMCircularBuffer(CircularBuffer* buf);

    /**
     * @brief Destroy and cleanup memory (everything should be static anyways). Do whatever
     * needs to be done here.
     *
     */
    ~TMCircularBuffer();

    /**
     * @brief Dequeue a byte from the queue
     *
     * @param success A pointer to a boolean that will be set to true if the dequeue was successful
     * @return MAVLinkByte The byte that was dequeued
     */
    MAVLinkByte dequeue(bool* success = nullptr);

    /**
     * @brief Enqueue a byte into the queue
     *
     * @param byte The byte to be enqueued
     * 
     * @return bool True if the byte was enqueued successfully, false otherwise
     */
    bool enqueue(MAVLinkByte byte);

    /**
     * @brief 
     * 
     * @param bytes The bytes to be enqueued
     * @param size The number of bytes to be enqueued
     * @return bool True if the bytes were enqueued successfully, false otherwise
     */
    bool enqueue(MAVLinkByte* bytes, uint16_t size);

    /**
     * @brief Get the number of bytes until the end of the first full message in the queue
     * determined by the end flag in the MAVLink message. This is used to make sure we only
     * send full messages.
     * 
     * @param success A pointer to a boolean that will be set to true if the operation was successful
     * and false if it was not.
     *
     * @return int The number of bytes until the end of the first full message in the queue.
     * in the MAVLink message.
     *
     */
    int bytesUntilMessageEnd(bool* success = nullptr);

    /**
     * @brief Returns the index of the current byte in the queue.
     *
     * @return int The index of the current byte in the queue.
     */
    uint16_t currentIndex();
};

#endif
