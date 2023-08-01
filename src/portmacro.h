////////////////////////////////////////////////////////////////////////////////
/**
 * @file        portmacro.h
 * 
 * @author      Martin Legleiter
 * 
 * @brief       TODO
 * 
 * @copyright   (c) 2023 Martin Legleiter
 * 
 * @license     Use of this source code is governed by an MIT-style
 *              license that can be found in the LICENSE file or at
 *              @see https://opensource.org/licenses/MIT.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __PORTMACRO_H__
#define __PORTMACRO_H__

/*-----------------------------------------------------------*/

#if defined( ARDUINO_AVR_UNO ) || \
    defined( ARDUINO_AVR_MEGA2560 ) || \
    defined( ARDUINO_AVR_LEONARDO ) || \
    defined( ARDUINO_AVR_PRO )

    #include "ATmega/portmacro.h"

#elif defined( ARDUINO_MINIMA ) || \
      defined( ARDUINO_UNOWIFIR4 )

    /* INFO: This port is a modified copy of Arduino_FreeRTOS (v1.0.0) from the
    ArduinoCore-renesas BSP:
    https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/Arduino_FreeRTOS */
    #include "FSP/portmacro.h"

#elif defined( ARDUINO_AVR_NANO_EVERY ) || \
      defined( ARDUINO_AVR_UNO_WIFI_REV2 )

    #include "AVR_Mega0/portmacro.h"

#else

    #error "The currently selected board is not supported by this port of FreeRTOS"

#endif
/*-----------------------------------------------------------*/

#if( configUSE_IDLE_HOOK == 0 )
    #error configUSE_IDLE_HOOK must be set to 1 for this port.
#endif
/*-----------------------------------------------------------*/

#endif /* __PORTMACRO_H__ */
