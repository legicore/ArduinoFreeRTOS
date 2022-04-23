Port of the FreeRTOS Kernel for Arduino.

## Tested Boards

Name               | Notes
------------------ | ---------------------
Arduino Uno        | Rev3 and Rev3 SMD
Arduino Mega 2560  |
Arduino Leonardo   |
Arduino Nano Every |
Arduino Uno Wifi   | Rev2
Arduino Pro Mini   | ATmega328P, 5V, 16MHz

## Source Changes

* TCB_t has been changed to FreeRTOS_TCB_t in the files croutine.h, task.c, and port.c (in folders ATmega and AVR_Mega0) because the Arduino megaavr architecture core implementation uses the same type name in the sources.
