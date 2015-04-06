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
		case 1: // hours
			while(configMode == 1)
			{
				updateClockField(0x00, &alarm.hours, 24);
			}
			break;
		case 2: // minutes
			while(configMode == 2)
			{
				updateClockField(0x03, &alarm.minutes, 60);
			}
			break;
		case 3: // seconds
			while(configMode == 3)
			{
				updateClockField(0x06, &alarm.seconds, 60);
			}
			break;
		case 4: // alarm cfg
			changeValueWithS2(&temperature_treshold);
			temperature_treshold %= 100;
			break;
		case 5: // activate temperature
			changeValueWithS2(&lumos_treshold);
			lumos_treshold %= 6;
			break;
		case 6: // activate lumos
			configMode = 0;
			configModeUpdated = 0; // reset the variable
			update_seconds = update_minutes = update_hours = 1;
			break;
		case 7: // energy savings
			break;
		case 8: // temperature threshold
			break;
		case 9: // lumos threshold
			break;
	}
	updateLCD = 1;
}

char cursorState( char ignore )
{ // use TMR1L TMR1H to get shorter time scales
	if(update_seconds == 1)
	{
		update_seconds = 0;
		if(!ignore) // if the field was recently updated, wait at least 1 second
			return clock.seconds % 2; // if 0, blink cursor (delete the number from the screen)
	}
	return -1;
}

char changeValueWithS2(char * value)
{
	if(PORTAbits.RA4 == 0)
	{
		while(PORTAbits.RA4 == 0);

		PORTBbits.RB3 = 1; // activa led
		(*value)++;
/*		delayms(500);
		while(PORTAbits.RA4 == 0)
		{
			delayms(100);
			(*value)++;
		}
	*/PORTBbits.RB3 = 0;
		return 1;
	}
	return 0;
}

void updateClockField(char LCDaddr, char * fielddata, char modulos)
{
	char time_buf[3];
	char blink;
	
	if(changeValueWithS2(fielddata))
	{
		*fielddata %= modulos;
		SetDDRamAddr(LCDaddr);
		int_to_str(*fielddata, time_buf);
		putsXLCD(time_buf);
		blink = cursorState(1); // ignore next second update
	}
	else
		blink = cursorState(0);

		if(blink > 0)
		{
			SetDDRamAddr(LCDaddr);
			int_to_str(*fielddata, time_buf);
		putsXLCD(time_buf);
	}
	if(blink == 0)
	{
		SetDDRamAddr(LCDaddr);
		time_buf[0] = ' ';
		time_buf[1] = ' ';
		time_buf[2] = 0;
		putsXLCD(time_buf);
	}
}
