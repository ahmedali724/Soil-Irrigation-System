/*
 ===================================================
 Author    : Abdel Rhman Helal Saleh
 File Name : waterPumb.c
 Data      : Apr 17, 2024
 Time      : 7:22:30 PM
 ===================================================
 */
#include "WaterPumb.h"
#include "../../LIB/common_macros.h"
#include "../../MCAL/GPIO/gpio.h"

void WaterPumb_Init(void)
{
	/*out pin*/
	GPIO_setupPinDirection(WaterPumb_PORT, WaterPumb_PIN, PIN_OUTPUT);
	/*Stop*/
	GPIO_writePin(WaterPumb_PORT, WaterPumb_PIN, LOGIC_LOW);
}

void WaterPumb_States(WaterPumb_State state)
{
	if (state == on)
	{
		GPIO_writePin(WaterPumb_PORT, WaterPumb_PIN, LOGIC_HIGH);
	}
	else
	{
		GPIO_writePin(WaterPumb_PORT, WaterPumb_PIN, LOGIC_LOW);
	}
}
