#include "main.h"
#include "setup.h"
#include "eeprom.h"
#include "LCD.h"
#include "interrupts.h"
#include "delays.h"
#include "sensors.h"
#include "configMode.h"

/* ----------- GLOBALS ---------------*/
volatile time clock;
time alarm;
char temperature_treshold = 99;
char lumos_treshold = 10;
char updateLCD = 1;
// these variables are changed by ISRs
volatile char configMode = 0;
volatile char configModeUpdated = 0;
volatile char update_hours = 1;
volatile char update_minutes = 1;
volatile char update_seconds = 1;
volatile char update_alt = 1;
volatile char update_a = 1;
volatile char update_P = 1;
volatile char update_temp = 1;
volatile char update_M = 1;
volatile char update_lumus = 1;

void main (void)
{
	char teste;
	setup();

	while(1)
	{

		if(configModeUpdated)
		{
			config();
		}

		if(update_seconds)
		{
			if (teste != 'k') // just for testing, oscillate the values on the screen
				teste = 'k';
				else
				teste = 'f';
				EEPROMTesting(teste);
		}
		updateScreen();
	}
}

void int_to_str(int raw, char *str)
{
	str[1] = raw%10 + '0';
	raw /= 10;
	str[0] = raw + '0';
	str[2] = 0;
}
