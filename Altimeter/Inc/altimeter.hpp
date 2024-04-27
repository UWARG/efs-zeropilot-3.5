#ifndef INC_ALTIMETER_HPP_
#define INC_ALTIMETER_HPP_

#include "stm32l5xx_hal.h"
#include "cmsis_os.h"
#include <cstdint>
#include <cmath>


constexpr uint8_t RESET_COMMAND = 0x1E;

// PROM Addresses.
constexpr uint8_t PROM_READ_ADDRESS_0 = 0xA0;
constexpr uint8_t PROM_READ_ADDRESS_1 = 0xA2;
constexpr uint8_t PROM_READ_ADDRESS_2 = 0xA4;
constexpr uint8_t PROM_READ_ADDRESS_3 = 0xA6;
constexpr uint8_t PROM_READ_ADDRESS_4 = 0xA8;
constexpr uint8_t PROM_READ_ADDRESS_5 = 0xAA;
constexpr uint8_t PROM_READ_ADDRESS_6 = 0xAC;
constexpr uint8_t PROM_READ_ADDRESS_7 = 0xAE;


// digital pressure/temperature commands at different sampling rates.
constexpr uint8_t CONVERT_D1_OSR_256 = 0x40;
constexpr uint8_t CONVERT_D1_OSR_512 = 0x42;
constexpr uint8_t CONVERT_D1_OSR_1024 = 0x44;
constexpr uint8_t CONVERT_D1_OSR_2048 = 0x46;
constexpr uint8_t CONVERT_D1_OSR_4096 = 0x48;
constexpr uint8_t CONVERT_D2_OSR_256 = 0x50;
constexpr uint8_t CONVERT_D2_OSR_512  = 0x52;
constexpr uint8_t CONVERT_D2_OSR_1024 = 0x54;
constexpr uint8_t CONVERT_D2_OSR_2048 = 0x56;
constexpr uint8_t CONVERT_D2_OSR_4096 = 0x58;

constexpr uint8_t ADC_READ = 0x0;

constexpr uint32_t TIMEOUT = 1 << 31;

class MS5611{
public:
	/**
	* Constructor for the MS5611 class.
	* It assigns the spi_handler and ports and pins related to the chip select and protocol pins.
	* This function sends the reset command, populates reads calibration coefficients from
	* PROM and gets the sensor's initial elevation above sea level.
	 *
	*
	* @param spi_handle -> hspix handle
	* @param cs_port -> chip select pin port
	* @param ps_port -> protocol select port
	* @param cs_pin -> chip select pin
	* @param ps_pin -> protocol select pin
	 *
	* @return none
	 */
	MS5611(SPI_HandleTypeDef *spi_handle, 
		   GPIO_TypeDef *cs_port, 
		   GPIO_TypeDef *ps_port, 
		   uint16_t cs_pin, 
		   uint16_t ps_pin);

	/**
	* @return the current pressure value
	 */
	float getPressure();

	/**
	* @return the current temperature value
	 */
	float getTemperature();

	/**
	* @return the current altitude above sea level.
	 */
	float getAltitudeAboveSeaLevel();

	/**
	* @return the current altitude above zero point.
	 */
	float getAltitudeAboveGroundLevel();


private:

	// Setup description comment.
	SPI_HandleTypeDef *spi_handle_;
	GPIO_TypeDef *cs_port_;
	GPIO_TypeDef *ps_port_;
	uint16_t cs_pin_;
	uint16_t ps_pin_;

	// ms5611 calibration coefficients for pressure and temperature calculations. 
	uint16_t pressure_sensitivity_;
	uint16_t pressure_offset_;
	uint16_t pres_sensitivity_temp_coeff_; // Temperature coefficient of pressure sensitivity.
	uint16_t pres_offset_temp_coeff_; // temperature coefficient of temperature offset.
	uint16_t reference_temperature_;
	uint16_t temp_coeff_of_temp_;     // Temperature coefficient of temperature.

	// barometer measurement variables.
	float base_elev_;
	float height_;
	float temp_;
	float pres_;

	HAL_StatusTypeDef spi_status_;

	/**
	* Sends the reset command to the barometer. The reset command makes sure that
	* the PROM, which holds calibration data, gets loaded into the device's internal register.
	 *
	* @return none
	 */
	void reset();

	/**
	* Populates the struct with calibration coefficients.
	 *
	* @return none
	 */
	void getConvCoeffs();

	/**
	* Reads PROM data from user-specified 16-bit address.
	 *
	* @return calibration data
	 */
	uint16_t promRead(uint8_t address);

	/**
	* Reads uncompenssated temperature/pressure value from altimeter.
	 *
	* @return uncompensated pressure/temperature value
	 */
	uint32_t readPressureTemperatureUncompensated(uint8_t conversion_command);

	/**
	* Calculates the temperature in degrees celsius and pressure in mbar.
	 *
	* @return none
	 */
	void calculateTemperatureAndPressure();
};


#endif /* INC_ALTIMETER_HPP_ */
