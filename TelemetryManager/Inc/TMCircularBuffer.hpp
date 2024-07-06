/**
 * @file TMCircularBuffer.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure
 * @author Rahul Ramkumar: implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TMCIRCULARBUFFER_H
#define TMCIRCULARBUFFER_H
#define MAX_MAVLINK_MSG_SIZE 280

#include "/Users/yaremadzulynsky/Software/Languages/C++/WARG/efs-zeropilot-3.5/Drivers/common/circular_buffer/inc/circular_buffer.hpp"

class TMCircularBuffer : public CircularBuffer {
   public:
    using MAVLinkByte = unsigned char;  // not 100% sure if this is the right type
    /**
     * @brief Construct a new Circular Buffer object. Do whatever needs to be done here.
     *
     */
    TMCircularBuffer(uint8_t* buf, uint16_t size);

    /**
     * @brief Construct a new Circular Buffer object. Do whatever needs to be done here.
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
     */
    bool enqueue(MAVLinkByte byte);

    bool enqueue(MAVLinkByte* bytes, uint16_t size);

    /**
     * @brief Get the number of bytes until the end of the last full message in the queue
     * determined by the end flag in the MAVLink message. This is so if we have a partial
     * message in the queue because an ISR was triggered while we were in the middle of
     * enqueuing a message, we only send completed messages and keep the partial message to be
     * finished after the ISR. These partial messages once filled will be sent during the next
     * transmission.
     *
     * @return int The index of the last full message in the queue determined by the end flag
     * in the MAVLink message.
     *
     */
    int bytesUntilLastMessageEnd(bool* success = nullptr);

    /**
     * @brief Returns the index of the current byte in the queue. This is useful for when we want to
     * avoid sending partial messages, as we know the index of the end of the last complete message.
     * Therefore, we can check if the current byte is just before the last complete message and if
     * so, we can avoid sending it.
     *
     * @return int The index of the current byte in the queue.
     */
    uint16_t currentIndex();
};

#endif
