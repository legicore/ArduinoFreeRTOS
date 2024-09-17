////////////////////////////////////////////////////////////////////////////////
/**
 * @file        port.c
 * 
 * @author      Martin Legleiter
 * 
 * @brief       https://www.freertos.org/a00110.html
 * 
 * @copyright   (c) 2024 Martin Legleiter
 * 
 * @license     Use of this source code is governed by an MIT-style
 *              license that can be found in the LICENSE file or at
 *              @see https://opensource.org/licenses/MIT.
 */
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include <Arduino.h>

/*-----------------------------------------------------------*/

#if configUSE_IDLE_HOOK == 1

    void vApplicationIdleHook( void )
    {
        loop();

        extern void serialEventRun( void ) __attribute__ ( ( weak ) );
        if( serialEventRun )
        {
            serialEventRun();
        }
    }

#endif
/*-----------------------------------------------------------*/

#if configUSE_MALLOC_FAILED_HOOK == 1

    void vApplicationMallocFailedHook( void )
    {
        pinMode( LED_BUILTIN, OUTPUT );

        for( ;; )
        {
            digitalWrite( LED_BUILTIN, HIGH );
            delay( 50 );
            digitalWrite( LED_BUILTIN, LOW );
            delay( 100 );
            digitalWrite( LED_BUILTIN, HIGH );
            delay( 50 );
            digitalWrite( LED_BUILTIN, LOW );
            delay( 800 );
        }
    }

#endif
/*-----------------------------------------------------------*/

#if configCHECK_FOR_STACK_OVERFLOW > 0

    void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
    {
        ( void ) xTask;
        ( void ) pcTaskName;

        pinMode( LED_BUILTIN, OUTPUT );

        for( ;; )
        {
            digitalWrite( LED_BUILTIN, HIGH );
            delay( 50 );
            digitalWrite( LED_BUILTIN, LOW );
            delay( 100 );
            digitalWrite( LED_BUILTIN, HIGH );
            delay( 50 );
            digitalWrite( LED_BUILTIN, LOW );
            delay( 100 );
            digitalWrite( LED_BUILTIN, HIGH );
            delay( 50 );
            digitalWrite( LED_BUILTIN, LOW );
            delay( 650 );
        }
    }

#endif
