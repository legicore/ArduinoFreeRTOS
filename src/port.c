/*
 * FreeRTOS Kernel V10.4.3
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

/* 

Changes from V2.6.0

    + AVR port - Replaced the inb() and outb() functions with direct memory
      access.  This allows the port to be built with the 20050414 build of
      WinAVR.
*/

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "FreeRTOS.h"
#include "task.h"

#include <Arduino.h>

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the AVR port.
 *----------------------------------------------------------*/

/* Start tasks with interrupts enables. */
#define portFLAGS_INT_ENABLED                       ( ( StackType_t ) 0x80 )

#if configMCU_TIMER == 2

    /* Hardware constants for timer 2. */
    #define portCLEAR_COUNTER_ON_MATCH              ( ( uint8_t ) 0x02 )
    #define portPRESCALE_64                         ( ( uint8_t ) 0x04 )
    #define portCLOCK_PRESCALER                     ( ( uint32_t ) 64 )
    #define portCOMPARE_MATCH_A_INTERRUPT_ENABLE    ( ( uint8_t ) 0x02 )

#elif configMCU_TIMER == 3

    /* Hardware constants for timer 3. */
    #define portCLEAR_COUNTER_ON_MATCH              ( ( uint8_t ) 0x08 )
    #define portPRESCALE_64                         ( ( uint8_t ) 0x03 )
    #define portCLOCK_PRESCALER                     ( ( uint32_t ) 64 )
    #define portCOMPARE_MATCH_A_INTERRUPT_ENABLE    ( ( uint8_t ) 0x02 )

#endif
/*-----------------------------------------------------------*/

/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/*-----------------------------------------------------------*/

/* 
 * Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.  
 * 
 * The first thing we do is save the flags then disable interrupts.  This is to 
 * guard our stack against having a context switch interrupt after we have already 
 * pushed the registers onto the stack - causing the 32 registers to be on the 
 * stack twice. 
 * 
 * r1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1. 
 * 
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer. 
 */

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)

    #define portSAVE_CONTEXT()                                  \
        asm volatile (  "push   r0                      \n\t"   \
                        "in     r0, __SREG__            \n\t"   \
                        "cli                            \n\t"   \
                        "push   r0                      \n\t"   \
                        "in     r0, 0x3b                \n\t"   \
                        "push   r0                      \n\t"   \
                        "in     r0, 0x3c                \n\t"   \
                        "push   r0                      \n\t"   \
                        "push   r1                      \n\t"   \
                        "clr    r1                      \n\t"   \
                        "push   r2                      \n\t"   \
                        "push   r3                      \n\t"   \
                        "push   r4                      \n\t"   \
                        "push   r5                      \n\t"   \
                        "push   r6                      \n\t"   \
                        "push   r7                      \n\t"   \
                        "push   r8                      \n\t"   \
                        "push   r9                      \n\t"   \
                        "push   r10                     \n\t"   \
                        "push   r11                     \n\t"   \
                        "push   r12                     \n\t"   \
                        "push   r13                     \n\t"   \
                        "push   r14                     \n\t"   \
                        "push   r15                     \n\t"   \
                        "push   r16                     \n\t"   \
                        "push   r17                     \n\t"   \
                        "push   r18                     \n\t"   \
                        "push   r19                     \n\t"   \
                        "push   r20                     \n\t"   \
                        "push   r21                     \n\t"   \
                        "push   r22                     \n\t"   \
                        "push   r23                     \n\t"   \
                        "push   r24                     \n\t"   \
                        "push   r25                     \n\t"   \
                        "push   r26                     \n\t"   \
                        "push   r27                     \n\t"   \
                        "push   r28                     \n\t"   \
                        "push   r29                     \n\t"   \
                        "push   r30                     \n\t"   \
                        "push   r31                     \n\t"   \
                        "lds    r26, pxCurrentTCB       \n\t"   \
                        "lds    r27, pxCurrentTCB + 1   \n\t"   \
                        "in     r0, 0x3d                \n\t"   \
                        "st     x+, r0                  \n\t"   \
                        "in     r0, 0x3e                \n\t"   \
                        "st     x+, r0                  \n\t"   \
                    );

