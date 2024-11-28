// imu_driver.hpp

#ifndef IMU_DRIVER_HPP
#define IMU_DRIVER_HPP

#include <stdint.h>

struct IMUData_t {
    float gyrx, gyry, gyrz;
    float accx, accy, accz;
};

class IMUDriver {
    public:
        /**
        * Initializes the IMU for sampling
         */
        virtual void beginMeasuring() = 0;
        
        /**
        * Calibrates IMU gyro sensor
         */
        virtual void calibrate() = 0;
        
        /**
        * Retrieves newest data stored by IMU
         */
        virtual IMUData_t getResult(uint8_t * data_buffer) = 0;

        virtual ~IMUDriver() {}
};

#endif  //IMU_DRIVER_HPP
