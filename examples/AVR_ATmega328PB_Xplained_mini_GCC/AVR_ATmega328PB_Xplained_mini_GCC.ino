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

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Tests. */
#include "regtest.h"
#include "integer.h"
#include "PollQ.h"
#include "partest.h"

/*-----------------------------------------------------------*/

/* Priority definitions for most of the tasks in the demo application. */
#define mainCHECK_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
#define mainQUEUE_POLL_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define mainLED_BLINK_PRIORITY			( tskIDLE_PRIORITY + 2 )

/* The period between executions of the check task. */
#define mainCHECK_PERIOD				( ( TickType_t ) 1000  )

/* The period to toggle LED. */
#define mainBLINK_LED_OK_HALF_PERIOD	( ( TickType_t ) 100 )

/* The task function for the "Check" task. */
static void vErrorChecks( void *pvParameters );

/* The task function for blinking LED at a certain frequency. */
static void vBlinkOnboardUserLED( void *pvParameters );

/*-----------------------------------------------------------*/

void setup( void )
{
    /* Initialize the serial port. */
    Serial.begin( 9600 );

    vParTestInitialise();

	/* Standard register test. */
	vStartRegTestTasks();

	/* Optionally enable below tests. This port only has 2KB RAM. */
	vStartIntegerMathTasks( tskIDLE_PRIORITY );
	vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
	xTaskCreate( vBlinkOnboardUserLED, "LED", 50, NULL, mainCHECK_TASK_PRIORITY, NULL );

	/* Create the tasks defined within this file. */
	xTaskCreate( vErrorChecks, "Check", configMINIMAL_STACK_SIZE, NULL, mainLED_BLINK_PRIORITY, NULL );

	/* In this port, to use preemptive scheduler define configUSE_PREEMPTION
	as 1 in portmacro.h.  To use the cooperative scheduler define
	configUSE_PREEMPTION as 0. */
	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

void loop( void )
{
    /* The Arduino loop function is used as the idle hook. In FreeRTOSConfig.h
    configUSE_IDLE_HOOK must be set because there are (serial) events that were
    processed in the backround of the Arduino core implementation of loop(). */
}
/*-----------------------------------------------------------*/

static void vErrorChecks( void *pvParameters )
{
static UBaseType_t uxErrorHasOccurred = 0;
BaseType_t xFirstTimeCheck = pdTRUE;

	/* The parameters are not used. */
	( void ) pvParameters;

	/* Cycle for ever, delaying then checking all the other tasks are still
	operating without error. */
	for( ;; )
	{
		if( xAreRegTestTasksStillRunning() != pdTRUE )
		{
			uxErrorHasOccurred |= 0x01U ;
		}
		if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
		{
			uxErrorHasOccurred |= ( 0x01U << 1);
		}
		if( xArePollingQueuesStillRunning() != pdTRUE )
		{
			uxErrorHasOccurred |= ( 0x01U << 2);
		}

		/* When check task runs before any other tasks, all above checks shall fail.
		To avoid false alarm, clear errors upon first entry. */
		if ( xFirstTimeCheck == pdTRUE )
		{
			uxErrorHasOccurred = 0;
			xFirstTimeCheck = pdFALSE;
        }
        else
        {
            Serial.println( uxErrorHasOccurred, BIN );
        }

		/* Could set break point at below line to verify uxErrorHasOccurred. */
		vTaskDelay( mainCHECK_PERIOD );
	}
}
/*-----------------------------------------------------------*/

static void vBlinkOnboardUserLED( void *pvParameters )
{
	/* The parameters are not used. */
	( void ) pvParameters;

	/* Cycle forever, blink onboard user LED at a certain frequency. */
	for( ;; )
	{
		vParTestToggleLED( 0 );

		vTaskDelay( mainBLINK_LED_OK_HALF_PERIOD );
	}

}
