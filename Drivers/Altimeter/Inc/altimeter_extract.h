/*
 * calibration_data.h
 *
 *  Created on: Jul 27, 2023
 *      Author: folafatola
 */

#ifndef INC_ALTIMETER_EXTRACT_H_
#define INC_ALTIMETER_EXTRACT_H_

//#include "adc.h"
//#include "spi.h"
//#include "tim.h"
//#include "gpio.h"
#include "main.h"
#include <stdint.h>

/* MS5611 commands
 * The MS5611 has 5, one-byte commands.
 * Below is a list of those commands.
 */

#define RESET 0x1E

/* The following are PROMREAD addresses.
 * According to the data sheet, there are 8 of these.
 */

#define PROM_READ_ADDRESS_0 0xA0
#define PROM_READ_ADDRESS_1 0xA2
#define PROM_READ_ADDRESS_2 0xA4
#define PROM_READ_ADDRESS_3 0xA6
#define PROM_READ_ADDRESS_4 0xA8
#define PROM_READ_ADDRESS_5 0xAA
#define PROM_READ_ADDRESS_6 0xAC
#define PROM_READ_ADDRESS_7 0xAE


#define CONVERT_D1_OSR_256 0x40 //convert D1 at oversampling rate of 256...
#define CONVERT_D2_OSR_256 0x50
#define ADC_READ 0x0


#define DATA_SIZE 3
#define TIMEOUT 1 << 31

#define TWO_POW_8 256
#define TWO_POW_23 8388608
#define TWO_POW_15 32768
#define TWO_POW_16 65536
#define TWO_POW_21 2097152

#define TWENTY_DEGREE_VAL 2000


/* Necessary handlers
 */

SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim1;
HAL_StatusTypeDef hal_status;




/**
* Resets the MS5611 altimeter to make sure calibration PROM gets into the internal register.
 *
* @param port -> pointer to microcontroller porter.
 *
* @param GPIO_Pin -> microcontroller pin
 *
* @return none.
 */

void reset(GPIO_TypeDef *port, uint16_t GPIO_Pin);


/**
* Reads from one of the 16-bit addresses of the altimeter's 128-bit PROM.
* This is relevant to read the calibration coefficients needed to get accurate
* temperature/pressure values.
 *
* @param port -> pointer to microcontroller porter.
 *
* @param GPIO_Pin -> microcontroller pin.
 *
* @param address -> address of PROM to be read from.
 *
* @return uint16_t conversion_coefficient value.
 */

uint16_t promRead(GPIO_TypeDef *port, uint16_t GPIO_Pin, uint8_t address);


/**
* This function calculates the uncompensated pressure/temperature values read from the altimeter.
* by choosing to execute command D1 or D2.
 *
* @param port -> pointer to microcontroller porter.
 *
* @param GPIO_Pin -> microcontroller pin.
 *
* @param conversion -> Takes in either the D1 or D2 command. See CONVERT_D1_OSR_256 and CONVERT_D2_OSR_256 in
* altimeter_extract.h file.
 *
* @return uint32_t uncompensated temperature/pressure value.
 */

uint32_t uncompensatedPressureTemperature(GPIO_TypeDef *port, uint16_t GPIO_Pin, uint8_t conversion);


/**
* This function calculates the accurate temperature based on values read from altimeter and
* conversion coefficients read from the 128-bit PROM.
 *
* @param port -> pointer to microcontroller porter.
 *
* @param GPIO_Pin -> microcontroller pin.
 *
* @return long temperature value.
 */

long calculateTemperature(GPIO_TypeDef *port, uint16_t GPIO_Pin);


/**
* This function calculates the accurate pressure based on values read from altimeter and
* conversion coefficients read from the 128-bit PROM.
 *
* @param port -> pointer to microcontroller porter.
 *
* @param GPIO_Pin -> microcontroller pin.
 *
* @return long pressure value.
 */

long calculatePressure(GPIO_TypeDef *port, uint16_t GPIO_Pin);



#endif /* INC_ALTIMETER_EXTRACT_H_ */
