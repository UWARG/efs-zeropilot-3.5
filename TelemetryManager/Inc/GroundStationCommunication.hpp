/**
 * @file GroundStationCommunication.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure
 * @author Roni Kant: implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#include "TMCircularBuffer.hpp"
#include "drivers_config.hpp"
#ifndef GROUNDSTATIONCOMMUNICATION_H
#define GROUNDSTATIONCOMMUNICATION_H

/**
 * @brief This class is responsible for handling the communication between the ground station
 * and the drone. It is responsible for sending and receiving bytes from the ground station. It
 * acts as a layer of abstraction between the RFD 900 and the rest of the program. This is useful
 * because it allows us to define the behaviour of the RFD 900 send/receive function before its
 * implementation.
 */
class GroundStationCommunication {
   private:
    uint8_t internalBuffer_[RFD900_BUF_SIZE];

   public:
    /**
     * @brief Construct a new Ground Station Communication object. Do whatever needs to be done
     * here.
     * @param DMAReceiveBuffer A TMCircularBuffer created FROM the rfd900_circular_buffer in the drivers_config.hpp file. 
     * @param lowPriorityTransmitBuffer A uint8_t buffer to be used by a TM CircularBuffer
     * @param highPriorityTransmitBuffer A uint8_t buffer to be used by a TM CircularBuffer
     * @param length The length of the buffers.
     */
    GroundStationCommunication(TMCircularBuffer& DMAReceiveBuffer, uint8_t* lowPriorityTransmitBuffer, uint8_t* highPriorityTransmitBuffer, int length);
    ~GroundStationCommunication();


    /**
     * To make c++ happy while using rfd900_circular_buffer from drivers_config.hpp, we need to
     * create a pointer to the buffer. Then we dereference it.
     */
    // TMCircularBuffer* DMAReceiveBufferPtr = new TMCircularBuffer(rfd900_circular_buffer);

/*
     * When the DMA interrupt is triggered the data should be stored in the DMAReceiveBuffer
     * IF there is space.
     */
    TMCircularBuffer& DMAReceiveBuffer;

    // low priority/Non routine Mavlink bytes to be sent to the ground station.
    TMCircularBuffer lowPriorityTransmitBuffer;

    // high priority/Routine Mavlink bytes to be sent to the ground station.
    TMCircularBuffer highPriorityTransmitBuffer;

    /**
     * @brief This function sends data from a CircularBuffer to the ground station.
     *
     * This function is essentially a wrapper for the RFD 900 send function. This allows us
     * to define the behaviour of the RFD 900 send function before its implementation. This
     * is useful because we can then use this definition to write the rest of the program
     * regardless of the status of the RFD 900 send function implementation.
     *
     *
     * @param transmissionBuffer A CircularBuffer containing the data/MAVLink bytes to be sent
     * to the ground station
     */
    void transmit(TMCircularBuffer& transmissionBuffer);
};

#endif  // GROUNDSTATIONCOMMS_H
