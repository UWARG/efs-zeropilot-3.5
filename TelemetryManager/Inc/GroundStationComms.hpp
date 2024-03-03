//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#include "CircularBuffer.hpp"
#ifndef GROUNDSTATIONCOMMS_H
#define GROUNDSTATIONCOMMS_H

/**
 * @brief
 */
class GroundStationComms
{

public:
    /*
    * When the DMA interrupt is triggered the data should be stored in the DMAReceiveBuffer
    * if there is space
    */
    CircularBuffer DMAReceiveBuffer;

    // low priority mavlink bytes to be sent to the ground station.
    CircularBuffer lowPriorityTransmitBuffer;

    // high priority mavlink bytes to be sent to the ground station.
    CircularBuffer highPriorityTransmitBuffer;

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
    virtual void sendToGroundStation(CircularBuffer &transmissionBuffer);

    /**
     * @brief This is the Interrupt Service Routine (ISR) for when the RFD 900 receives data from
     * the ground station and stores bytes from the transmission into the GSC.DMAReceiveBuffer if
     * there is space. Otherwise the data is discarded.
     */
    virtual void receiveFromGroundStationISR();
};

#endif // GROUNDSTATIONCOMMS_H
