#include <altimeter.hpp>


MS5611::MS5611(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, GPIO_TypeDef *ps_port, uint16_t cs_pin, uint16_t ps_pin){
	spi_handle_ = spi_handle;
	cs_port_ = cs_port;
	ps_port_ = ps_port;
	cs_pin_ = cs_pin;
	ps_pin_ = ps_pin;
	HAL_GPIO_WritePin(ps_port_, ps_pin_, GPIO_PIN_RESET);

	reset();
	getConvCoeffs();

	// Get the average of 100 readings.
	float reading_sum = 0.0f;
	for (int i = 0; i < 100; i++) {
		reading_sum += getAltitudeAboveSeaLevel();
	}

	base_elev_ = reading_sum / 100.0f;
}

void MS5611::reset(){
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t tx_data[3] = {RESET_COMMAND, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	HAL_SPI_TransmitReceive(spi_handle_, tx_data, rx_data, data_size, TIMEOUT);
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

	uint8_t tx_data[3] = {address, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	HAL_SPI_TransmitReceive(spi_handle_, tx_data, rx_data, data_size, TIMEOUT);

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// construct 16-bit calibration coefficient.
	uint16_t calibration_coefficient = (rx_data[1] << 8) | (rx_data[2]) & 0x0000FFFF;

	return calibration_coefficient;
}

uint32_t MS5611::readPressureTemperatureUncompensated(uint8_t conversion_command){

	uint8_t tx_data = conversion_command;
	uint8_t rx_data;
	uint16_t data_size = 1;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// send the digital pressure or temperature with the command.
	HAL_SPI_TransmitReceive(spi_handle_, tx_data, rx_data, data_size, TIMEOUT);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// at least 8.22 ms delay required for ADC conversion.
	osDelay(9);

	uint8_t tx_data_2[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_data_2[4];
	data_size = 4;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// Start adc conversion to get value
	HAL_SPI_TransmitReceive(spi_handle_, tx_data_2, rx_data_2, data_size, TIMEOUT);

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// get 24-bit uncompensated pressure or temperature value value.
	uint32_t uncompensated_value = (rx_data_2[1] << 16 | rx_data_2[2] << 8 | rx_data_2[3]) & 0x00FFFFFF;

	return uncompensated_value;
}

void MS5611::calculateTemperatureAndPressure(){

	uint32_t digital_temperature = readPressureTemperatureUncompensated(CONVERT_D2_OSR_256);
	uint32_t digital_pressure = readPressureTemperatureUncompensated(CONVERT_D1_OSR_256);

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

	// Second order temperature compensation.
	if (temp < 2000) {
		float t2 = (dt*dt) / TWO_POW_31;
		float pressure_offset_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_1;
		float pressure_sensitivity_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_2;

		if (temp < 1500) {
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
	const float REFERENCE_TEMP = 288.15f; /* Ref temperature in Kelvins */
	const float TEMP_LAPSE_RATE = 0.0065f;
	const float EXP_GMRL = 5.2558;

	const float REFERENCE_PRESSURE = 101325.0f; /* in Pa */
	const float CURRENT_PRESSURE = getPressure() * 100.0f;
	const float EXPONENT = (log(CURRENT_PRESSURE) - log(REFERENCE_PRESSURE)) / EXP_GMRL;

	float height = REFERENCE_TEMP / TEMP_LAPSE_RATE * (1 - pow(M_E, EXPONENT));

	return height;
}

float MS5611::getAltitudeAboveGroundLevel(){
	height_ = getAltitudeAboveSeaLevel();
	return {height_ - base_elev_};
}

