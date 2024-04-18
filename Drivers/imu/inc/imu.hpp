/**
 * IMU class definition / stub
 * Designed to be extended by IMU devices
 * Created by: Anthony Luo on 2024-04-18
 * Maintained by:
 * Last updated:
*/

#ifndef IMU_HPP
#define IMU_HPP

#include "CommonDataTypes.hpp"

/************************************************
 * Definitions
 ************************************************/

struct IMUData_t {
    float gyrx, gyry, gyrz;
    float accx, accy, accz;
    float magx, magy, magz;

    bool is_data_new;
    SensorErrorCode sensor_status; // TODO: determine if we need this?
};

/************************************************
 * Prototypes
 ************************************************/
class IMU{
    public:
        // Leaving the constructor to be elsewhere.... don't know how to handle passing SPI/I2C handles

        /**
         * Re-calibrates sensor. Useful if we need to hard re-set an IMU in-flight or something.
        */
        virtual SensorErrorCode calibrate() = 0;
        /**
         * Begins IMU data sampling. TODO: determine how this is determined
         * Data should be stored in a buffer and accessible at any time.
         * 
        */
        virtual void beginMeasuring() = 0;

        /**
         * Retrieves newest data stored by IMU.
         * TODO: determine if we want to return error codes or if we want to set the struct
        */
        virtual SensorErrorCode getResult(IMUData_t &data) = 0;
    private:
        IMUData_t latest_data;
};