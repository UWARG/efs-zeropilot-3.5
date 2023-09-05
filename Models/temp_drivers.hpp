#ifndef ZPSW3_TEMP_DRIVERS_HPP
#define ZPSW3_TEMP_DRIVERS_HPP

//TODO: Remove this file when drivers and flightmodes are implemented

#include "ZP_D_PWMChannel.hpp"

class TempDSHOTDriver : public MotorChannel
{
    public:
        TempDSHOTDriver() {}
        void set(uint8_t percent){}
};
class RCInputDriver{};
class TempPPMDriver : public RCInputDriver{};
class TempSBusDriver : public RCInputDriver{};
class GPSDriver{};
class TempNEOM8Driver : public GPSDriver{};
class otherGPSDriver : public GPSDriver{};

class ExampleFlightmode1 : public AM::Flightmode{
    public:
    ExampleFlightmode1(){}
    //TODO: Implement control algorithm functions in AM
    void run();
    void updatePid();
};

class ExampleFlightmode2 : public AM::Flightmode{
    public:
    ExampleFlightmode2(){}
    void run();
    void updatePid();
};


#endif // ZPSW3_TEMP_DRIVERS_HPP