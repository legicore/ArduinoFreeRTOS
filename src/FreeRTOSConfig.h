/*
 * FreeRTOS Kernel V10.4.2
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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <avr/io.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/*
 * The Arduino port can use either the watchdog, timer 2 or timer 3 interrupt to
 * generate the system tick. Set configUSE_WATCHDOG_TICK to 1 to use the
 * watchdog, or 0 to use timer 2 or 3 (depending on configMCU_TIMER).
 */
#define configUSE_WATCHDOG_TICK                 0

/*
 * Timer 0 and 1 are used by the Arduino core library.
 * Timer 2 is not available to Arduino Leonardo.
 * Timer 3 is not available to Arduino Uno and (Pro) Mini.
 */
#define configMCU_TIMER                         2

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      ( ( uint32_t ) F_CPU )
#define configMAX_PRIORITIES                    ( 4 )
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 112 )
#define configMAX_TASK_NAME_LEN                 ( 8 )
#define configUSE_16_BIT_TICKS                  1
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   1
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  1
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     1 /* Arduino loop()! */
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          1 /* Debuggung */
#define configUSE_MALLOC_FAILED_HOOK            1 /* Debuggung */
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               2
#define configTIMER_QUEUE_LENGTH                5
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_xResumeFromISR                  0
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          0
#define INCLUDE_xTaskGetCurrentTaskHandle       0
#define INCLUDE_uxTaskGetStackHighWaterMark     1 /* Debuggung */
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        0
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              0

/* Set appropriate Heap size. */
#if defined( __AVR_ATmega640__  ) || defined( __AVR_ATmega1280__ ) || \
    defined( __AVR_ATmega1281__ ) || defined( __AVR_ATmega2560__ ) || \
    defined( __AVR_ATmega2561__ )

    /* Arduino Mega 2560 type. */
    #define configTOTAL_HEAP_SIZE               ( ( size_t ) 3072 )

#elif defined( __AVR_ATmega328P__ ) || defined( __AVR_ATmega168__ ) || \
      defined( __AVR_ATmega8__    )

    /* Arduino Uno type. */
    #define configTOTAL_HEAP_SIZE               ( ( size_t ) 1024 )

#elif defined( __AVR_ATmega32U4__ ) || defined( __AVR_ATmega16U4__ )

    /* Arduino Leonardo type. */
    #define configTOTAL_HEAP_SIZE               ( ( size_t ) 1280 )

#endif

/* Check the timer configuration and set the tick rate. */
#if configUSE_WATCHDOG_TICK == 1

    /* TODO */
    #define configTICK_RATE_HZ                  ( ( TickType_t ) 1000 / 15 )

#elif( configMCU_TIMER == 2 ) && \
    !defined( __AVR_ATmega32U4__ ) && !defined( __AVR_ATmega16U4__ )

    /* TODO */
    #define configTICK_RATE_HZ                  ( ( TickType_t ) 1000 )

#elif( configMCU_TIMER == 3 ) && \
    !defined( __AVR_ATmega328P__ ) && !defined( __AVR_ATmega168__ ) && \
    !defined( __AVR_ATmega8__    )

    /* TODO */
    #define configTICK_RATE_HZ                  ( ( TickType_t ) 1000 )

#elif( configMCU_TIMER == 0 ) || ( configMCU_TIMER == 1 )
    #error Timer 0 and 1 are used by the Arduino core library.
#else
    #error The selected timer is invalid or not available for this board type.
#endif


#endif /* FREERTOS_CONFIG_H */
