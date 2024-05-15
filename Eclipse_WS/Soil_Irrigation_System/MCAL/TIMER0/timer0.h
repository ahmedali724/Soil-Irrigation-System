/*
 ============================================================================
 Name        : timer0.h
 Module Name : TIMER0
 Author      : Ahmed Ali
 Date        : 25 Apr 2024
 Description : Header file for the AVR TIMER0 driver
 ============================================================================
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include "../../LIB/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define OC0_PORTID		PORTB_ID
#define OC0_PINID 		PIN3_ID

#define OC0_MAX_VALUE 	255

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	NORMAL_MODE, PHASE_CORRECT_MODE, CTC_MODE, FAST_PWM_MODE
} TIMER0_ModeType;

typedef enum
{
	NORMAL_MODE_OC0_DISCONNECTED,
	CTC_TOGGLE__PWM_RESERVED,
	CTC_CLEAR__PWM_NON_INVERTING,
	CTC_SET__PWM_INVERTING
} TIMER0_CompareOutputType;

typedef enum
{
	NO_CLOCK_SOURCE,
	PRESCALER_1,
	PRESCALER_8,
	PRESCALER_64,
	PRESCALER_256,
	PRESCALER_1024,
	EXTERNAL_SOURCE_FALLING_EDGE,
	EXTERNAL_SOURCE_RISING_EDGE
} TIMER0_PrescalerType;

typedef struct
{
	uint8 initial_value;
	uint8 compare_value; /* for CTC mode only */
	TIMER0_ModeType mode;
	TIMER0_PrescalerType prescaler;
	TIMER0_CompareOutputType outputMode;
} TIMER0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver.
 */
void Timer0_init(const TIMER0_ConfigType *Config_Ptr);

/*
 * Description :
 * Function to disable the Timer0.
 */
void Timer0_deInit(void);

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer0_setCallBack(void (*a_ptr)(void));

/*
 * Description:
 * Setup the compare value based on the required input duty cycle.
 */
void Timer0_PWM_Start(uint8 a_dutyCycle);

#endif /* TIMER0_H_ */
