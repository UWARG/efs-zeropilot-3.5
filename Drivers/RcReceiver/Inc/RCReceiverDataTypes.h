#ifndef RC_RECEIVER_DATA_TYPES_H
#define RC_RECEIVER_DATA_TYPES_H

#include <stdint.h>

#define SBUS_INPUT_CHANNELS	16

typedef struct SBus{
    uint16_t ch[SBUS_INPUT_CHANNELS]; //value from 192 - 1792, can be little off
    bool lost_frame;
    bool failsafe;
    bool ch17, ch18;
    bool new_data;
} SBus_t;

//the background work that allows a variable name refer to the same memory to an array element
template<uint8_t Index, class T>
struct ControlRef
{
    operator T&() // allows: double d = object.Member;
    {
        return ((T*)(this))[Index];
    }

    T &operator=(T const &rhs) // allows: object.member = 1.0;
    {
        T &me = ((T*)(this))[Index];

        me = rhs;

        return me;
    }

    T* operator&() // allows: double *p = &object.Member;
    {
        return &((T*)(this))[Index];
    }

    bool operator<(T const &rhs) // allows: if(object.Member < 1.0)
    {
        return ((T*)(this))[Index] < rhs;
    }

};

/*  a struct for control signal channel mapping and attribute values*/
/*  for now, the value range is 0 to 100 float*/
struct RCControl{
    union{
        float ControlSignals[16];
        ControlRef<0, float> roll;
        ControlRef<1, float> pitch;
        ControlRef<2, float> throttle;
        ControlRef<3, float> yaw;
        ControlRef<4, float> arm;
        ControlRef<5, float> vtx;
        ControlRef<6, float> led;
        ControlRef<7, float> mode;
        ControlRef<8, float> nonconfigch9;
        ControlRef<9, float> nonconfigch10;
        ControlRef<10, float> nonconfigch11;
        ControlRef<11, float> nonconfigch12;
        ControlRef<12, float> nonconfigch13;
        ControlRef<13, float> nonconfigch14;
        ControlRef<14, float> nonconfigch15;
        ControlRef<15, float> nonconfigch16;
    };

    float &operator[] (int i) { return ControlSignals[i]; }

    /*  initial values*/
    RCControl()
    {
        ControlSignals[0] = 50.0f;
        ControlSignals[1] = 50.0f;
        ControlSignals[2] = 0.0f;
        ControlSignals[3] = 50.0f;
        ControlSignals[4] = 0.0f;
        ControlSignals[5] = 0.0f;
        ControlSignals[6] = 0.0f;
        ControlSignals[7] = 0.0f;
        ControlSignals[8] = 0.0f;
        ControlSignals[9] = 0.0f;
        ControlSignals[10] = 0.0f;
        ControlSignals[11] = 0.0f;
        ControlSignals[12] = 0.0f;
        ControlSignals[13] = 0.0f;
        ControlSignals[14] = 0.0f;
        ControlSignals[15] = 0.0f;
    }
};

#endif