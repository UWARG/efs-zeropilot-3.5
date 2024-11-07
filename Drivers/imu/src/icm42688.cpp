// icm42688.cpp

#include "icm42688.hpp"
#include "stm32l5xx_hal.h"
#include "stm32l5xx_hal_gpio.h"
#include "stm32l5xx_hal_spi.h"
#include "spi.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

//Register Addresses
#define REG_BANK_SEL 0x76
#define UB0_REG_DEVICE_CONFIG 0x11
#define UB0_REG_PWR_MGMT0 0x4E
#define UB0_REG_TEMP_DATA1 0x1D

#define BIT_READ 0x80   //Read bit mask

#define NUM_GYRO_SAMPLES 1000   //Number of samples to take for calibration

//Scale Factors (refer to page 11-12 of https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000347-icm-42688-p-v1.6.pdf)
#define GYRO_SENSITIVITY_2000DPS 16.4           //Currently in Primary Use
#define GYRO_SENSITIVITY_1000DPS 32.8
#define GYRO_SENSITIVITY_500DPS 65.5
#define GYRO_SENSITIVITY_250DPS 131
#define GYRO_SENSITIVITY_125DPS 262
#define GYRO_SENSITIVITY_62_5DPS 524.3
#define GYRO_SENSITIVITY_31_25DPS 1048.6
#define GYRO_SENSITIVITY_15_625PS 2097.2

#define ACCEL_SENSITIVITY_2G 16384 
#define ACCEL_SENSITIVITY_4G 8192
#define ACCEL_SENSITIVITY_8G 4096
#define ACCEL_SENSITIVITY_16G 2048              //Currently in Primary Use

ICM42688::ICM42688(SPI_HandleTypeDef * spi_handle, GPIO_TypeDef * cs_gpio_port, uint16_t cs_pin) {
    spiHandle_ = spi_handle;
    csGpioPort_ = cs_gpio_port;
    csPin_ = cs_pin;
}

void ICM42688::readRegister(uint8_t sub_address, uint8_t num_bytes_to_retrieve, uint8_t * destination) {
    //Set read bit for register address
    uint8_t tx = sub_address | BIT_READ;

    //Dummy transmit and receive buffers
    uint8_t dummy_tx[num_bytes_to_retrieve];
    uint8_t dummy_rx;

    //Initialize values to 0
    memset(dummy_tx, 0, num_bytes_to_retrieve * sizeof(dummy_tx[0]));

    HAL_GPIO_WritePin(csGpioPort_, csPin_, GPIO_PIN_RESET);

    HAL_SPI_TransmitReceive(spiHandle_, &tx, &dummy_rx, 1, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(spiHandle_, dummy_tx, destination, num_bytes_to_retrieve, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(csGpioPort_, csPin_, GPIO_PIN_SET);
}

void ICM42688::writeRegister(uint8_t sub_address, uint8_t data_to_imu) {
    //Prepare transmit buffer
    uint8_t tx_buf[2] = {sub_address, data_to_imu};

    HAL_GPIO_WritePin(csGpioPort_, csPin_, GPIO_PIN_RESET);

    HAL_SPI_Transmit(spiHandle_, tx_buf, sizeof(tx_buf), HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(csGpioPort_, csPin_, GPIO_PIN_SET);
}

void ICM42688::beginMeasuring() {
    HAL_GPIO_WritePin(csGpioPort_, csPin_, GPIO_PIN_SET);
    reset();
    setLowNoiseMode();
    calibrate();
}

void ICM42688::setBank(uint8_t bank) {
    writeRegister(REG_BANK_SEL, bank);      //bank should always be 0 for acc and gyr data
}

void ICM42688::reset() {
    setBank(0);
    writeRegister(UB0_REG_DEVICE_CONFIG, 0x01);
    HAL_Delay(1);
}

void ICM42688::setLowNoiseMode() {
    writeRegister(UB0_REG_PWR_MGMT0, 0x0F);
}

void ICM42688::setGyroFS(uint8_t fssel) {
    uint8_t reg;
    
    setBank(0);
    
    //Read current register value
    readRegister(0x4F, 1, &reg);

    //Only change FS_SEL in reg
    reg = (fssel << 5) | (reg & 0x1F);

    writeRegister(0x4F, reg);
    
    gyroScale_ = (2000.0f / (float)(1 << fssel)) / 32768.0f;
    gyroFS_ = fssel;
}

void ICM42688::calibrate() {
    //Set at a lower range (more resolution since IMU not moving)
    const uint8_t currentFSSel_ = gyroFS_;
    setGyroFS(GYRO_SENSITIVITY_250DPS);        //Set to 250 dps

    //Take samples and find bias
    gyroBD_[0] = 0;
    gyroBD_[1] = 0;
    gyroBD_[2] = 0;

    for (size_t i = 0; i < NUM_GYRO_SAMPLES; i++) {
        readRegister(UB0_REG_TEMP_DATA1, 14, gyro_buffer_);

        //Combine raw bytes into 16-bit values
        for (size_t j = 0; j < 7; j++) {
            raw_meas_gyro_[j] = ((int16_t)gyro_buffer_[j * 2] << 8) | gyro_buffer_[j * 2 + 1];
        }

        //Process gyro data
        for (size_t k = 0; k < 3; k++) {
            gyr_[k] = (float)raw_meas_gyro_[k + 3] / GYRO_SENSITIVITY_250DPS;
        }

        /*
        Calculate bias by collecting samples and considering pre-existing bias
        For each iteration, add the existing bias with the new bias and divide by the sample size
        to get an average bias over a specified number of gyro samples
        */
        gyroBD_[0] += (gyr_[0] + gyrB_[0]) / NUM_GYRO_SAMPLES;
        gyroBD_[1] += (gyr_[1] + gyrB_[1]) / NUM_GYRO_SAMPLES;
        gyroBD_[2] += (gyr_[2] + gyrB_[2]) / NUM_GYRO_SAMPLES;

        HAL_Delay(1);
    }

    gyrB_[0] = gyroBD_[0];
    gyrB_[1] = gyroBD_[1];
    gyrB_[2] = gyroBD_[2];
    
    //Recover the full scale setting
    setGyroFS(currentFSSel_);
}

IMUData_t ICM42688::getResult(uint8_t * data_buffer) {
    //Collect Data
    readRegister(UB0_REG_TEMP_DATA1, 14, data_buffer);

    //Formatting raw data
    for (size_t i = 0; i < 3; i++) {
        raw_meas_[i] = ((int16_t)data_buffer[i * 2] << 8) | data_buffer[i * 2 + 1];
    }

    IMUData_t IMUData {};

    IMUData.accx = (float)raw_meas_[1] / ACCEL_SENSITIVITY_16G;
    IMUData.accy = (float)raw_meas_[2] / ACCEL_SENSITIVITY_16G;
    IMUData.accz = (float)raw_meas_[3] / ACCEL_SENSITIVITY_16G;

    IMUData.gyrx = (float)raw_meas_[4] / GYRO_SENSITIVITY_2000DPS;
    IMUData.gyry = (float)raw_meas_[5] / GYRO_SENSITIVITY_2000DPS;
    IMUData.gyrz = (float)raw_meas_[6] / GYRO_SENSITIVITY_2000DPS;

    return IMUData;
}