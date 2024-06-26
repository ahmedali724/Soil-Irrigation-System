#include <util/delay.h>

#include "HAL/LCD/lcd.h"
#include "HAL/KEYPAD/keypad.h"
#include "HAL/SOIL_SENSOR/SoilSensor.h"
#include "HAL/WATER_PUMP/waterPumb.h"

#include "MCAL/TIMER0/timer0.h"
#include "MCAL/ADC/adc.h"
#include "MCAL/UART/uart.h"
#include "MCAL/EXTERNAL_INTERRUPT/external_interrupt.h"
#include "MCAL/GPIO/gpio_private.h"

#include "LIB/common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PERIOD_LENGTH 2
#define NUMBER_OF_COMPARE_MTACHES_PER_SECOND 63
#define CTC_VALUE 250
#define CTC_INITIAL_VALUE 0
#define PERIOD_TO_SEND_DATA_THROUGH_UART 5
#define NUMBER_OF_SECONDS_FOR_PUMP 5

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

volatile uint8 g_Mode = 0;
volatile uint8 secondCounter = 0;
volatile uint8 adcValue = 0;
volatile uint8 g_ticks = 0;
volatile uint8 countSecondToActivePumpFlag = 0;
volatile uint8 numberOfSecondToActivePump = NUMBER_OF_SECONDS_FOR_PUMP;
volatile uint32 delay_period_copy = 0;
uint32 delay_period = 0;
uint8 percentageOfRead = 0;

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

/*
 * Description :
 * Function to active keypad to get input number from it.
 * Function input:  array of uint8 to save numbers from keypad on it.
 * Function output: void.
 */
void getPeriodFromKeypad(uint8 period[]);

/*
 * Description :
 * This is the call back function which will be executed each interrupt in INT0.
 * Function input:  void.
 * Function output: void.
 */
void INT0_Func(void);

/*
 * Description :
 * This is the call back function which will be executed each interrupt in INT1.
 * Function input:  void.
 * Function output: void.
 */
void INT1_Func(void);

/*
 * Description :
 * This is the call back function which will be executed each interrupt in TIMER0.
 * Function input:  void.
 * Function output: void.
 */
void TIMER0_Func(void);

int main(void)
{
	/* Configuration and initialization functions */

	/* array for the period which user will enter */
	uint8 entredPeriod[PERIOD_LENGTH];

	/* select the configuration of ADC */
	ADC_ConfigType ADC_Soil =
		{AVCC_5V, ADC_F_CPU_Pre_128};

	/* select the configuration of EXT0 */
	EXT_INT0_ConfigType EXT_INT0_Config =
		{FALLING_EDGE};

	/* select the configuration of EXT1 */
	EXT_INT1_ConfigType EXT_INT1_Config =
		{FALLING_EDGE};

	/* select the configuration of TIMER0 */
	TIMER0_ConfigType TIMER0_Config =
		{CTC_INITIAL_VALUE, CTC_VALUE, CTC_MODE, PRESCALER_1024,
		 NORMAL_MODE_OC0_DISCONNECTED};

	/* passing the configuration to initialization function of TIMER0 */
	Timer0_init(&TIMER0_Config);

	/* passing the configuration to initialization function of EXT0 */
	EXT_INT0_init(&EXT_INT0_Config);
	/* passing the configuration to initialization function of EXT01 */
	EXT_INT1_init(&EXT_INT1_Config);

	/* passing the configuration to initialization function of ADC */
	ADC_init(&ADC_Soil);

	/* setup the call back function */
	EXT_INT0_setCallBack(INT0_Func);
	/* setup the call back function */
	EXT_INT1_setCallBack(INT1_Func);

	/* setup the call back function */
	Timer0_setCallBack(TIMER0_Func);

	/* initialization function of UART with baud rate 9600 */
	UART_init(9600);

	/* call the initialization function of LCD */
	LCD_init();

	/* call the initialization function of Water Pump */
	WaterPumb_Init();

	/* global interrupt enable */
	SREG_REG.Bits.I_Bit = 1;

	for (;;)
	{
		/* if the selected mode is the first mode */
		while (g_Mode == 0)
		{
			/* to print the state of water pump and the percentage of ADC reading */
			LCD_displayStringRowColumn(0, 0, "Water Pump: ");
			LCD_displayStringRowColumn(1, 0, "Value: ");
			/* get the value from the ADC channel */
			adcValue = Soilsensor_getValue();
			/* calculate the percentage of reading */
			percentageOfRead = 100 - ((uint32)(adcValue * 100) / 255);
			/* display it in its place in LCD */
			LCD_moveCursor(1, 7);
			LCD_intgerToString(percentageOfRead);
			LCD_displayCharacter('%');
			/*
			 * check the state of reading, if less than or equal 50 then activate the water pump
			 * else turn off the water pump
			 */
			if (percentageOfRead <= 50)
			{
				LCD_displayStringRowColumn(0, 12, "ON");
				/*
				 * this to only this the character after ON is space
				 * when switch from OFF to ON we need to remove the last F latter
				 */
				LCD_displayCharacter(' ');
				WaterPumb_States(on);
			}
			else
			{
				LCD_displayStringRowColumn(0, 12, "OFF");
				WaterPumb_States(off);
			}
			/* this condition only for LCD display the specific digits from the data  */
			if (percentageOfRead < 100)
			{
				LCD_moveCursor(1, 10);
				LCD_displayCharacter(' ');
			}
			if (percentageOfRead < 10)
			{
				LCD_moveCursor(1, 9);
				LCD_displayCharacter(' ');
			}
		}
		/* if the selected mode is the second mode */
		while (g_Mode == 1)
		{
			/* turn off the pump */
			WaterPumb_States(off);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Enter Period");
			LCD_moveCursor(1, 0);
			/* wait for the user to enter the period */
			getPeriodFromKeypad(entredPeriod);
			_delay_ms(1500);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 6, "DONE");
			_delay_ms(1500);
			LCD_clearScreen();
			/* transform delay_period from minutes to seconds */
			delay_period = (entredPeriod[1] + (entredPeriod[0] * 10));
			delay_period *= 60;
			/* make a copy to repeat the operation each delay_period */
			delay_period_copy = delay_period;
			/* continue this mode, its mean we will start count the delay period after the user enter it */
			g_Mode++;
		}
		while (g_Mode == 2)
		{
			LCD_displayStringRowColumn(0, 0, "Timer : ");
			LCD_intgerToString(delay_period_copy);
			/* this condition only for LCD display the specific digits from the data  */
			if (delay_period_copy < 1000)
			{
				LCD_moveCursor(0, 11);
				LCD_displayCharacter(' ');
			}
			if (delay_period_copy < 100)
			{
				LCD_moveCursor(0, 10);
				LCD_displayCharacter(' ');
			}
			if (delay_period_copy < 10)
			{
				LCD_moveCursor(0, 9);
				LCD_displayCharacter(' ');
			}
			/* check if the counter become 0 which mean the period is done */
			if (delay_period_copy == 0)
			{
				/* check the state of ADC if the soil need water or not */
				adcValue = Soilsensor_getValue();
				percentageOfRead = 100 - ((uint32)(adcValue * 100) / 255);
				/* send the data to UART */
				UART_sendByte(percentageOfRead);
				/* if the percentage less than 50, then the soil need water */
				if (percentageOfRead <= 50)
				{
					/* active the pump */
					WaterPumb_States(on);
					/* make the flag of count one minute is true */
					countSecondToActivePumpFlag = 1;
					/* wait for one minute */
					while (numberOfSecondToActivePump != 0)
						;
					/* reset the counter ,flag and turn of the pump */
					numberOfSecondToActivePump = NUMBER_OF_SECONDS_FOR_PUMP;
					countSecondToActivePumpFlag = 0;
					WaterPumb_States(off);
				}
				/* repeat the operation each delay_period */
				delay_period_copy = delay_period;
			}
		}
	}
}

