/*
 ============================================================================
 Name        : timer0.c
 Module Name : TIMER0
 Author      : Ahmed Ali
 Date        : 25 Apr 2024
 Description : Source file for the AVR TIMER0 driver
 ============================================================================
 */

#include "timer0.h"
#include "timer0_private.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver.
 */
void Timer0_init(const TIMER0_ConfigType *Config_Ptr)
{
	/************************** TCCR0 Description **************************
	 * FOC0:      Force Output Compare for Compare unit (non-PWM mode)
	 * WGM01:0    Waveform Generation Mode, selected in configuration
	 * COM01:0    Compare Match Output Mode, selected in configuration
	 * CS02:0     Clock Select, selected in configuration
	 ***********************************************************************/

	/*
	 * insert the required mode in WGM bits (WGM00 and WGM01) of TCCR0 Register
	 */
	if (Config_Ptr->mode == NORMAL_MODE)
	{
		TCCR0_REG.Bits.FOC0_Bit = 1;
		TCCR0_REG.Bits.WGM00_Bit = 0;
		TCCR0_REG.Bits.WGM01_Bit = 0;
	}
	if (Config_Ptr->mode == CTC_MODE)
	{
		TCCR0_REG.Bits.FOC0_Bit = 1;
		TCCR0_REG.Bits.WGM00_Bit = 0;
		TCCR0_REG.Bits.WGM01_Bit = 1;
	}
	if (Config_Ptr->mode == FAST_PWM_MODE)
	{
		TCCR0_REG.Bits.FOC0_Bit = 0;
		TCCR0_REG.Bits.WGM00_Bit = 1;
		TCCR0_REG.Bits.WGM01_Bit = 1;
	}

	/*
	 * insert the required compare output mode in COM bits (COM00 and COM01) of TCCR0 Register
	 */
	if (Config_Ptr->outputMode == NORMAL_MODE_OC0_DISCONNECTED)
	{
		TCCR0_REG.Bits.COM00_Bit = 0;
		TCCR0_REG.Bits.COM01_Bit = 0;
	}
	if (Config_Ptr->outputMode == CTC_TOGGLE__PWM_RESERVED)
	{
		TCCR0_REG.Bits.COM00_Bit = 1;
		TCCR0_REG.Bits.COM01_Bit = 0;
	}
	if (Config_Ptr->outputMode == CTC_CLEAR__PWM_NON_INVERTING)
	{
		TCCR0_REG.Bits.COM00_Bit = 0;
		TCCR0_REG.Bits.COM01_Bit = 1;
	}
	if (Config_Ptr->outputMode == CTC_SET__PWM_INVERTING)
	{
		TCCR0_REG.Bits.COM00_Bit = 1;
		TCCR0_REG.Bits.COM01_Bit = 1;
	}

	/*
	 * insert the required prescaler in CS bits (CS00, CS01 and CS02) of TCCR0 Register
	 */
	TCCR0_REG.Byte = (TCCR0_REG.Byte & 0xF8) | (Config_Ptr->prescaler);

	TCNT0_REG = Config_Ptr->initial_value;
	OCR0_REG = Config_Ptr->compare_value;

	/* Enable Timer/Counter0 Output Compare Match interrupt */
	TIMSK_REG.Bits.OCIE0_Bit = 1;

	/* Enable Timer/Counter0 Overflow Interrupt */
	TIMSK_REG.Bits.TOIE0_Bit = 1;
}

/*
 * Description :
 * Function to disable the Timer0.
 */
void Timer0_deInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR0_REG.Byte = 0;
	TCNT0_REG = 0;
	OCR0_REG = 0;

	/* Disable the Output Compare A match and Overflow interrupt */
	TIMSK_REG.Bits.OCIE0_Bit = 0;
	TIMSK_REG.Bits.TOIE0_Bit = 0;

	/* Reset the global pointer value */
	g_callBackPtr = NULL_PTR;
}

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer0_setCallBack(void (*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/*
 * Description:
 * Setup the compare value based on the required input duty cycle.
 */
void Timer0_PWM_Start(uint8 a_dutyCycle)
{
	/* If the input duty cycle greater than 100, then set it to 100 */
	if (a_dutyCycle > 100)
	{
		a_dutyCycle = 100;
	}
	/* If the input duty cycle less than 0, then set it to 0 */
	if (a_dutyCycle < 0)
	{
		a_dutyCycle = 0;
	}
	OCR0_REG = (uint8) ((uint16) a_dutyCycle * OC0_MAX_VALUE / 100);
}
