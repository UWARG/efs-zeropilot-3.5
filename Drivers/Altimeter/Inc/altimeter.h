/*
 * altimeter.h
 *
 *  Created on: Jul 27, 2023
 *      Author: folafatola
 */

#ifndef INC_ALTIMETER_H_
#define INC_ALTIMETER_H_

#include <altimeter_extract.h>

//typedef struct{
//	long temp;
//	long pressure;
//
//} Altimeter;

void altimeterInit(GPIO_TypeDef *port, uint16_t cs_pin);

/**
* This function returns the temperature value read by the altimeter sensor.
 *
* @param port, the port of the pin connected to the CS pin of the altimeter.
 *
* @param cs_pin, the pin of the altimeter
 *
* @returns temperature value.
 */

long getTemperature(GPIO_TypeDef *port, uint16_t cs_pin);


/**
* This function returns the pressure value read by the altimeter sensor.
 *
* @param port, the port of the pin connected to the CS pin of the altimeter.
 *
* @param cs_pin, the pin of the altimeter
 *
* @returns pressure value.
 */

long getPressure(GPIO_TypeDef *port, uint16_t cs_pin);

//How do I calculate the height off of temperature and pressure???
long getHeight(GPIO_TypeDef *port, uint16_t cs_pin);




#endif /* INC_ALTIMETER_H_ */
