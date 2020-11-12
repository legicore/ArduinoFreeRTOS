/*
 * FreeRTOS V202011.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include "FreeRTOS.h"
#include "task.h"
#include "partest.h"

#include <Arduino.h>

/* 
 * ATmega328PB Xplained Mini board has a user LED at PB5. 
 * Everything below is specific for this setup only.
 * LED is lit when PB5 is set to a high.
 */
#define partestLED_ON HIGH

static volatile uint8_t uCurrentLedOutputVal = ~partestLED_ON;

/*-----------------------------------------------------------*/

void vParTestInitialise( void )
{
	/* Turn on user LED. This function is not thread safe. */
	pinMode( LED_BUILTIN, OUTPUT );
	uCurrentLedOutputVal = ~partestLED_ON;
}

/*-----------------------------------------------------------*/

void vParTestSetLED( UBaseType_t uxLED, BaseType_t xValue )
{
	/* There's only one LED on this board. */
	( void ) uxLED;
	
	/* Turn on user LED. 
	The compound action is guaranteed to be not interrupted by other tasks. */
	vTaskSuspendAll();
	
	if ( xValue == 0 )
	{
		/* Turn off, only when input value is zero. */
		digitalWrite( LED_BUILTIN, LOW );
		uCurrentLedOutputVal = ~partestLED_ON;
	}
	else
	{
		/* Turn on, when input value is none zero. */
		digitalWrite( LED_BUILTIN, HIGH );
		uCurrentLedOutputVal = partestLED_ON;
	}
	
	xTaskResumeAll();
}

/*-----------------------------------------------------------*/

void vParTestToggleLED( UBaseType_t uxLED )
{
	/* There's only one LED on this board. */
	( void ) uxLED;
	
	/* Toggle user LED. 
	The compound action is guaranteed to be not interrupted by other tasks. */
	vTaskSuspendAll();
	
	if ( uCurrentLedOutputVal == partestLED_ON )
	{
		/* Turn off. */
		digitalWrite( LED_BUILTIN, LOW );
		uCurrentLedOutputVal = ~partestLED_ON;
	}
	else
	{
		/* Turn on. */
		digitalWrite( LED_BUILTIN, HIGH );
		uCurrentLedOutputVal = partestLED_ON;
	}
		
	xTaskResumeAll();
}
