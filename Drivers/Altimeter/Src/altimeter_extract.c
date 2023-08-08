/*
 * calibration_data.c
 *
 *  Created on: Jul 27, 2023
 *      Author: folafatola
 */

#include <altimeter_extract.h>


void reset(GPIO_TypeDef *port, uint16_t GPIO_Pin){

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET); //pull the CS pin high.

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_RESET); //pull the CS pin low.

	//buffer stuff.
	uint8_t send_reset[3] = {RESET, 0xFF, 0xFF};


	hal_status = HAL_SPI_Transmit(&hspi1, send_reset, DATA_SIZE, TIMEOUT);

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET); //pull the CS pin high.
}


uint16_t promRead(GPIO_TypeDef *port, uint16_t GPIO_Pin, uint8_t address){
	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET); //pull the CS pin high.

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // start timer.

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_RESET);

	uint8_t send_read_command[3] = {address, 0xFF, 0xFF};
	uint8_t memory_data[3];

	hal_status = HAL_SPI_TransmitReceive(&hspi1, send_read_command, memory_data, DATA_SIZE, TIMEOUT);

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET);

	//Grab conversion coefficients.
	uint16_t conversion_coefficient = (memory_data[1] << 8) | (memory_data[2]);

	return conversion_coefficient;
}


uint32_t uncompensatedPressureTemperature(GPIO_TypeDef *port, uint16_t GPIO_Pin, uint8_t conversion){

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET); //pull the CS pin high.

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // start timer.

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_RESET);

	uint8_t d2_d2_conversion[1] = {conversion};
	uint8_t receive_buffer[1];

	//start the d1_conversion with the command.
	hal_status = HAL_SPI_TransmitReceive(&hspi1, d2_d2_conversion, receive_buffer, 1, TIMEOUT);

	//Required 8.22 ms delay for ADC conversion.
	HAL_delay(15);

	uint8_t tx_buffer[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_buffer[4];

	//get uncompensated pressure value.
	hal_status = HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, 4, TIMEOUT);

	//get 24-bit uncompensated pressure or temperature value value.
	uint32_t uncompensated_value = (rx_buffer[1] << 16 | rx_buffer[2] << 8 | rx_buffer[3]);

	HAL_GPIO_WritePin(port, GPIO_Pin, GPIO_PIN_SET);

	return uncompensated_value;
}


long calculateTemperature(GPIO_TypeDef *port, uint16_t GPIO_Pin){

	//calculate calibration data.
	uint16_t t_ref = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_5);
	uint16_t temp_sens = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_6);

	uint32_t d2_conversion = uncompensatedPressureTemperature(port, GPIO_Pin, CONVERT_D2_OSR_256);

	//difference between actual and reference temperature (D2 - C5).
	long dt = d2_conversion - t_ref * TWO_POW_8;

	long temperature = TWENTY_DEGREE_VAL + dt * temp_sens / TWO_POW_23;

	return temperature;
}


long calculatePressure(GPIO_TypeDef *port, uint16_t GPIO_Pin){
	uint16_t sens = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_1);
	uint16_t off = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_2);
	uint16_t tcs = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_3);
	uint16_t tco = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_4);
	uint16_t t_ref = promRead(port, GPIO_Pin, PROM_READ_ADDRESS_5);

	uint32_t d2_conversion = uncompensatedPressureTemperature(port, GPIO_Pin, CONVERT_D2_OSR_256);

	long dt = d2_conversion - t_ref * TWO_POW_8;

	int64_t offset = off * TWO_POW_16 + (tco * dt) / TWO_POW_23;

	int64_t sensitivity_at_real_temp = sens * TWO_POW_15 + (tcs * dt) / TWO_POW_8;

	uint32_t d1_conversion = uncompensatedPressureTemperature(port, GPIO_Pin, CONVERT_D1_OSR_256);

	long pressure = (d1_conversion * sensitivity_at_real_temp / TWO_POW_21 - offset) / TWO_POW_15;

	return pressure;
}
