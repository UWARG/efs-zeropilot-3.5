// icm42688.hpp

#ifndef ICM42688_HPP
#define ICM42688_HPP

#include "imu_driver.hpp"
#include <stdint.h>
#include "stm32l5xx_hal.h"
#include "stm32l5xx_hal_gpio.h"
#include <stm32l552xx.h>

#define GYRO_CALIBRATION_DATA_BUFFER_SIZE 14    //Size of buffer to store raw data from IMU, as there are 14 registers to be read from
#define RAW_MEAS_BUFFER_SIZE 7                  //Size of buffer to store raw measurements from IMU (3 accel, 3 gyrop, 1 temp)

class ICM42688 : public IMUDriver {
    public:
        ICM42688(SPI_HandleTypeDef * spi_handle, GPIO_TypeDef * cs_gpio_port, uint16_t cs_pin);

        /**
        * Sets up the IMU's initial starting conditions by setting 
        * the cs pin and calling reset(), setLowNoiseMode(), and calibrate()
         *
        * @return none
         */
        void beginMeasuring() override;
        
        /**
        * Calibrates the gyroscope. Algorithm adapted from https://github.com/finani/ICM42688/tree/master/src
         *
        *  @return none 
         */
        void calibrate() override;
        
        /**
        * Retrieve and process IMU accelerometer and gyroscope data into m/s and deg/s
         *
        * @param data_buffer -> array used to store and process raw data from IMU
         * 
        * @return IMUData_t structure consisting of accelerometer and gyroscope data in m/s and deg/s
         */
        IMUData_t getResult(uint8_t * data_buffer) override;

    private:
        /**
        * Communicate with IMU via SPI to read raw data
         * 
        * @param sub_address -> memory address of starting byte to be retrieved
        * @param num_bytes_to_retrieve -> number of bytes to be retrieved
        * @param destination -> array to be populated with raw data
         * 
        * @return none
         */
        void readRegister(uint8_t sub_address, uint8_t num_bytes_to_retrieve, uint8_t * destination);
        
        /**
        * Communicate with IMU via SPI to write data onto IMU
         * 
        * @param sub_address -> memory address of byte to be written on
        * @param data_to_imu -> data to be written onto IMU
         * 
        * @return none
         */
        void writeRegister(uint8_t sub_address, uint8_t data_to_imu);

        /**
        * Resets the IMU's device configurations. Necessary to initilize IMU
         * 
        * @return none
         */
        void reset();

        /**
        * Sets the IMU's power management settings to low noise mode. Necessary to initilaize IMU
         * 
        * @return none
         */
        void setLowNoiseMode();

        /**
        * Sets the IMU register address bank to a value of 0 - 4
         *
        * @param bank -> Bank number (0 - 4)
         * 
        * @return none
         */
        void setBank(uint8_t bank);

        /**
        * Configures the full-scale range of the gyroscope on the IMU. Adapted from https://github.com/finani/ICM42688/tree/master/src
         * 
        * @param fs_sel -> full-scale selection for the gyro
         * 
        * @return none
         */
        void setGyroFS(uint8_t fs_sel);

        //Variables used in gyro calibration
        float gyroScale_ = 0;       //gyro scale factor
        uint8_t currentFSSel_ = 0;  //current full-scale selection for the gyro
        uint8_t gyroFS_ = 0;
        float gyroBD_[3] = {0, 0, 0};
        float gyrB_[3] = {0, 0, 0};
        float gyr_[3] = {0, 0, 0};
        uint8_t gyro_buffer_[GYRO_CALIBRATION_DATA_BUFFER_SIZE];
        int16_t raw_meas_gyro_[RAW_MEAS_BUFFER_SIZE];

        //Used to hold raw IMU data
        int16_t raw_meas_[RAW_MEAS_BUFFER_SIZE];

        SPI_HandleTypeDef * spiHandle_;
        GPIO_TypeDef * csGpioPort_;
        uint16_t csPin_;
};

#endif //ICM42688_HPP
