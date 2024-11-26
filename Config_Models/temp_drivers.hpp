#ifndef ZPSW3_TEMP_DRIVERS_HPP
#define ZPSW3_TEMP_DRIVERS_HPP

//TODO: Remove this file when drivers and flightmodes are implemented

#include "ZP_D_MotorChannel.hpp"

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

#endif // ZPSW3_TEMP_DRIVERS_HPP