#else

    #define portSAVE_CONTEXT()                                  \
        asm volatile (  "push   r0                      \n\t"   \
                        "in     r0, __SREG__            \n\t"   \
                        "cli                            \n\t"   \
                        "push   r0                      \n\t"   \
                        "push   r1                      \n\t"   \
                        "clr    r1                      \n\t"   \
                        "push   r2                      \n\t"   \
                        "push   r3                      \n\t"   \
                        "push   r4                      \n\t"   \
                        "push   r5                      \n\t"   \
                        "push   r6                      \n\t"   \
                        "push   r7                      \n\t"   \
                        "push   r8                      \n\t"   \
                        "push   r9                      \n\t"   \
                        "push   r10                     \n\t"   \
                        "push   r11                     \n\t"   \
                        "push   r12                     \n\t"   \
                        "push   r13                     \n\t"   \
                        "push   r14                     \n\t"   \
                        "push   r15                     \n\t"   \
                        "push   r16                     \n\t"   \
                        "push   r17                     \n\t"   \
                        "push   r18                     \n\t"   \
                        "push   r19                     \n\t"   \
                        "push   r20                     \n\t"   \
                        "push   r21                     \n\t"   \
                        "push   r22                     \n\t"   \
                        "push   r23                     \n\t"   \
                        "push   r24                     \n\t"   \
                        "push   r25                     \n\t"   \
                        "push   r26                     \n\t"   \
                        "push   r27                     \n\t"   \
                        "push   r28                     \n\t"   \
                        "push   r29                     \n\t"   \
                        "push   r30                     \n\t"   \
                        "push   r31                     \n\t"   \
                        "lds    r26, pxCurrentTCB       \n\t"   \
                        "lds    r27, pxCurrentTCB + 1   \n\t"   \
                        "in     r0, 0x3d                \n\t"   \
                        "st     x+, r0                  \n\t"   \
                        "in     r0, 0x3e                \n\t"   \
                        "st     x+, r0                  \n\t"   \
                    );

#endif

/* 
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer. 
 */

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)

    #define portRESTORE_CONTEXT()                               \
        asm volatile (  "lds    r26, pxCurrentTCB       \n\t"   \
                        "lds    r27, pxCurrentTCB + 1   \n\t"   \
                        "ld     r28, x+                 \n\t"   \
                        "out    __SP_L__, r28           \n\t"   \
                        "ld     r29, x+                 \n\t"   \
                        "out    __SP_H__, r29           \n\t"   \
                        "pop    r31                     \n\t"   \
                        "pop    r30                     \n\t"   \
                        "pop    r29                     \n\t"   \
                        "pop    r28                     \n\t"   \
                        "pop    r27                     \n\t"   \
                        "pop    r26                     \n\t"   \
                        "pop    r25                     \n\t"   \
                        "pop    r24                     \n\t"   \
                        "pop    r23                     \n\t"   \
                        "pop    r22                     \n\t"   \
                        "pop    r21                     \n\t"   \
                        "pop    r20                     \n\t"   \
                        "pop    r19                     \n\t"   \
                        "pop    r18                     \n\t"   \
                        "pop    r17                     \n\t"   \
                        "pop    r16                     \n\t"   \
                        "pop    r15                     \n\t"   \
                        "pop    r14                     \n\t"   \
                        "pop    r13                     \n\t"   \
                        "pop    r12                     \n\t"   \
                        "pop    r11                     \n\t"   \
                        "pop    r10                     \n\t"   \
                        "pop    r9                      \n\t"   \
                        "pop    r8                      \n\t"   \
                        "pop    r7                      \n\t"   \
                        "pop    r6                      \n\t"   \
                        "pop    r5                      \n\t"   \
                        "pop    r4                      \n\t"   \
                        "pop    r3                      \n\t"   \
                        "pop    r2                      \n\t"   \
                        "pop    r1                      \n\t"   \
                        "pop    r0                      \n\t"   \
                        "out    0x3c, r0                \n\t"   \
                        "pop    r0                      \n\t"   \
                        "out    0x3b, r0                \n\t"   \
                        "pop    r0                      \n\t"   \
                        "out    __SREG__, r0            \n\t"   \
                        "pop    r0                      \n\t"   \
                    );

