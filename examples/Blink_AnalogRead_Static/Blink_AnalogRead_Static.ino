#include <FreeRTOS.h>
#include <task.h>

/*-----------------------------------------------------------*/

void vTaskBlink( void * pvParameters );
void vTaskAnalogRead( void * pvParameters );

#define blinkTASK_STACK_SIZE configMINIMAL_STACK_SIZE
static StaticTask_t xTaskBlink;
static StackType_t pxBlinkTaskStackBuffer[ blinkTASK_STACK_SIZE ];

#define analogTASK_STACK_SIZE configMINIMAL_STACK_SIZE
static StaticTask_t xTaskAnalogRead;
static StackType_t pxAnalogTaskStackBuffer[ analogTASK_STACK_SIZE ];

/*-----------------------------------------------------------*/

void setup( void )
{
    /* Initialize the serial port. */
    Serial.begin( 9600 );
    while( Serial != true );

    /* Create our tasks. */
    xTaskCreateStatic(
        vTaskBlink,             /* Pointer to the Task implementation. */
        "Blink",                /* Internal name of the task. */
        blinkTASK_STACK_SIZE,   /* Wanted task stack size. */
        NULL,                   /* Task parameter (optional). */
        1,                      /* Task priority. */
        pxBlinkTaskStackBuffer, /* Pointer to the task buffer. */
        &xTaskBlink             /* Pointer to the task TCB. */
    );

    xTaskCreateStatic( vTaskAnalogRead, "Analog", analogTASK_STACK_SIZE, NULL, 1, pxAnalogTaskStackBuffer, &xTaskAnalogRead );

    /* Start the kernel sheduler. */
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

void vTaskBlink( void * pvParameters )
{
    /* Initialize the built in LED. */
    pinMode( LED_BUILTIN, OUTPUT );

    /* Keep the compiler happy because pvParameters is not used here. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* Toggle the built in LED. */
        digitalWrite( LED_BUILTIN, digitalRead( LED_BUILTIN ) ^ 1 );

        /*  */
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
/*-----------------------------------------------------------*/

void vTaskAnalogRead( void * pvParameters )
{
    int16_t sAnalogValue;

    /* Keep the compiler happy because pvParameters is not used here. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* Read the analog value from A0 print it out. */
        sAnalogValue = analogRead( A0 );
        Serial.println( sAnalogValue );

        /* Short delay (1 tick) for stability. */
        vTaskDelay( 1 );
    }
}
