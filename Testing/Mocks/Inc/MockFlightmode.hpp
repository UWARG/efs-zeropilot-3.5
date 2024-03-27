#include "flightmode.hpp"

class MockFlightmode : public AM::Flightmode{
public:
    MockFlightmode(){}
    AM::AttitudeManagerInput run(const AM::AttitudeManagerInput& input) {
        return input;
    };
    void updatePid(){};
};