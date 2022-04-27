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

* The name of `TCB_t` has been changed to `FreeRTOS_TCB_t` in the files __*croutine.h*__, __*task.c*__, and __*port.c*__ (in folders __ATmega__ and __AVR_Mega0__) because the Arduino *megaavr* architecture core implementation uses the same type name in the sources.
