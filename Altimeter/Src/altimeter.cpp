#include <altimeter.hpp>


MS5611::MS5611(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, GPIO_TypeDef *ps_port, uint16_t cs_pin, uint16_t ps_pin){
	spi_handle_ = spi_handle;
	cs_port_ = cs_port;
	ps_port_ = ps_port;
	cs_pin_ = cs_pin;
	ps_pin_ = ps_pin;
	HAL_GPIO_WritePin(ps_port_, ps_pin_, GPIO_PIN_RESET);

	communication_success_ = true;

	reset();
	getPromData();

	// Grab the sensor's altitude above sea level while on some surface and assign it to base_elev_
	// Subtract this from measurements at greater heights for the height above that initial surface.
	float base_elevation = 0.0f;
	if (communication_success_) {   //if HAL_SPI_TransmitReceive_IT doesn't fail in reset or getPromData()

		float altitude_above_sea_level_measurement = 0;
		int num_successful_measurements = 0;
		for (int i = 0; i < 100; i++) { 			//take a few measurements to make sure they are accurate/consistent.
			communication_success_ = true;
			communication_success_ = getAltitudeAboveSeaLevel(altitude_above_sea_level_measurement);
			osDelay(10);
			if (communication_success_) {			//count the number of successful measurements.
				base_elevation = altitude_above_sea_level_measurement;
				num_successful_measurements += 1;
			}else{
				communication_success_ = false;
			}
		}
	}

	base_elev_ = base_elevation;
}

void MS5611::reset(){
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t tx_data[3] = {RESET_COMMAND, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data, rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		communication_success_ = false;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	osDelay(3);
}

void MS5611::getPromData(){
	for (int i = 0; i < 8; ++i) {
		communication_success_ = promRead(PROM_READ_ADDRESS_0 + 2 * i, ms6511_prom_data_ + i);
		if (!communication_success_){
			return;
		}
	}
}

bool MS5611::promRead(uint8_t address, uint16_t *ms5611_prom_data){
	if (!communication_success_){
		return communication_success_;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	uint8_t tx_data[3] = {address, 0xFF, 0xFF};
	uint8_t rx_data[3];
	uint16_t data_size = 3;

	spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data, rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		communication_success_ = false;
		return communication_success_;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	osDelay(9);

	// construct 16-bit calibration coefficient.
	*ms5611_prom_data = ((rx_data[1] << 8) | (rx_data[2])) & 0x0000FFFF;

	return communication_success_;
}

bool MS5611::readPressureTemperatureUncompensated(uint8_t conversion_command, uint32_t &uncompensated_pressure_temperature){

	if (!communication_success_){
		return communication_success_;
	}

	uint8_t tx_data = conversion_command;
	uint8_t rx_data;
	uint8_t tx_data_2[4] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t rx_data_2[4];
	uint16_t data_size = 1;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// send the digital pressure or temperature with the command.
	spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, &tx_data, &rx_data, data_size);
	if (spi_status_ != HAL_OK) {
		communication_success_ = false;
		return communication_success_;
	}
	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// at least 8.22 ms delay required for ADC conversion.
	osDelay(9);

	data_size = 4;

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_RESET);

	// Start adc conversion to get value
	spi_status_ = HAL_SPI_TransmitReceive_IT(spi_handle_, tx_data_2, rx_data_2, data_size);
	if (spi_status_ != HAL_OK) {
		communication_success_ = false;
		return communication_success_;
	}

	HAL_GPIO_WritePin(cs_port_, cs_pin_, GPIO_PIN_SET);

	// get 24-bit uncompensated pressure or temperature value value.
	uncompensated_pressure_temperature = (rx_data_2[1] << 16 | rx_data_2[2] << 8 | rx_data_2[3]) & 0x00FFFFFF;
	osDelay(9);

	return communication_success_;
}

void MS5611::calculateTemperatureAndPressure(){

	uint32_t digital_temperature = 0;
	uint32_t digital_pressure = 0;

	communication_success_ = readPressureTemperatureUncompensated(CONVERT_D2_OSR_4096, digital_temperature);
	if (!communication_success_){
		return;
	}

	communication_success_ = readPressureTemperatureUncompensated(CONVERT_D1_OSR_4096, digital_pressure);
	if (!communication_success_){
		return;
	}

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
		float pressure_offset_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_1;
		float pressure_sensitivity_2 = FIVE * pow((temp - 2000), TWO_POW_1) / TWO_POW_2;

		if (temp < 1500) {
			pressure_offset_2 = pressure_offset_2 + 7 * pow((temp + 1500), TWO_POW_1);
			pressure_sensitivity_2 = pressure_sensitivity_2 + 11 * pow((temp + 1500), TWO_POW_1) / TWO_POW_1;
		}

		temp = temp - t2;
		f_pressure_offset = f_pressure_offset - pressure_offset_2;
		f_pressure_sensitivity = f_pressure_sensitivity - pressure_sensitivity_2;
	}

	float pressure = (digital_pressure * (f_pressure_sensitivity / TWO_POW_21) - f_pressure_offset) / TWO_POW_15;
	pressure /= 100.0f;
	temp /= 100.0f;

	pres_ = pressure;
	temp_ = temp;
}

bool MS5611::getPressure(float &pressure){
	calculateTemperatureAndPressure();
	if (!communication_success_) {
		return false;
	}
	pressure = pres_;
	return true;
}

bool MS5611::getTemperature(float &temperature){
	calculateTemperatureAndPressure();
	if (!communication_success_) {
		return false;
	}
	temperature = temp_;
	return true;
}

bool MS5611::getAltitudeAboveSeaLevel(float &altitude_above_sea_level){
	float pressure = 0.0f;
	communication_success_ = getPressure(pressure);

	if (!communication_success_) {
		return communication_success_;
	}

	const float REFERENCE_TEMP = 288.15f; /* Ref temperature in Kelvins */
	const float TEMP_LAPSE_RATE = 0.0065f;
	const float EXP_GMRL = 5.2558;

	const float REFERENCE_PRESSURE = 101325.0f; /* in Pa */
	const float CURRENT_PRESSURE = pressure * 100.0f;
	const float EXPONENT = (log(CURRENT_PRESSURE) - log(REFERENCE_PRESSURE)) / EXP_GMRL;

	altitude_above_sea_level = REFERENCE_TEMP / TEMP_LAPSE_RATE * (1 - pow(M_E, EXPONENT));

	return communication_success_;
}

bool MS5611::getAltitudeAboveGroundLevel(float &altitude_above_ground_level){
	reset();
	float alt_above_sea_lvl = 0.0f;
	communication_success_ = getAltitudeAboveSeaLevel(alt_above_sea_lvl);
	if (!communication_success_) {
		return communication_success_;
	}

	altitude_above_ground_level = alt_above_sea_lvl - base_elev_;
	return communication_success_;
}
