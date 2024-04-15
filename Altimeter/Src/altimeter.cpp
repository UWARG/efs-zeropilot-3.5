#include <altimeter.hpp>


AltDevice::AltDevice(GPIO_TypeDef *ps_port, uint16_t ps_pin){
	HAL_GPIO_WritePin(ps_port, ps_pin, GPIO_PIN_RESET);
}

void AltDevice::altInit(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin){
	reset(spi_handle, cs_port, cs_pin);
	getConvCoeffs(spi_handle, cs_port, cs_pin);

	/*Get the average of 100 readings*/

	float reading_sum = 0.0f;
	for(int i = 0; i < 100; i++){
		calculateTempPres(spi_handle, cs_port, cs_pin);
		reading_sum += getAltAboveSeaLvl();
	}

	base_elev_ = reading_sum / 100.0f;
}

void AltDevice::reset(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin){

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

	uint8_t send_reset[3] = {RESET, 0xFF, 0xFF};
	uint8_t receive_data[3];

	HAL_SPI_TransmitReceive(spi_handle, send_reset, receive_data, 3, TIMEOUT);
	osDelay(3);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
}

void AltDevice::getConvCoeffs(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin){
	coeffs_.sens = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_1);
	coeffs_.off = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_2);
	coeffs_.tcs = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_3);
	coeffs_.tco = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_4);
	coeffs_.t_ref = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_5);
	coeffs_.temp_sens = promRead(spi_handle, cs_port, cs_pin, PROM_READ_ADDRESS_6);
}


uint16_t AltDevice::promRead(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t address){
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

	uint8_t send_read_command[3] = {address, 0xFF, 0xFF};
	uint8_t coefficient_val[3];

	HAL_SPI_TransmitReceive(spi_handle, send_read_command, coefficient_val, 3, TIMEOUT);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	//Grab conversion coefficients.
	uint16_t conversion_coefficient = (coefficient_val[1] << 8) | (coefficient_val[2]);

	return conversion_coefficient;
}



uint32_t AltDevice::uncompensatedPressureTemperature(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t conversion_command){

	uint8_t digital_pres_temp[1] = {conversion_command};
	uint8_t receive_buffer[1];

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

	/* send the digital pressure or temperature with the command */
	HAL_SPI_TransmitReceive(spi_handle, digital_pres_temp, receive_buffer, 1, TIMEOUT);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	/* at least 8.22 ms delay required for ADC conversion.*/
	osDelay(9);

	uint8_t tx_buffer[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_buffer[4];

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

	/*Start adc conversion to get value */
	HAL_SPI_TransmitReceive(spi_handle, tx_buffer, rx_buffer, 4, TIMEOUT);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	//get 24-bit uncompensated pressure or temperature value value.
	uint32_t uncompensated_value = (rx_buffer[1] << 16 | rx_buffer[2] << 8 | rx_buffer[3]);

	return uncompensated_value;
}


void AltDevice::calculateTempPres(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin){


	uint32_t digital_temperature = uncompensatedPressureTemperature(spi_handle, cs_port, cs_pin, CONVERT_D2_OSR_256);
	uint32_t digital_pressure = uncompensatedPressureTemperature(spi_handle, cs_port, cs_pin, CONVERT_D1_OSR_256);

	float dt = digital_temperature - (float)coeffs_.t_ref * 256.0f;
	float temp = 2000 + dt * (float) coeffs_.temp_sens / pow(2, 23);


	float off = coeffs_.off * 65536.0f + (coeffs_.tco * dt) / 128.0f;
	float sens = coeffs_.sens * 32768.0f  + (coeffs_.tcs * dt) / 256.0f;

	/*Second order temperature compensation */
	if(temp < 2000){
		float t2 = (dt*dt) / 2147483648.0f;
		float off2 = 5.0f * pow((temp - 2000), 2) / 2.0f;
		float sens2 = 5.0f * pow((temp - 2000), 2) / 4.0f;


		if(temp < 1500){
			off2 = off2 + 7 * pow((temp + 1500), 2);
			sens2 = sens2 + 11 * pow((temp + 1500), 2) / 2;
		}

		temp = temp - t2;
		off = off - off2;
		sens = sens - sens2;

	}

	float pressure = (digital_pressure * (sens / 2097152.0f) - off) / 32768.0f;
	pressure /= 100.0f;
	temp /= 100.0f;

	pres_ = pressure;
	temp_ = temp;
}


float AltDevice::getPressure(){
	return pres_;
}

float AltDevice::getTemperature(){
	return temp_;
}

float AltDevice::getAltAboveSeaLvl(){

	float reference_temp = 288.15f; /* Ref temperature in Kelvins */
	float temp_lapse_rate = 0.0065f;
	float exp_gmrl = 5.2558;

	float reference_pressure = 101325.0f; /* in Pa */
	float current_pressure = getPressure() * 100.0f;
	float exponent = (log(current_pressure) - log(reference_pressure)) / exp_gmrl;

	float height = reference_temp/temp_lapse_rate * (1 - pow(M_E, exponent));

	return height;
}


float AltDevice::getAltitude(){
	height_ = getAltAboveSeaLvl();
	return {height_ - base_elev_};
}

