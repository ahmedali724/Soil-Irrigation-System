/*
 ===================================================
 Author    : Abdel Rhman Helal Saleh
 File Name : SoilSensor.c
 Data      : Apr 13, 2024
 Time      : 7:27:06 PM
 ===================================================
 */

#include "../../LIB/std_types.h"
#include "SoilSensor.h"
#include "../../MCAL/ADC/adc.h"

uint16 Soilsensor_getValue(void)
{
	uint16 adc_value = 0;
	adc_value = ADC_readChannel(SENSOR_CHANNEL_ID);
	return adc_value;
}