#else

    #define portRESTORE_CONTEXT()                               \
        asm volatile (  "lds    r26, pxCurrentTCB       \n\t"   \
                        "lds    r27, pxCurrentTCB + 1   \n\t"   \
                        "ld     r28, x+                 \n\t"   \
                        "out    __SP_L__, r28           \n\t"   \
                        "ld     r29, x+                 \n\t"   \
                        "out    __SP_H__, r29           \n\t"   \
                        "pop    r31                     \n\t"   \
                        "pop    r30                     \n\t"   \
                        "pop    r29                     \n\t"   \
                        "pop    r28                     \n\t"   \
                        "pop    r27                     \n\t"   \
                        "pop    r26                     \n\t"   \
                        "pop    r25                     \n\t"   \
                        "pop    r24                     \n\t"   \
                        "pop    r23                     \n\t"   \
                        "pop    r22                     \n\t"   \
                        "pop    r21                     \n\t"   \
                        "pop    r20                     \n\t"   \
                        "pop    r19                     \n\t"   \
                        "pop    r18                     \n\t"   \
                        "pop    r17                     \n\t"   \
                        "pop    r16                     \n\t"   \
                        "pop    r15                     \n\t"   \
                        "pop    r14                     \n\t"   \
                        "pop    r13                     \n\t"   \
                        "pop    r12                     \n\t"   \
                        "pop    r11                     \n\t"   \
                        "pop    r10                     \n\t"   \
                        "pop    r9                      \n\t"   \
                        "pop    r8                      \n\t"   \
                        "pop    r7                      \n\t"   \
                        "pop    r6                      \n\t"   \
                        "pop    r5                      \n\t"   \
                        "pop    r4                      \n\t"   \
                        "pop    r3                      \n\t"   \
                        "pop    r2                      \n\t"   \
                        "pop    r1                      \n\t"   \
                        "pop    r0                      \n\t"   \
                        "out    __SREG__, r0            \n\t"   \
                        "pop    r0                      \n\t"   \
                    );

#endif
/*-----------------------------------------------------------*/

/*
 * Perform hardware setup to enable ticks from timer 1, compare match A.
 */
static void prvSetupTimerInterrupt( void );

/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
uint16_t usAddress;

    /* Place a few bytes of known values on the bottom of the stack. 
    This is just useful for debugging. */

    *pxTopOfStack = 0x11;
    pxTopOfStack--;
    *pxTopOfStack = 0x22;
    pxTopOfStack--;
    *pxTopOfStack = 0x33;
    pxTopOfStack--;

    /* Simulate how the stack would look after a call to vPortYield() generated by 
    the compiler. */

    /*lint -e950 -e611 -e923 Lint doesn't like this much - but nothing I can do about it. */

    /* The start of the task code will be popped off the stack last, so place
    it on first. */
    usAddress = ( uint16_t ) pxCode;
    *pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
    pxTopOfStack--;

    usAddress >>= 8;
    *pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
    pxTopOfStack--;

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)

    *pxTopOfStack = 0;
    pxTopOfStack--;

#endif

    /* Next simulate the stack as if after a call to portSAVE_CONTEXT().  
    portSAVE_CONTEXT places the flags on the stack immediately after r0
    to ensure the interrupts get disabled as soon as possible, and so ensuring
    the stack use is minimal should a context switch interrupt occur. */
    *pxTopOfStack = ( StackType_t ) 0x00;   /* R0 */
    pxTopOfStack--;
    *pxTopOfStack = portFLAGS_INT_ENABLED;
    pxTopOfStack--;

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)

    *pxTopOfStack = ( StackType_t ) 0x00;   /* EIND */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x00;   /* RAMPZ */
    pxTopOfStack--;

#endif

    /* Now the remaining registers.   The compiler expects R1 to be 0. */
    *pxTopOfStack = ( StackType_t ) 0x00;   /* R1 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x02;   /* R2 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x03;   /* R3 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x04;   /* R4 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x05;   /* R5 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x06;   /* R6 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x07;   /* R7 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x08;   /* R8 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x09;   /* R9 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x10;   /* R10 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x11;   /* R11 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x12;   /* R12 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x13;   /* R13 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x14;   /* R14 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x15;   /* R15 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x16;   /* R16 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x17;   /* R17 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x18;   /* R18 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x19;   /* R19 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x20;   /* R20 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x21;   /* R21 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x22;   /* R22 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x23;   /* R23 */
    pxTopOfStack--;

    /* Place the parameter on the stack in the expected location. */
    usAddress = ( uint16_t ) pvParameters;
    *pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
    pxTopOfStack--;

    usAddress >>= 8;
    *pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
    pxTopOfStack--;

    *pxTopOfStack = ( StackType_t ) 0x26;   /* R26 X */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x27;   /* R27 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x28;   /* R28 Y */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x29;   /* R29 */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x30;   /* R30 Z */
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) 0x031;  /* R31 */
    pxTopOfStack--;

    /*lint +e950 +e611 +e923 */

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
    /* Setup the hardware to generate the tick. */
    prvSetupTimerInterrupt();

    /* Restore the context of the first task that is going to run. */
    portRESTORE_CONTEXT();

    /* Simulate a function call end as generated by the compiler.  We will now
    jump to the start of the task the context of which we have just restored. */
    asm volatile ( "ret" );

    /* Should not get here. */
    return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    /* It is unlikely that the AVR port will get stopped.  If required simply
    disable the tick interrupt here. */
}
/*-----------------------------------------------------------*/

