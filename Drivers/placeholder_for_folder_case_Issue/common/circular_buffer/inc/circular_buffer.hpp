#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <stdint.h>

class CircularBuffer {
    public:
        /** @brief Constructor for the CircularBuffer class
        *
        *  CircularBuffer will manage a stack-allocated byte array as a
        *  circular buffer.
        *
        *  @param buf The byte array to be managed.
        *  @param size Size of the byte array. This will be the size of
        *              the circular buffer.
        */
        CircularBuffer(uint8_t* buf, uint16_t size);

        /** @brief Read the value of a byte in the circular buffer
        *          without incrementing the read pointer
        *
        *  @param res The byte value to be returned. Passed by reference.
        *  @param index The index of the byte.
        *  @return False if the index is out of bounds.
        */
        bool peek(uint8_t& res, uint16_t index);

        /** @brief Read bytes from the circular buffer and increment the
        *          read pointer.
        *
        *  @param buf The buffer to read the bytes into.
        *  @param dist Number of bytes to be read.
        *  @return False if the number of bytes to be read exceed
        *          the number of available bytes.
        */
        bool read(uint8_t* buf, uint16_t dist);

        /** @brief Write a byte into the circular buffer and increment
        *          the write pointer.
        *
        *  @param byte The byte to write.
        *  @return False if the circular buffer is already full.
        */
        bool write(uint8_t byte);

        /** @brief Check if the circular buffer is full.
        *
        *  @return True if the circular buffer is full.
        */
        bool isFull();

        /** @brief Check the number of bytes available to be read.
        *
        *  @return The number of bytes available to be read.
        */
        uint16_t getNumAvailBytes();

    private:
        uint8_t* buf_;
        uint16_t size_;
        uint16_t writePtr_ = 0;
        uint16_t readPtr_ = 0;
};


#endif