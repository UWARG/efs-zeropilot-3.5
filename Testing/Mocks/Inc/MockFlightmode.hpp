#include "flightmode.hpp"

class MockFlightmode : public AM::Flightmode{
public:
    MockFlightmode(){}
    AM::AttitudeManagerInput run(const AM::AttitudeManagerInput& input) {
        return input;
    };
    void updatePid(){};
    void updatePidGains() {};
    void updateControlLimits(ControlLimits_t limits) {};
};