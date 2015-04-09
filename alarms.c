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
				SetDDRamAddr(0x09);
				putcXLCD('A');

				if(buzzTimer == 0)
				{
					buzzTimer = TSOM+1;
					buzzSetup(); // SOUND THE ALARM!!!
					buzzOpen(244);
				}
			}
		}
	}
}

void fireTempAlarm(void)
{
	alarmMask &= 0b11111101;

	SetDDRamAddr(0x0A);
	putcXLCD('T');

	buzzTimer = TSOM+1;
	buzzSetup(); // SOUND THE ALARM!!!
	buzzOpen(732);
}

void fireLumusAlarm(void)
{
	alarmMask &= 0b11111110;

	SetDDRamAddr(0x0B);
	putcXLCD('L');

	buzzTimer = TSOM+1;
	buzzSetup(); // SOUND THE ALARM!!!
	buzzOpen(488);
}
