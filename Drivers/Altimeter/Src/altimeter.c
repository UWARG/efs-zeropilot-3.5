/*
 * altimeter.c
 *
 *  Created on: Jul 30, 2023
 *      Author: folafatola
 */


#include <altimeter.h>


void altimeterInit(GPIO_TypeDef *port, uint16_t cs_pin){
	reset(port, cs_pin);
}

long getTemperature(GPIO_TypeDef *port, uint16_t cs_pin){

	long temperature = calculateTemperature(port, cs_pin);

	return temperature;
}

long getPressure(GPIO_TypeDef *port, uint16_t cs_pin){

	long pressure = calculatePressure(port, cs_pin);

	return pressure;
}
