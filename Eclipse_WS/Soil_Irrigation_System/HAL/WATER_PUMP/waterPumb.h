/*
 ===================================================
 Author    : Abdel Rhman Helal Saleh
 File Name : waterPumb.h
 Data      : Apr 17, 2024
 Time      : 7:22:37 PM
 ===================================================
 */

#ifndef WATERPUMB_H_
#define WATERPUMB_H_

#include "../../LIB/std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define WaterPumb_PORT PORTC_ID
#define WaterPumb_PIN PIN4_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	off, on
} WaterPumb_State;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void WaterPumb_Init(void);

void WaterPumb_States(WaterPumb_State state);

#endif /* WATERPUMB_H_ */