/*
 * Manual context switch.  The first thing we do is save the registers so we
 * can use a naked attribute.
 */
void vPortYield( void ) __attribute__ ( ( naked ) );
void vPortYield( void )
{
    portSAVE_CONTEXT();
    vTaskSwitchContext();
    portRESTORE_CONTEXT();

    asm volatile ( "ret" );
}
/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to 
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void vPortYieldFromTick( void ) __attribute__ ( ( naked ) );
void vPortYieldFromTick( void )
{
    portSAVE_CONTEXT();
    if( xTaskIncrementTick() != pdFALSE )
    {
        vTaskSwitchContext();
    }
    portRESTORE_CONTEXT();

    asm volatile ( "ret" );
}
/*-----------------------------------------------------------*/

#if configUSE_WATCHDOG_TICK == 1

    /*
    * Setup the watchdog to generate a tick interrupt.
    */
    static void prvSetupTimerInterrupt( void )
    {
        /* Enable the watchdog interrupt (set WDIE), disable the system reset
        (clear WDE) and set the smallest timeout of 15 ms (clear WDP0-2). */
        _WD_CONTROL_REG = ( 1 << WDIE ) | ( 0 << WDE ) | ( 0 << WDP2 ) | ( 0 << WDP1 ) | ( 0 << WDP0 );
    }

#elif configMCU_TIMER == 2

    /*
    * Setup timer 2 compare match A to generate a tick interrupt.
    */
    static void prvSetupTimerInterrupt( void )
    {
    uint32_t ulCompareMatch;
    uint8_t ucByte;

        /* Using 8bit timer 2 to generate the tick.  Correct fuses must be
        selected for the configCPU_CLOCK_HZ clock. */

        ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

        /* We only have 8 bits so have to scale to get our required tick rate. */
        ulCompareMatch /= portCLOCK_PRESCALER;

        /* Adjust for correct value. */
        ulCompareMatch -= ( uint32_t ) 1;

        /* Setup compare match value for compare match A.  Interrupts are disabled 
        before this is called so we need not worry here. */
        ucByte = ( uint8_t ) ( ulCompareMatch & ( uint32_t ) 0xff );
        OCR2A = ucByte;

        /* Setup compare match behaviour. */
        ucByte = portCLEAR_COUNTER_ON_MATCH;
        TCCR2A |= ucByte;

        /* Setup clock source. */
        ucByte = portPRESCALE_64;
        TCCR2B |= ucByte;

        /* Enable the interrupt - this is okay as interrupt are currently globally
        disabled. */
        ucByte = TIMSK2;
        ucByte |= portCOMPARE_MATCH_A_INTERRUPT_ENABLE;
        TIMSK2 = ucByte;
    }

#elif configMCU_TIMER == 3

    /*
    * Setup timer 3 compare match A to generate a tick interrupt.
    */
    static void prvSetupTimerInterrupt( void )
    {
    uint32_t ulCompareMatch;
    uint8_t ucHighByte, ucLowByte;

        /* Using 16bit timer 3 to generate the tick.  Correct fuses must be
        selected for the configCPU_CLOCK_HZ clock. */

        ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

        /* We only have 16 bits so have to scale to get our required tick rate. */
        ulCompareMatch /= portCLOCK_PRESCALER;

        /* Adjust for correct value. */
        ulCompareMatch -= ( uint32_t ) 1;

        /* Setup compare match value for compare match A.  Interrupts are disabled 
        before this is called so we need not worry here. */
        ucLowByte = ( uint8_t ) ( ulCompareMatch & ( uint32_t ) 0xff );
        ulCompareMatch >>= 8;
        ucHighByte = ( uint8_t ) ( ulCompareMatch & ( uint32_t ) 0xff );
        OCR3AH = ucHighByte;
        OCR3AL = ucLowByte;

        /* Setup clock source and compare match behaviour. */
        ucLowByte = portCLEAR_COUNTER_ON_MATCH | portPRESCALE_64;
        TCCR3B = ucLowByte;

        /* Enable the interrupt - this is okay as interrupt are currently globally
        disabled. */
        ucLowByte = TIMSK3;
        ucLowByte |= portCOMPARE_MATCH_A_INTERRUPT_ENABLE;
        TIMSK3 = ucLowByte;
    }

#endif
/*-----------------------------------------------------------*/

