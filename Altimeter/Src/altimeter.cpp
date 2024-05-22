#include <altimeter.hpp>

MS5611::MS5611(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, GPIO_TypeDef *ps_port, uint16_t cs_pin, uint16_t ps_pin){
	spi_handle_ = spi_handle;
	cs_port_ = cs_port;
	ps_port_ = ps_port;
	cs_pin_ = cs_pin;
	ps_pin_ = ps_pin;
	HAL_GPIO_WritePin(ps_port_, ps_pin_, GPIO_PIN_RESET);

	setup_done_ = true;
	base_elevation_set_ = true;
	getProm();

	// Grab the sensor's altitude above sea level while on some surface and assign it to base_elev_
	// Subtract this from measurements at greater heights for the height above that initial surface.
	for (int i = 0; i < 100; i++) { //take a few measurements to make sure they are accurate/consistent.
		setBaseElevation();
	}
}

void MS5611::reset(){
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t tx_data[3] = {RESET_COMMAND, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	HAL_StatusTypeDef spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data, rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		setup_done_ = false;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	osDelay(3);
}

uint16_t MS5611::getPromElement(uint8_t address){

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t tx_data[3] = {address, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	HAL_StatusTypeDef spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data, rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		setup_done_ = false;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// construct 16-bit calibration coefficient.
	uint16_t prom_element = ((rx_data[1] << 8) | (rx_data[2])) & 0x0000FFFF;

	return prom_element;
}

void MS5611::getProm(){
	reset();
	for (int i = 0; i < 8; ++i) {
		ms6511_prom_data_[i] = getPromElement(PROM_READ_ADDRESS_0 + 2 * i);
		if (!setup_done_){
			return;
		}
	}

	setup_done_ = true;
}

bool MS5611::getBarometer(uint8_t conversion_command, uint32_t &uncompensated_pressure_temperature){

	if(!setup_done_){
		return false;
	}

	uint8_t tx_data = conversion_command;
	uint8_t rx_data;
	uint8_t tx_data_2[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_data_2[4];
	uint16_t data_size = 1;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// send the digital pressure or temperature with the command.
	HAL_StatusTypeDef spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, &tx_data, &rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		return false;
	}
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// at least 8.22 ms delay required for ADC conversion.
	osDelay(9);

	data_size = 4;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// Start adc conversion to get value
	spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data_2, rx_data_2, data_size);
	if (spi_status_ != HAL_OK) {
		return false;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	osDelay(9);

	// get 24-bit uncompensated pressure or temperature value value.
	uncompensated_pressure_temperature = (rx_data_2[1] << 16 | rx_data_2[2] << 8 | rx_data_2[3]) & 0x00FFFFFF;

	return true;
}

float MS5611::getPressure(){

	uint32_t digital_temperature = 0;
	uint32_t digital_pressure = 0;

	if (!getBarometer(CONVERT_D2_OSR_4096, digital_temperature)){
		return FLT_MAX;
	}

	if (!getBarometer(CONVERT_D1_OSR_4096, digital_pressure)){
		return FLT_MAX;
	}

	// ms5611 calibration coefficients for pressure and temperature calculations.
	uint16_t pressure_sensitivity = ms6511_prom_data_[1];
	uint16_t pressure_offset = ms6511_prom_data_[2];
	uint16_t pres_sensitivity_temp_coeff = ms6511_prom_data_[3]; //Temperature coefficient of pressure sensitivity.
	uint16_t pres_offset_temp_coeff = ms6511_prom_data_[4];	  // temperature coefficient of temperature offset.
	uint16_t reference_temperature = ms6511_prom_data_[5];
	uint16_t temp_coeff_of_temp = ms6511_prom_data_[6];		  // Temperature coefficient of temperature.

	float dt = digital_temperature - (float)reference_temperature * TWO_POW_8;
	float f_pressure_offset = pressure_offset * TWO_POW_16 + (pres_offset_temp_coeff * dt) / TWO_POW_7;
	float f_pressure_sensitivity = pressure_sensitivity * TWO_POW_15  + (pres_sensitivity_temp_coeff * dt) / TWO_POW_8;

	float pressure = ((digital_pressure * (f_pressure_sensitivity / TWO_POW_21) - f_pressure_offset) / TWO_POW_15)/100;

	return pressure;
}

float MS5611::getTemperature(){

	uint32_t digital_temperature = 0;
	if (!getBarometer(CONVERT_D2_OSR_4096, digital_temperature)){
		return FLT_MAX;
	}

	// ms5611 calibration coefficients for pressure and temperature calculations.
	uint16_t pressure_sensitivity = ms6511_prom_data_[1];
	uint16_t pressure_offset = ms6511_prom_data_[2];
	uint16_t pres_sensitivity_temp_coeff = ms6511_prom_data_[3]; //Temperature coefficient of pressure sensitivity.
	uint16_t pres_offset_temp_coeff = ms6511_prom_data_[4];	  // temperature coefficient of temperature offset.
	uint16_t reference_temperature = ms6511_prom_data_[5];
	uint16_t temp_coeff_of_temp = ms6511_prom_data_[6];		  // Temperature coefficient of temperature.


	float dt = digital_temperature - (float)reference_temperature * TWO_POW_8;
	float temp = 2000 + dt * (float)temp_coeff_of_temp / TWO_POW_23;
	float f_pressure_offset = pressure_offset * TWO_POW_16 + (pres_offset_temp_coeff * dt) / TWO_POW_7;
	float f_pressure_sensitivity = pressure_sensitivity * TWO_POW_15  + (pres_sensitivity_temp_coeff * dt) / TWO_POW_8;


	// Second order temperature compensation.
	if (temp < 2000) {
		float t2 = (dt*dt) / TWO_POW_31;
		float pressure_offset_2 = 5.0f * pow((temp - 2000), 2) / 2;
		float pressure_sensitivity_2 = 5.0f * pow((temp - 2000), 2) / 4;

		if (temp < 1500) {
			pressure_offset_2 = pressure_offset_2 + 7 * pow((temp + 1500), 2);
			pressure_sensitivity_2 = pressure_sensitivity_2 + 11 * pow((temp + 1500), 2) / 2;
		}

		temp = temp - t2;
		f_pressure_offset = f_pressure_offset - pressure_offset_2;
		f_pressure_sensitivity = f_pressure_sensitivity - pressure_sensitivity_2;
	}

	temp /= 100.0f;

	return temp;
}

float MS5611::getAltitudeAboveSeaLevel(){
	float pressure = getPressure();
	if(pressure == FLT_MAX){
		return FLT_MAX;
	}

	const float CURRENT_PRESSURE = pressure * 100.0f;
	const float EXPONENT = (log(CURRENT_PRESSURE) - log(REFERENCE_PRESSURE)) / EXP_GMRL;

	float altitude_above_sea_level = REFERENCE_TEMP / TEMP_LAPSE_RATE * (1 - pow(M_E, EXPONENT));

	return altitude_above_sea_level;
}

float MS5611::getAltitudeAboveGroundLevel(){
	if(!base_elevation_set_){
		return FLT_MAX;
	}

	float alt_above_sea_lvl = getAltitudeAboveSeaLevel();
	if (alt_above_sea_lvl == FLT_MAX) {
		return FLT_MAX;
	}

	float altitude_above_ground_level = alt_above_sea_lvl - base_elev_;
	return altitude_above_ground_level;
}

float MS5611::setBaseElevation(){
	base_elev_ = getAltitudeAboveSeaLevel();
	if(base_elev_ == FLT_MAX){
		base_elevation_set_ = false;
		return FLT_MAX;
	}

	base_elevation_set_ = true;

	return base_elev_;
}
