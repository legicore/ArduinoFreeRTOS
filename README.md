Port of the FreeRTOS Kernel for Arduino.

## Tested Boards

Name               | Notes
------------------ | ---------------------
Arduino Uno        | Rev3 and Rev3 SMD
Arduino Mega 2560  |
Arduino Leonardo   |
Arduino Pro Mini   | ATmega328P, 5V, 16MHz
Arduino UNO R4     | MINIMA and WiFi
Arduino Uno Wifi   | Rev2
Arduino Nano Every |

## Source changes

* Some includes had to be adjusted for the *renesas* port (see comments in `FSP` folder starting with "// CHANGE ...").
* The name of `TCB_t` has been changed to `FreeRTOS_TCB_t` (in the files __*croutine.h*__ and __*task.c*__) because the Arduino *megaavr* architecture core implementation uses the same type name in the sources.
* The file `timers.h` has been renamed to `timersFreeRTOS.h` because the Arduino *megaavr* architecture core implementation includes a file with the same name.
* The include order of `FreeRTOS.h` and `porthardware.h` in __*port.c*__ (*megaavr* port in folder `AVR_Mega0`) had to be swapped.
