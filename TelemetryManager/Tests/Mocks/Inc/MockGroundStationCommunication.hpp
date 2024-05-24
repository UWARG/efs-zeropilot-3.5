#include <gmock/gmock.h>
#include "GroundStationCommunication.hpp"
#include "TMCircularBuffer.hpp"

class MockGroundStationCommunication : public GroundStationCommunication 
{
    public:
 
    MOCK_METHOD(void, transmit, (TMCircularBuffer buffer));
    MOCK_METHOD(void, receiveInterruptServiceRoutine, ());

};

