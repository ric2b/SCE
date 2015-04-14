#include "main.h"
#include "buzzer.h"

void fireTimeAlarm(void)
{
	updateTimeAlarm = 0;

	if(alarm.seconds == clock.seconds)
	{
		if(alarm.minutes == clock.minutes)
		{
			if (alarm.hours == clock.hours)
			{
				alarmMask &= 0b11111011; // disable the alarm

				sleeping = 0;
				while(BusyXLCD());
				WriteCmdXLCD(CURSOR_OFF);
				SetDDRamAddr(0x09);
				putcXLCD('A');

				if(buzzTimer == 0)
				{
					buzzTimer = TSOM+1;
					buzzSetup(); // SOUND THE ALARM!!!
					buzzOpen(244);
					addToEEPROM(7); // WRITE THE ALARM!!! 7 stands for time alarm going off
				}

			}
		}
	}
}

void fireTempAlarm(void)
{
	alarmMask &= 0b11111101;

	sleeping = 0;
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);
	SetDDRamAddr(0x0A);
	putcXLCD('T');

	buzzTimer = TSOM+1;
	buzzSetup(); // SOUND THE ALARM!!!
	buzzOpen(732);

	addToEEPROM(8); // WRITE THE ALARM!!! 8 stands for temperature alarm going off
}

void fireLumusAlarm(void)
{
	alarmMask &= 0b11111110;

	sleeping = 0;
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);
	SetDDRamAddr(0x0B);
	putcXLCD('L');

	buzzTimer = TSOM+1;
	buzzSetup(); // SOUND THE ALARM!!!
	buzzOpen(488);

	addToEEPROM(9); // WRITE THE ALARM!!! 9 stands for lumus alarm going off
}
