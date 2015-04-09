#include "main.h"
#include "setup.h"
#include "eeprom.h"
#include "LCD.h"
#include "interrupts.h"
#include "delays.h"
#include "sensors.h"
#include "configMode.h"
#include "buzzer.h"
#include "alarms.h"

/* ----------- GLOBALS ---------------*/
volatile time clock;
time alarm;
time * clockToChange = &clock;
char temperature_treshold = 10;
char lumus_treshold = 5;
char updateLCD = 1;
char lumus = 0;
char temp = 0; // temperature, NOT temporary!
char alarmMask = 0; //3 lsb's define if the clock, temperature or lumos alarms are enabled (respectively)
char sleeping = 0;
char buzzTimer = 0;
int nStored = 0;
// these variables are changed by ISRs
volatile char configMode = 0;
volatile char configModeUpdated = 0;
volatile char update_hours = 1;
volatile char update_minutes = 1;
volatile char update_seconds = 1;
volatile char update_alt = 0;
volatile char update_temp = 1;
volatile char update_lumus = 1;
volatile char pmon_counter = 0;
volatile char updateTimeAlarm = 0;

void main (void)
{
	delayms(1000);
	setup();

	while(1)
	{
		while(configMode)
		{
			config();
		}

		if(updateTimeAlarm && (alarmMask & 0b00000100))
			fireTimeAlarm();

		if(!sleeping)
			updateScreen();

		if(buzzTimer != 0)
		{
				buzzTimer--;
				if(buzzTimer == 0)
					buzzKill();
		}

		_asm sleep _endasm
	}
}

void int_to_str(int raw, char *str)
{
	str[1] = raw%10 + '0';
	raw /= 10;
	str[0] = raw + '0';
	str[2] = 0;
}
