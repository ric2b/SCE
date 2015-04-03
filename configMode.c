#include "configMode.h"

void config()
{
	char time_buf[3];

#ifdef debug
	TRISBbits.TRISB2 = 0;
	TRISBbits.TRISB3 = 0;
	PORTBbits.RB2 = 1;
	PORTBbits.RB3 = 0;
#endif
	//reset the configurable variables
	alarm.seconds = alarm.minutes = alarm.hours = 0;
	temperature_treshold = 0;
	lumos_treshold = 0;
	while(BusyXLCD());

	switch(configMode)
	{
		case 1:
			changeValueWithS2(&alarm.hours);
			alarm.hours %= 24;
			SetDDRamAddr(0x00);    // First line, first column
			int_to_str(alarm.hours, time_buf);
			putsXLCD(time_buf);
			break;
		case 2:
			changeValueWithS2(&alarm.minutes);
			alarm.minutes %= 60;
			SetDDRamAddr(0x03);
			int_to_str(alarm.minutes, time_buf);
			putsXLCD(time_buf);
			break;
		case 3:
			changeValueWithS2(&alarm.seconds);
			alarm.seconds %= 60;
			SetDDRamAddr(0x06);
			int_to_str(alarm.seconds, time_buf);
			putsXLCD(time_buf);
			break;
		case 4:
			changeValueWithS2(&temperature_treshold);
			temperature_treshold %= 100;
			break;
		case 5:
			changeValueWithS2(&lumos_treshold);
			lumos_treshold %= 6;
			break;
		case 6:
			configMode = 0;
			configModeUpdated = 0; // reset the variable
			update_seconds = update_minutes = update_hours = 1;
			break;
	}
	updateLCD = 1;
}

void changeValueWithS2(char * value)
{
	if(PORTAbits.RA4 == 0)
	{
PORTBbits.RB3 = 1;
		(*value)++;
		delayms(500);
		while(PORTAbits.RA4 == 0)
		{
			delayms(100);
			(*value)++;
		}
	}
PORTBbits.RB3 = 0;
	return;
}
