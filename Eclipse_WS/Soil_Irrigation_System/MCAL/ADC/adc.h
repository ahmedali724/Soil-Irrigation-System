/*
 ===================================================
 Author    : Abdel Rhman Helal Saleh
 File Name : adc.h
 Data      : Oct 8, 2023
 Time      : 9:48:45 PM
 ===================================================
 */
#ifndef ADC_H_
#define ADC_H_

#include "../../LIB/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   5

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	AREF_External, AVCC_5V, Reserved, Internal_2_56V
} ADC_ReferenceVolatge;

typedef enum
{
	ADC_F_CPU_Pre_2,
	ADC_F_CPU_Pre_2_too,
	ADC_F_CPU_Pre_4,
	ADC_F_CPU_Pre_8,
	ADC_F_CPU_Pre_16,
	ADC_F_CPU_Pre_32,
	ADC_F_CPU_Pre_64,
	ADC_F_CPU_Pre_128
} ADC_Prescaler;

typedef struct
{
	ADC_ReferenceVolatge ref_volt;
	ADC_Prescaler prescaler;
} ADC_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(const ADC_ConfigType *Config_Ptr);
/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */
