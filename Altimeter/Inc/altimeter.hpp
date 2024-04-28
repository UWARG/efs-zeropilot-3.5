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


class MS5611{
public:
	/**
	* Constructor for the MS5611 class.
	* It assigns the spi_handler and ports and pins related to the chip select and protocol pins.
	* This function resets the device, populates reads calibration data from the device's PROM
	* PROM, and gets the sensor's initial elevation above sea level.
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
	* Calculates the pressure in mbar and assigns it to the variable passed by
	* reference.
	 *
	* @param pressure -> reference to pressure variable
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool getPressure(float &pressure);

	/**
	* Calculates the temperature in celsius and assigns it to the variable passed by
	* reference.
	 *
	* @param temperature -> reference to temperature variable
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool getTemperature(float &temperature);

	/**
	* Calculates the height above sea level in meters and assigns it to the variable passed by
	* reference.
	 *
	* @param altitude_above_sea_level -> reference to altitude variable
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool getAltitudeAboveSeaLevel(float &altitude_above_sea_level);

	/**
	* Calculates the height above ground level in meters and assigns it to the variable passed by
	* reference.
	 *
	* @param altitude_above_ground_level -> reference to altitude variable
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool getAltitudeAboveGroundLevel(float &altitude_above_ground_level);


private:

	//pointer for spi handler.
	SPI_HandleTypeDef *spi_handle_;

	//chip select and protocol select ports and pins.
	GPIO_TypeDef *cs_port_;
	GPIO_TypeDef *ps_port_;
	uint16_t cs_pin_;
	uint16_t ps_pin_;

	//Holds the barometer PROM data.
	uint16_t ms6511_prom_data_[8];

	//barometer measurement variables.
	float base_elev_;
	float temp_;
	float pres_;

	//hal_spi_transmitreceive status
	HAL_StatusTypeDef spi_status_;

	//to track communication failures
	bool communication_success_;

	/**
	* Sends the reset command to the barometer. The reset command makes sure that
	* the PROM, which holds calibration data, gets loaded into the device's internal register.
	 *
	* @return none
	 */
	void reset();

	/**
	* Populates ms6511_prom_data_ with data from PROM
	 *
	* @return none
	 */
	void getPromData();

	/**
	* Reads PROM data from user-specified 16-bit address.
	 *
	* @param address -> The address from which you want to read data.
	* @param ms5611_prom_data -> pointer to ms5611_prom_data array.
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool promRead(uint8_t address, uint16_t *ms5611_prom_data);

	/**
	* Reads uncompensated temperature/pressure value from altimeter.
	 *
	* @param conversion_command -> command used to fetch digital temperature/pressure
	* @param uncompensated_pressure_temperature -> reference to uncompensated pressure/temperature value
	 *
	* @return bool for if there were no communication failures with barometer.
	 */
	bool readPressureTemperatureUncompensated(uint8_t conversion_command, uint32_t &uncompensated_pressure_temperature);

	/**
	* Calculates the temperature in degrees celsius and pressure in mbar.
	 *
	* @return none
	 */
	void calculateTemperatureAndPressure();
};


#endif /* INC_ALTIMETER_HPP_ */

