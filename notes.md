
#MPLAB C18 C Compiler User’s Guide
##Interrupts

an ISR must be declared with no parameters and no return value

Global variables that are accessed by both an ISR and mainline functions should be declared volatile.


#PICmicro 18C MCU Family Reference Manual

RB4 has Interrupt on change pin.

RB2:RB0 pins can also function as external interrupt sources INT2:INT0 while working as digital inputs.

the buffer to the interrupt structure from pin RB0 is a Schmitt Trigger, which requires a higher voltage (than TTL input) before the high input is registered.  So it is possible to read a ‘1’, but not get the interrupt. The interrupt was given a Schmitt Trigger input with hysteresis to minimize noise problems.