/*
 * Description :
 * Function to active keypad to get input number from it.
 * Function input:  array of uint8 to save numbers from keypad on it.
 * Function output: void.
 */
void getPeriodFromKeypad(uint8 period[])
{
	uint8 i, temp; /* i for the period length counter, temp for the number from user */
	i = 0;		   /* start from index 0 in the array */
	while (i < PERIOD_LENGTH)
	{
		/* take the input number form keypad */
		temp = KEYPAD_getPressedKey();
		/* delay between each press and the next */
		_delay_ms(250);
		/* if the input is not a number form 0 to 9 then repeat taking input step */
		/* ^ for the keys don't have a number or # */
		if (temp == '^' || temp == '#')
		{
			continue;
		}
		/* the input is a number, then save it to its index in array */
		period[i] = temp;
		/* display the the number in LCD */
		LCD_intgerToString(temp);
		/* increment the index */
		i++;
	}
	/* waiting for pressing enter key */
	while (KEYPAD_getPressedKey() != '#')
		;
}

/*
 * Description :
 * This is the call back function which will be executed each interrupt in INT0.
 * Function input:  void.
 * Function output: void.
 */
void INT0_Func(void)
{
	/* switch the mode for the first mode */
	g_Mode = 0;
}

/*
 * Description :
 * This is the call back function which will be executed each interrupt in INT1.
 * Function input:  void.
 * Function output: void.
 */
void INT1_Func(void)
{
	/* switch the mode for the second mode */
	g_Mode = 1;
}

/*
 * Description :
 * This is the call back function which will be executed each interrupt in TIMER0.
 * Function input:  void.
 * Function output: void.
 */
void TIMER0_Func(void)
{
	g_ticks++;
	/* we need a specific number of ticks for one second delay */
	if (g_ticks == NUMBER_OF_COMPARE_MTACHES_PER_SECOND)
	{
		/* if we in the second mode and not in the water pump activation mode */
		if (g_Mode == 2 && !countSecondToActivePumpFlag)
		{
			/* then decrement the delay period by 1 */
			delay_period_copy--;
		}
		/* if we in the first mode, we just need to send a data to UART every five second */
		if (g_Mode == 0)
		{
			secondCounter++;
			if (secondCounter == PERIOD_TO_SEND_DATA_THROUGH_UART)
			{
				/* send the data then reset the second counter */
				UART_sendByte(percentageOfRead);
				secondCounter = 0;
			}
		}
		/* this is the water pump activation mode in mode 2 */
		if (countSecondToActivePumpFlag)
		{
			numberOfSecondToActivePump--;
		}
		/* reset the ticks each one second */
		g_ticks = 0;
	}
}
