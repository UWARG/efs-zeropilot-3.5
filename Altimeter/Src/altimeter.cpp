#include <altimeter.hpp>


MS5611::MS5611(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, GPIO_TypeDef *ps_port, uint16_t cs_pin, uint16_t ps_pin){
	spi_handle_ = spi_handle;
	cs_port_ = cs_port;
	ps_port_ = ps_port;
	cs_pin_ = cs_pin;
	ps_pin_ = ps_pin;
	HAL_GPIO_WritePin(ps_port_, ps_pin_, GPIO_PIN_RESET);
}

void MS5611::altInit(){


	reset();
	getConvCoeffs();

	/*Get the average of 100 readings*/

	float reading_sum = 0.0f;
	for(int i = 0; i < 100; i++){
		calculateTemperatureAndPressure();
		reading_sum += getAltitudeAboveSeaLevel();
	}

	base_elev_ = reading_sum / 100.0f;
}

void MS5611::reset(){
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t send_reset[3] = {RESET, 0xFF, 0xFF};
	uint8_t receive_data[3];

	HAL_SPI_TransmitReceive(spi_handle_, send_reset, receive_data, 3, TIMEOUT);
	osDelay(3);

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
}

void MS5611::getConvCoeffs(){
	pressure_sensitivity_ = promRead(PROM_READ_ADDRESS_1);
	pressure_offset_ = promRead(PROM_READ_ADDRESS_2);
	pres_sensitivity_temp_coeff_ = promRead(PROM_READ_ADDRESS_3);
	pres_offset_temp_coeff_ = promRead(PROM_READ_ADDRESS_4);
	reference_temperature_ = promRead(PROM_READ_ADDRESS_5);
	temp_coeff_of_temp_ = promRead(PROM_READ_ADDRESS_6);
}


uint16_t MS5611::promRead(uint8_t address){
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t send_read_command[3] = {address, 0xFF, 0xFF};
	uint8_t coefficient_val[3];

	HAL_SPI_TransmitReceive(spi_handle_, send_read_command, coefficient_val, 3, TIMEOUT);

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	//Grab conversion coefficients.
	uint16_t conversion_coefficient = (coefficient_val[1] << 8) | (coefficient_val[2]) & 0x0000FFFF;;

	return conversion_coefficient;
}



uint32_t MS5611::uncompensatedPressureTemperature(uint8_t conversion_command){

	uint8_t digital_pres_temp[1] = {conversion_command};
	uint8_t receive_buffer[1];

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	/* send the digital pressure or temperature with the command */
	HAL_SPI_TransmitReceive(spi_handle_, digital_pres_temp, receive_buffer, 1, TIMEOUT);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	/* at least 8.22 ms delay required for ADC conversion.*/
	osDelay(9);

	uint8_t tx_buffer[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_buffer[4];

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	/*Start adc conversion to get value */
	HAL_SPI_TransmitReceive(spi_handle_, tx_buffer, rx_buffer, 4, TIMEOUT);

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	//get 24-bit uncompensated pressure or temperature value value.
	uint32_t uncompensated_value = (rx_buffer[1] << 16 | rx_buffer[2] << 8 | rx_buffer[3]) & 0x00FFFFFF;

	return uncompensated_value;
}


void MS5611::calculateTemperatureAndPressure(){

	uint32_t digital_temperature = uncompensatedPressureTemperature(CONVERT_D2_OSR_256);
	uint32_t digital_pressure = uncompensatedPressureTemperature(CONVERT_D1_OSR_256);

	const float TWO_POW_1 = 2.0f;
	const float TWO_POW_2 = 4.0f;
	const float TWO_POW_7 = 128.0f;
	const float TWO_POW_8 = 256.0f;
	const float TWO_POW_15 = 32768.0f;
	const float TWO_POW_16 = 65536.0f;
	const float TWO_POW_21 = 2097152.0f;
	const float TWO_POW_23 = 8388608.0f;
	const float TWO_POW_31 = 2147483648.0f;

	const float FIVE = 5.0f;

	float dt = digital_temperature - (float)reference_temperature_ * TWO_POW_8;
	float temp = 2000 + dt * (float)temp_coeff_of_temp_ / TWO_POW_23;
	float pressure_offset = pressure_offset_ * TWO_POW_16 + (pres_offset_temp_coeff_ * dt) / TWO_POW_7;
	float pressure_sensitivity = pressure_sensitivity_ * TWO_POW_15  + (pres_sensitivity_temp_coeff_ * dt) / TWO_POW_8;

	/*Second order temperature compensation */
	if(temp < 2000){
		float t2 = (dt*dt) / TWO_POW_31;
		float pressure_offset_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_1;
		float pressure_sensitivity_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_2;

		if(temp < 1500){
			pressure_offset_2 = pressure_offset_2 + 7 * pow((temp + 1500), TWO_POW_1);
			pressure_sensitivity_2 = pressure_sensitivity_2 + 11 * pow((temp + 1500), TWO_POW_1) / TWO_POW_1;
		}

		temp = temp - t2;
		pressure_offset = pressure_offset - pressure_offset_2;
		pressure_sensitivity = pressure_sensitivity - pressure_sensitivity_2;
	}

	float pressure = (digital_pressure * (pressure_sensitivity / TWO_POW_21) - pressure_offset) / TWO_POW_15;
	pressure /= 100.0f;
	temp /= 100.0f;

	pres_ = pressure;
	temp_ = temp;
}

float MS5611::getPressure(){
	calculateTemperatureAndPressure();
	return pres_;
}

float MS5611::getTemperature(){
	calculateTemperatureAndPressure();
	return temp_;
}

float MS5611::getAltitudeAboveSeaLevel(){

	float reference_temp = 288.15f; /* Ref temperature in Kelvins */
	float temp_lapse_rate = 0.0065f;
	float exp_gmrl = 5.2558;

	float reference_pressure = 101325.0f; /* in Pa */
	float current_pressure = getPressure() * 100.0f;
	float exponent = (log(current_pressure) - log(reference_pressure)) / exp_gmrl;

	float height = reference_temp/temp_lapse_rate * (1 - pow(M_E, exponent));

	return height;
}

float MS5611::getAltitudeAboveGroundLevel(){
	height_ = getAltitudeAboveSeaLevel();
	return {height_ - base_elev_};
}

