#ifndef RC_RECEIVER_DATA_TYPES_H
#define RC_RECEIVER_DATA_TYPES_H

#include <cstdint>
#include <iostream>

#define SBUS_INPUT_CHANNELS	16

typedef struct SBus{
    uint16_t ch[SBUS_INPUT_CHANNELS]; //value from 192 - 1792, can be little off
    bool lost_frame;
    bool failsafe;
    bool ch17, ch18;
    bool new_data;
} SBus_t;

/*  a struct for control signal channel mapping and attribute values*/
/*  for now, the value range is 0 to 100 float*/
struct RCControl{
    float ControlSignals[16];
    
    float &roll = ControlSignals[0];
    float &pitch = ControlSignals[1];
    float &throttle = ControlSignals[2];
    float &yaw = ControlSignals[3];
    float &arm = ControlSignals[4];
    float &aux1 = ControlSignals[5];
    float &aux2 = ControlSignals[6];
    float &aux3 = ControlSignals[7];
    float &aux4 =  ControlSignals[8];
    float &aux5 = ControlSignals[9];
    float &aux6 = ControlSignals[10];
    float &aux7 = ControlSignals[11];
    float &aux8 = ControlSignals[12];
    float &aux9 = ControlSignals[13];
    float &aux10 = ControlSignals[14];
    float &aux11 = ControlSignals[15];

    float &operator[] (int i) { return ControlSignals[i]; }

    RCControl operator=(const RCControl& other){
        std::copy(other.ControlSignals, other.ControlSignals + SBUS_INPUT_CHANNELS, this->ControlSignals);
        return *this;
    }

    /*  initial values*/
    RCControl()
    {
        roll = 50.0f;
        pitch = 50.0f;
        throttle = 0.0f;
        yaw = 50.0f;
        arm = 0.0f;
        aux1 = 0.0f;
        aux2 = 0.0f;
        aux3 = 0.0f;
        aux4 = 0.0f;
        aux5 = 0.0f;
        aux6 = 0.0f;
        aux7 = 0.0f;
        aux8 = 0.0f;
        aux9 = 0.0f;
        aux10 = 0.0f;
        aux11 = 0.0f;
    }
};

#endif