

#include "GroundStationComms.hpp"

// ** Implement sendToGroundStation first ** 

void GroundStationComms::sendToGroundStation(CircularBuffer &transmissionBuffer)
{
    // Send the bytes in transmissionBuffer to the ground station via RFD900
}

void GroundStationComms::receiveFromGroundStationISR()
    {

        // if GSC.DMAReceiveBuffer has enough space for the new data add it
        //otherwise discard the data


        //end of ISR
    }