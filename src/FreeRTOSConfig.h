////////////////////////////////////////////////////////////////////////////////
/**
 * @file        FreeRTOSConfig.h
 * 
 * @author      Martin Legleiter
 * 
 * @brief       https://www.freertos.org/a00110.html
 * 
 * @copyright   (c) 2023 Martin Legleiter
 * 
 * @license     Use of this source code is governed by an MIT-style
 *              license that can be found in the LICENSE file or at
 *              @see https://opensource.org/licenses/MIT.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __FREERTOS_CONFIG_H__
#define __FREERTOS_CONFIG_H__

#include <stdlib.h>
#include <stdint.h>

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "FreeRTOS.h"

#include <Arduino.h>

/*-----------------------------------------------------------*/

#if defined( ARDUINO_MINIMA ) || \
    defined( ARDUINO_UNOWIFIR4 )

    #include "bsp_api.h"

    #define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    ( 1 )
    #define configMAX_SYSCALL_INTERRUPT_PRIORITY            ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - __NVIC_PRIO_BITS ) )

#endif
/*-----------------------------------------------------------*/

#define configUSE_PREEMPTION                        1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     0
#define configUSE_TICKLESS_IDLE                     0
#define configCPU_CLOCK_HZ                          ( ( uint32_t ) F_CPU )
//#define configTICK_RATE_HZ
#define configMAX_PRIORITIES                        ( 4 )
#define configMINIMAL_STACK_SIZE                    ( ( uint16_t ) 192 )
#define configMAX_TASK_NAME_LEN                     ( 8 )
#define configUSE_16_BIT_TICKS                      1 /* Must be 0 for pdMS_TO_TICKS() to work correctly with big delay values! */
#define configIDLE_SHOULD_YIELD                     1
#define configUSE_TASK_NOTIFICATIONS                1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES       1
#define configUSE_MUTEXES                           1 /* Should be used for Serial prints in different tasks! */
#define configUSE_RECURSIVE_MUTEXES                 0
#define configUSE_COUNTING_SEMAPHORES               0
#define configUSE_ALTERNATIVE_API                   0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE                   0
#define configUSE_QUEUE_SETS                        0
#define configUSE_TIME_SLICING                      0
#define configUSE_NEWLIB_REENTRANT                  0
#define configENABLE_BACKWARD_COMPATIBILITY         0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
#define configSTACK_DEPTH_TYPE                      uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE            size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION             1
#define configSUPPORT_DYNAMIC_ALLOCATION            1
//#define configTOTAL_HEAP_SIZE
#define configAPPLICATION_ALLOCATED_HEAP            0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                         1 /* Arduino loop() */
#define configUSE_TICK_HOOK                         0
#define configCHECK_FOR_STACK_OVERFLOW              1 /* Debugging */
#define configUSE_MALLOC_FAILED_HOOK                1 /* Debugging */
#define configUSE_DAEMON_TASK_STARTUP_HOOK          0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS               0
#define configUSE_TRACE_FACILITY                    0
#define configUSE_STATS_FORMATTING_FUNCTIONS        0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             1

/* Software timer related definitions. */
#define configUSE_TIMERS                            1
#define configTIMER_TASK_PRIORITY                   ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                    ( 5 )
#define configTIMER_TASK_STACK_DEPTH                configMINIMAL_STACK_SIZE

/* Define to trap errors during development. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                    0
#define INCLUDE_uxTaskPriorityGet                   0
#define INCLUDE_vTaskDelete                         1
#define INCLUDE_vTaskSuspend                        1
#define INCLUDE_xResumeFromISR                      0
#define INCLUDE_vTaskDelayUntil                     1
#define INCLUDE_vTaskDelay                          1
#define INCLUDE_xTaskGetSchedulerState              0
#define INCLUDE_xTaskGetCurrentTaskHandle           1
#define INCLUDE_uxTaskGetStackHighWaterMark         1 /* Debugging */
#define INCLUDE_xTaskGetIdleTaskHandle              1 /* Debugging */
#define INCLUDE_eTaskGetState                       0
#define INCLUDE_xEventGroupSetBitFromISR            0
#define INCLUDE_xTimerPendFunctionCall              0
#define INCLUDE_xTaskAbortDelay                     0
#define INCLUDE_xTaskGetHandle                      0
#define INCLUDE_xTaskResumeFromISR                  0

/*-----------------------------------------------------------*/

/* Set the tick rate for the supported devices. */
#if defined( ARDUINO_AVR_UNO ) || \
    defined( ARDUINO_AVR_LEONARDO ) || \
    defined( ARDUINO_AVR_MEGA2560 ) || \
    defined( ARDUINO_AVR_PRO )

    /* These devices are using the watchdog timer interrupt to generate the
    system ticks, which results in a different/lower tick rate. */
    #define configTICK_RATE_HZ          ( ( TickType_t ) 1000 / portTICK_PERIOD_MS )

#elif defined( ARDUINO_MINIMA ) || \
      defined( ARDUINO_UNOWIFIR4 )

    #define configTICK_RATE_HZ          ( ( TickType_t ) 1000 )

#elif defined( ARDUINO_AVR_NANO_EVERY ) || \
      defined( ARDUINO_AVR_UNO_WIFI_REV2 )

    /* INFO: The Arduino megaavr core implementation uses Timer0 for PWM pin 6,
    Timer1 for PWM pin 3 and the tone() function, Timer2 for the servo library
    and Timer3 for the functions millis(), micros() and delay(). */
    #define configUSE_TIMER_INSTANCE    4
    #define configTICK_RATE_HZ          ( ( TickType_t ) 1000 )

#endif
/*-----------------------------------------------------------*/

/* Set appropriate heap size for the supported devices. */
#if defined( ARDUINO_AVR_UNO ) || \
    defined( ARDUINO_AVR_PRO )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 768 )

#elif defined( ARDUINO_AVR_LEONARDO )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 1024 )

#elif defined( ARDUINO_AVR_MEGA2560 ) || \
      defined( ARDUINO_AVR_NANO_EVERY ) || \
      defined( ARDUINO_AVR_UNO_WIFI_REV2 )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 2048 )

#elif defined( ARDUINO_MINIMA ) || \
      defined( ARDUINO_UNOWIFIR4 )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 8192 )

#endif
/*-----------------------------------------------------------*/

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* __FREERTOS_CONFIG_H__ */
