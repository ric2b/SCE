# SCE

#MPLAB C18 C Compiler User’s Guide
##Interrupts

an ISR must be declared with no parameters and no return value

Global variables that are accessed by both an ISR and mainline functions should be declared volatile.

There is an 8x8 unsigned hardware multiplier that runs in 1 cycle.

#PICmicro 18C MCU Family Reference Manual

RB4 has Interrupt on change pin.

RB2:RB0 pins can also function as external interrupt sources INT2:INT0 while working as digital inputs.

the buffer to the interrupt structure from pin RB0 is a Schmitt Trigger, which requires a higher voltage (than TTL input) before the high input is registered.  So it is possible to read a ‘1’, but not get the interrupt. The interrupt was given a Schmitt Trigger input with hysteresis to minimize noise problems.

The Timer1 oscillator allows the counter to operate (increment) when the device is in sleep mode. This allows Timer1 to be used as a real-time clock.


#On the LCD - HD44780U

- http://elm-chan.org/docs/lcd/hd44780_e.html
- http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
- https://code.google.com/p/the-bus-pirate/source/browse/trunk/source/HD44780.c
- http://forum.arduino.cc/index.php/topic,46376.0.html
