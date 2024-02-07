#include "AM_ControlAlgorithm.hpp"

class MockFlightmode : public AM::Flightmode{
    public:
    MockFlightmode(){}
    void run(){};
    void updatePid(){};
};