////////////////////////////////////////////////////////////////////////////////
/**
 * @file        FreeRTOSConfig.h
 * 
 * @author      Martin Legleiter
 * 
 * @brief       https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/examples/template_configuration/FreeRTOSConfig.h
 *              https://www.freertos.org/a00110.html
 * 
 * @copyright   (c) 2024 Martin Legleiter
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

/* Hardware description related definitions. */
#define configCPU_CLOCK_HZ                          ( ( unsigned long ) F_CPU )

/* Scheduling behaviour related definitions. */
// #define configTICK_RATE_HZ                          100 /* Device specific */
#define configUSE_PREEMPTION                        1
#define configUSE_TIME_SLICING                      0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     0
#define configUSE_TICKLESS_IDLE                     0
#define configMAX_PRIORITIES                        5
#define configMINIMAL_STACK_SIZE                    128
#define configMAX_TASK_NAME_LEN                     16
#define configTICK_TYPE_WIDTH_IN_BITS               TICK_TYPE_WIDTH_16_BITS
#define configIDLE_SHOULD_YIELD                     1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES       1
#define configQUEUE_REGISTRY_SIZE                   0
#define configENABLE_BACKWARD_COMPATIBILITY         0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
#define configUSE_MINI_LIST_ITEM                    1
#define configSTACK_DEPTH_TYPE                      size_t
#define configMESSAGE_BUFFER_LENGTH_TYPE            size_t
#define configHEAP_CLEAR_MEMORY_ON_FREE             1
#define configSTATS_BUFFER_MAX_LENGTH               0xFFFF
#define configUSE_NEWLIB_REENTRANT                  0

/* Software timer related definitions. */
#define configUSE_TIMERS                            1
#define configTIMER_TASK_PRIORITY                   ( configMAX_PRIORITIES - 1 )
#define configTIMER_TASK_STACK_DEPTH                configMINIMAL_STACK_SIZE
#define configTIMER_QUEUE_LENGTH                    10

/* Event Group related definitions. */
#define configUSE_EVENT_GROUPS                      0

/* Stream Buffer related definitions. */
#define configUSE_STREAM_BUFFERS                    0

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION             1
#define configSUPPORT_DYNAMIC_ALLOCATION            1
// #define configTOTAL_HEAP_SIZE                       4096 /* Device specific */
#define configAPPLICATION_ALLOCATED_HEAP            0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   0
#define configENABLE_HEAP_PROTECTOR                 0

/* Interrupt nesting behaviour configuration. */
// #define configKERNEL_INTERRUPT_PRIORITY             0
// #define configMAX_SYSCALL_INTERRUPT_PRIORITY        0
// #define configMAX_API_CALL_INTERRUPT_PRIORITY       0

/* Hook and callback function related definitions. */
#define configUSE_IDLE_HOOK                         1 /* Arduino loop() */
#define configUSE_TICK_HOOK                         0
#define configUSE_MALLOC_FAILED_HOOK                1 /* Debugging */
#define configUSE_DAEMON_TASK_STARTUP_HOOK          0
#define configUSE_SB_COMPLETED_CALLBACK             0
#define configCHECK_FOR_STACK_OVERFLOW              1 /* Debugging */

/* Run time and task stats gathering related definitions. */
// #define configGENERATE_RUN_TIME_STATS               0
// #define configUSE_TRACE_FACILITY                    0
// #define configUSE_STATS_FORMATTING_FUNCTIONS        0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             1

/* Debugging assistance. */
#define configASSERT( x )         \
    if( ( x ) == 0 )              \
    {                             \
        taskDISABLE_INTERRUPTS(); \
        for( ; ; )                \
        ;                         \
    }

/* FreeRTOS MPU specific definitions. */
// #define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS  0
// #define configTOTAL_MPU_REGIONS                     8
// #define configTEX_S_C_B_FLASH                       0x07UL
// #define configTEX_S_C_B_SRAM                        0x07UL
// #define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY 1
// #define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS  0
// #define configUSE_MPU_WRAPPERS_V1                   0
// #define configPROTECTED_KERNEL_OBJECT_POOL_SIZE     10
// #define configSYSTEM_CALL_STACK_SIZE                128
// #define configENABLE_ACCESS_CONTROL_LIST            1

/* SMP( Symmetric MultiProcessing ) Specific Configuration definitions. */
// #define configRUN_MULTIPLE_PRIORITIES               0
// #define configUSE_CORE_AFFINITY                     0
// #define configTASK_DEFAULT_CORE_AFFINITY            tskNO_AFFINITY
// #define configUSE_TASK_PREEMPTION_DISABLE           0
// #define configUSE_PASSIVE_IDLE_HOOK                 0
// #define configTIMER_SERVICE_TASK_CORE_AFFINITY      tskNO_AFFINITY

/* ARMv8-M secure side port related definitions. */
// #define secureconfigMAX_SECURE_CONTEXTS             5
#define configKERNEL_PROVIDED_STATIC_MEMORY         1 /* Use kernel implementation of vApplicationGetIdleTaskMemory() and vApplicationGetTimerTaskMemory(). */

/* ARMv8-M port Specific Configuration definitions. */
// #define configENABLE_TRUSTZONE                      1
// #define configRUN_FREERTOS_SECURE_ONLY              1
// #define configENABLE_MPU                            1
// #define configENABLE_FPU                            1
// #define configENABLE_MVE                            1

/* ARMv7-M and ARMv8-M port Specific Configuration definitions. */
// #define configCHECK_HANDLER_INSTALLATION            1

/* Definitions that include or exclude functionality. */
#define configUSE_TASK_NOTIFICATIONS                1
#define configUSE_MUTEXES                           1 /* Should be used for Serial prints in different tasks! */
#define configUSE_RECURSIVE_MUTEXES                 0
#define configUSE_COUNTING_SEMAPHORES               0
#define configUSE_QUEUE_SETS                        0
#define configUSE_APPLICATION_TASK_TAG              0
#define configUSE_POSIX_ERRNO                       0

/* Set the following INCLUDE_* constants to 1 to incldue the named API function,
 * or 0 to exclude the named API function.  Most linkers will remove unused
 * functions even when the constant is 1. */
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
    defined( ARDUINO_AVR_PRO )

    /* These devices are using the watchdog timer interrupt to generate the
    system ticks, which results in a different/lower tick rate. */
    #define configTICK_RATE_HZ          ( ( TickType_t ) 1000 / portTICK_PERIOD_MS )

#elif defined( ARDUINO_AVR_MEGA2560 )

    /* When set to 0, the Watchdog timer interrupt is used to generate the
     * system ticks.
     *
     * INFO :   Some playground libraries can also use Timer5. Make sure that
     *          none of your used libraries also use Timer5 to prevent wrong
     *          configuration and/or cross usage.
     */
    #define portUSE_TIMER5              1

    #if( portUSE_TIMER5 == 1 )
        #define configTICK_RATE_HZ      ( ( TickType_t ) 1000 )
    #else
        #define configTICK_RATE_HZ      ( ( TickType_t ) 1000 / portTICK_PERIOD_MS )
    #endif

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
      defined( ARDUINO_AVR_NANO_EVERY )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 2048 )

#elif defined( ARDUINO_AVR_UNO_WIFI_REV2 )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 3072 )

#elif defined( ARDUINO_MINIMA ) || \
      defined( ARDUINO_UNOWIFIR4 )

    #define configTOTAL_HEAP_SIZE   ( ( size_t ) 4096 )

#endif
/*-----------------------------------------------------------*/

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* __FREERTOS_CONFIG_H__ */
