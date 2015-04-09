#include "sensors.h"
#include "buzzer.h"

void fireTimeAlarm(void)
{
	updateTimeAlarm = 0;

	SetDDRamAddr(0x43);
	putcXLCD('y');

	alarm.seconds--;
	if(alarm.seconds < 0)
	{
		alarm.seconds=59;
		alarm.minutes--;
		if(alarm.minutes < 0)
		{
			alarm.minutes=59;
			alarm.hours--;
			if (alarm.hours < 0)
			{
				alarmMask &= 0b11111011; // disable the alarm
			/*	SetDDRamAddr(0x43);
				putcXLCD('Y');
*/
				buzzSetup(); // SOUND THE ALARM!!!
				buzzOpen(244);
				delayms(2000);
				buzzKill();
			}
		}
	}
}

void readTemperature(char * temperature)
{
	char value;
	do{
		IdleI2C();
		StartI2C(); IdleI2C();
		WriteI2C(TC74ADDW); IdleI2C(); // slave address + write
		WriteI2C(RWCR); IdleI2C(); // Enable Read Write Config
		RestartI2C(); IdleI2C(); // ?????

		WriteI2C(TC74ADDR); IdleI2C(); // slave address + read
		value = ReadI2C(); IdleI2C(); // Read from slave
		NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
		StopI2C(); IdleI2C();
	} while (!(value & 0x40)); // ?????

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(TC74ADDW); IdleI2C(); // slave address + write
	WriteI2C(RTR); IdleI2C(); // Enable Read Write Config
	RestartI2C(); IdleI2C(); // ?????

	WriteI2C(TC74ADDR); IdleI2C(); // slave address + read
	value = ReadI2C(); IdleI2C(); // Read from slave
	NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
	StopI2C(); IdleI2C();

	if(value>=0)
	{
		temperature[0] = value/10 + '0';
		temperature[1] = value%10 + '0';
	}
	else
	{
		temperature[0] = 'N';
		temperature[1] = 'E';
	}
	temp = value;	// updated global variable for temperature

	return;
}
