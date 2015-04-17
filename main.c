#include "main.h"
#include "setup.h"
#include "eeprom.h"
#include "events.h"
#include "LCD.h"
#include "interrupts.h"
#include "delays.h"
#include "sensors.h"
#include "configMode.h"
#include "buzzer.h"
#include "alarms.h"
#include "EEPROMint.h"

/* ----------- GLOBALS ---------------*/
volatile time clock;
time alarm;
time * clockToChange = &clock;
char temperature_threshold = 10;
char lumus_threshold = 5;
char updateLCD = 1;
char lumus = 0;
char lumus_last = 0;
char temp_last = 0;
char temp = 0; // temperature, NOT temporary!
char alarmMask = 0; //3 lsb's define if the clock, temperature or lumos alarms are enabled (respectively)

short int writerPointer=0;
short int readerPointer=0;
short int numberEvents=0;

// these variables are changed by ISRs
volatile char configMode = 0;
volatile char configModeUpdated = 0;
volatile char update_hours = 1;
volatile char update_minutes = 1;
volatile char update_seconds = 1;
volatile char update_temp = 1;
volatile char update_lumus = 1;
volatile char pmon_counter = 0;
volatile char updateTimeAlarm = 0;
volatile char buzzTimer = 0;
volatile char sleeping = 0;

void main (void)
{
	//#ifdef proteus
	//delayms(50);
	//#endif
	//RCON &= 0b11100000;
	setup();
	updateScreen();

//	if(PMON != 0)
//		addToEEPROM(1); // Execution begins! 1 stands for that

	while(1)
	{
		while(configMode)
		{
			config();
		}

		if(update_lumus || update_temp || updateTimeAlarm)
			EEPROMintUpdateW();

		if(updateTimeAlarm && (alarmMask & 0b00000100))
			fireTimeAlarm();

		if(!sleeping || (alarmMask != 0 && (update_temp || update_lumus)))
			updateScreen();

		if(buzzTimer == 0)
		{
			buzzKill();
			Sleep();	 // cannot sleep while buzzer should be on
		}
	}
}

void int_to_str(int raw, char *str)
{
	str[1] = raw%10 + '0';
	raw /= 10;
	str[0] = raw + '0';
	str[2] = 0;
}