#if configUSE_PREEMPTION == 1

    /*
     * Tick ISR for preemptive scheduler.  We can use a naked attribute as
     * the context is saved at the start of vPortYieldFromTick().  The tick
     * count is incremented after the context is saved.
     */
    #if configUSE_WATCHDOG_TICK == 1

        ISR( WDT_vect, ISR_NAKED ) __attribute__ ( ( signal, naked ) );
        ISR( WDT_vect )
        {
            vPortYieldFromTick();
            asm volatile ( "reti" );
        }

    #elif configMCU_TIMER == 2

        ISR( TIMER2_COMPA_vect, ISR_NAKED ) __attribute__ ( ( signal, naked ) );
        ISR( TIMER2_COMPA_vect )
        {
            vPortYieldFromTick();
            asm volatile ( "reti" );
        }

    #elif configMCU_TIMER == 3

        ISR( TIMER3_COMPA_vect, ISR_NAKED ) __attribute__ ( ( signal, naked ) );
        ISR( TIMER3_COMPA_vect )
        {
            vPortYieldFromTick();
            asm volatile ( "reti" );
        }

    #endif

#else

    /*
     * Tick ISR for the cooperative scheduler.  All this does is increment the
     * tick count.  We don't need to switch context, this can only be done by
     * manual calls to taskYIELD();
     */
    #if configUSE_WATCHDOG_TICK == 1

        ISR( WDT_vect ) __attribute__ ( ( signal ) );
        ISR( WDT_vect )
        {
            xTaskIncrementTick();
        }

    #elif configMCU_TIMER == 2

        ISR( TIMER2_COMPA_vect ) __attribute__ ( ( signal ) );
        ISR( TIMER2_COMPA_vect )
        {
            xTaskIncrementTick();
        }

    #elif configMCU_TIMER == 3

        ISR( TIMER3_COMPA_vect ) __attribute__ ( ( signal ) );
        ISR( TIMER3_COMPA_vect )
        {
            xTaskIncrementTick();
        }

    #endif

#endif
/*-----------------------------------------------------------*/

#if configUSE_IDLE_HOOK == 1

    /*
     * TODO
     */
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

    /*
     * TODO
     */
    void vApplicationMallocFailedHook( void )
    {
        pinMode( LED_BUILTIN, OUTPUT );
        for( ;; )
        {
            digitalWrite( LED_BUILTIN, digitalRead( LED_BUILTIN ) ^ 1 );
            delay( 100 );
        }
    }

#endif
/*-----------------------------------------------------------*/

#if configCHECK_FOR_STACK_OVERFLOW == 1

    /*
     * TODO
     */
    void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
    {
        ( void  ) xTask;
        ( void  ) pcTaskName;

        pinMode( LED_BUILTIN, OUTPUT );
        for( ;; )
        {
            digitalWrite( LED_BUILTIN, digitalRead( LED_BUILTIN ) ^ 1 );
            delay( 1000 );
        }
    }

#endif
/*-----------------------------------------------------------*/

/*
 * See https://www.freertos.org/a00110.html
 */
#if configSUPPORT_STATIC_ALLOCATION == 1

    /* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must
    provide an implementation of vApplicationGetIdleTaskMemory() to provide the
    memory that is used by the Idle task. */
    void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,  StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
    {
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be
    allocated on the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

        /* Pass out a pointer to the StaticTask_t structure in which the Idle
        task's state will be stored. */
        *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

        /* Pass out the array that will be used as the Idle task's stack. */
        *ppxIdleTaskStackBuffer = uxIdleTaskStack;

        /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
        Note that, as the array is necessarily of type StackType_t,
        configMINIMAL_STACK_SIZE is specified in words, not bytes. */
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    }

    #if configUSE_TIMERS == 1

        /* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to
        1, so the application must provide an implementation of
        vApplicationGetTimerTaskMemory() to provide the memory that is used by
        the Timer service task. */
        void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
        {
        /* If the buffers to be provided to the Timer task are declared inside
        this function then they must be declared static - otherwise they will be
        allocated on the stack and so not exists after this function exits. */
        static StaticTask_t xTimerTaskTCB;
        static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

            /* Pass out a pointer to the StaticTask_t structure in which the
            Timer task's state will be stored. */
            *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

            /* Pass out the array that will be used as the Timer task's stack.
            */
            *ppxTimerTaskStackBuffer = uxTimerTaskStack;

            /* Pass out the size of the array pointed to by
            *ppxTimerTaskStackBuffer. Note that, as the array is necessarily of
            type StackType_t, configTIMER_TASK_STACK_DEPTH is specified in
            words, not bytes. */
            *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
        }

    #endif

#endif
