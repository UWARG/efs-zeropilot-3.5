// icm42688.cpp

#include "icm42688.hpp"
#include "stm32l5xx_hal_conf.h"
#include "stm32l5xx_it.h"
#include "spi.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//#define CS_GPIO_PORT GPIOA
//#define CS_PIN GPIO_PIN_4

#define REG_BANK_SEL 0x76;
#define UB0_REG_DEVICE_CONFIG 0x11;
#define UB0_REG_PWR_MGMT0 0x4E;
#define UB0_REG_TEMP_DATA1 0x1D;

#define READ_BIT 0x80

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

void ICM42688::readRegister(uint8_t sub_address, uint8_t count, uint8_t * dest) {
    //Set read bit for register address
    uint8_t tx = sub_address | READ_BIT;

    //Dummy transmit and receive buffers
    uint8_t dummy_tx[count];
    uint8_t dummy_rx;

    //Initialize values to 0
    memset(dummy_tx, 0, count * sizeof(dummy_tx[0]));

    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

    HAL_SPI_TransmitReceive(SPI_HANDLE, &tx, &dummy_rx, 1, HAL_MAX_DELAY);
    HAL_SPI_TransmitReceive(SPI_HANDLE, dummy_tx, dest, count, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
}

void ICM42688::writeRegister(uint8_t sub_address, uint8_t data) {
    //Prepare transmit buffer
    uint8_t tx_buf[2] = {sub_address, data};

    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

    HAL_SPI_Transmit(SPI_HANDLE, tx_buf, sizeof(tx_buf), HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
}

void ICM42688::beginMeasuring() {
    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
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
    
    gyro_scale = (2000.0f / (float)(1 << fssel)) / 32768.0f;
    gyroFS = fssel;
}

void ICM42688::calibrate() {
    //Set at a lower range (more resolution since IMU not moving)
    const uint8_t current_fssel = gyroFS;
    setGyroFS(0x03);        //Set to 250 dps

    //Take samples and find bias
    gyroBD[0] = 0;
    gyroBD[1] = 0;
    gyroBD[2] = 0;

    for (size_t i = 0; i < 3; i++) {
        getResult(gyro_buffer);
        for (size_t i = 0; i < 7; i++) {
            raw_meas_gyro[i] = ((int16_t)gyro_buffer[i * 2] << 8) | gyro_buffer[i * 2 + 1];
        }
        for (size_t i = 0; i < 3; i++) {
            gyr[i] = (float)raw_meas_gyro[i + 3] / 16.4;
        }

        gyroBD[0] += (gyr[0] + gyrB[0]) / 1000;
        gyroBD[1] += (gyr[1] + gyrB[1]) / 1000;
        gyroBD[2] += (gyr[2] + gyrB[2]) / 1000;

        HAL_Delay(1);
    }

    gyrB[0] = gyroBD[0];
    gyrB[1] = gyroBD[1];
    gyrB[2] = gyroBD[2];
    
    //Recover the full scale setting
    setGyroFS(current_fssel);
}

IMUData_t ICM42688::getResult(uint8_t * data_buffer) {
    //Collect Data
    readRegister(UB0_REG_TEMP_DATA1, 14, data_buffer);

    //Formatting raw data
    for (size_t i = 0; i < 3; i++) {
        raw_meas[i] = ((int16_t)data_buffer[i * 2] << 8) | data_buffer[i * 2 + 1];
    }

    IMUData_t IMUData {};

    IMUData.accx = (float)raw_meas[1] / ACCEL_SENSITIVITY_16G;
    IMUData.accy = (float)raw_meas[2] / ACCEL_SENSITIVITY_16G;
    IMUData.accz = (float)raw_meas[3] / ACCEL_SENSITIVITY_16G;

    IMUData.gyrx = (float)raw_meas[4] / GYRO_SENSITIVITY_2000DPS;
    IMUData.gyry = (float)raw_meas[5] / GYRO_SENSITIVITY_2000DPS;
    IMUData.gyrz = (float)raw_meas[6] / GYRO_SENSITIVITY_2000DPS;

    return IMUData;
}