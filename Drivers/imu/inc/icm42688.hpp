// icm42688.hpp

#ifndef ICM42688_HPP
#define ICM42688_HPP

#include "imu_driver.hpp"
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define GYRO_CALIBRATION_DATA_BUFFER_SIZE 14
#define GYRO_CALIBRATION_RAW_DATA_BUFFER_SIZE 7
#define RAW_DATA_BUFFER_SIZE 7

class ICM42688 : public IMUDriver {
    public:
        ICM42688(SPI_HandleTypeDef * SPI_HANDLE, GPIO_TypeDef * CS_GPIO_PORT, uint16_t CS_PIN);

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
        * @param count -> number of bytes to be retrieved
        * @param dest -> array to be populated with raw data
         * 
        * @return none
         */
        void readRegister(uint8_t sub_address, uint8_t count, uint8_t * dest);
        
        /**
        * Communicate with IMU via SPI to write data onto IMU
         * 
        * @param sub_address -> memory address of byte to be written on
        * @param data -> data to be written onto IMU
         * 
        * @return none
         */
        void writeRegister(uint8_t sub_address, uint8_t data);

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
        * @param fssel -> full-scale selection for the gyro
         * 
        * @return none
         */
        void setGyroFS(uint8_t fssel);

        //Constants (can be found in ICM42688 documentation https://www.cdiweb.com/datasheets/invensense/ds-000347-icm-42688-p-v1.2.pdf)
        const uint8_t REG_BANK_SEL = 0x76;
        const uint8_t UB0_REG_DEVICE_CONFIG = 0x11;
        const uint8_t UB0_REG_PWR_MGMT0 = 0x4E;
        const uint8_t UB0_REG_TEMP_DATA1 = 0x1D;

        //Variables used in gyro calibration
        float gyro_scale = 0;
        uint8_t current_fssel = 0;
        uint8_t gyroFS = 0;
        float gyroBD[3] = {0, 0, 0};
        float gyrB[3] = {0, 0, 0};
        float gyr[3] = {0, 0, 0};
        uint8_t gyro_buffer[GYRO_CALIBRATION_DATA_BUFFER_SIZE];
        int16_t raw_meas_gyro[GYRO_CALIBRATION_RAW_DATA_BUFFER_SIZE];

        //Used to hold raw IMU data
        int16_t raw_meas[RAW_DATA_BUFFER_SIZE];

        SPI_HandleTypeDef * SPI_HANDLE;
        GPIO_TypeDef * CS_GPIO_PORT;
        uint16_t CS_PIN;
};

#endif //ICM42688_HPP
