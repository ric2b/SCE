#include "main.h"
#include "sensors.h"
#include "LCD.h"
#include "delays.h"

char cursorState( char ignore )
{ // use TMR1L TMR1H to get shorter time scales
	if(update_seconds == 1)
	{
		update_seconds = 0;
		if(!ignore) // if the field was recently updated, wait at least 1 second
			return clock.seconds & 1; // if 0, blink cursor (delete the number from the screen)
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
		if(configMode == 9) // Lumus config
			putcXLCD(time_buf[1]);
		else
			putsXLCD(time_buf);
		blink = cursorState(1); // ignore next second update
	}
	else
		blink = cursorState(0);

	if(blink > 0)
	{
		SetDDRamAddr(LCDaddr);
		if(*fielddata != -1)
			int_to_str(*fielddata, time_buf);
		else
		{
			time_buf[0] = '-';
			time_buf[1] = '-';
			time_buf[2] = 0;
		}

		if(configMode == 9)
			putcXLCD(time_buf[1]);
		else
			putsXLCD(time_buf);
	}
	else if(blink == 0)
	{
		SetDDRamAddr(LCDaddr);
		time_buf[0] = ' ';
		time_buf[1] = ' ';
		time_buf[2] = 0;

		if(configMode == 9)
			putcXLCD(time_buf[1]);
		else
			putsXLCD(time_buf);
	}
}

char alarmONOFF(char alarmID, char character, char LCDaddr)
{
	char buffer[3];
	char blink;
	// this section blinks the alarm character
	blink = cursorState(0);
	SetDDRamAddr(LCDaddr);
	if(blink == 0)
		putcXLCD(' ');
	if(blink > 0)
		putcXLCD(character);
	// this section changes the active status of the alarm
	if(changeValueWithS2(buffer)) //ignore the buffer
	{
		alarmMask ^= (0b00000100 >> alarmID);
		SetDDRamAddr(0x0d);
		if(alarmMask & (0b00000100 >> alarmID))
		{
			putcXLCD('A');
			return 1; // if the user pressed the button and the new status is ACTIVE
		}
		else
			putcXLCD('a');
	}
	return 0;
}

void clearConfigScreen( void )
{
	SetDDRamAddr(0x09);
	putcXLCD(' ');
	putcXLCD(' ');
	putcXLCD(' ');
	SetDDRamAddr(0x0d);
	putcXLCD(' ');
	SetDDRamAddr(0x0f);
	putcXLCD(' ');
}

void config()
{
	char buffer[3];
	char changed = 0;
	char blink;
	time temp;

#ifdef debug
	TRISBbits.TRISB2 = 0;
	TRISBbits.TRISB3 = 0;
	PORTBbits.RB2 = 1;
	PORTBbits.RB3 = 0;
#endif
	//reset the configurable variables
	//alarm.seconds = alarm.minutes = alarm.hours = 0;

	// put P from low power mode
	while(BusyXLCD());
	SetDDRamAddr(0x09);
	putrsXLCD("ATL a P");

	while(BusyXLCD());

	switch(configMode)
	{
		case 1: // hours
			changed = 0;
			temp.hours = -1;
			while(configMode == 1)
			{
				updateClockField(0x00, &temp.hours, 24);
				if(temp.hours != -1)
					changed = 1;
			}
			if(changed == 1)
				clockToChange->hours = temp.hours;
			SetDDRamAddr(0x00);
			int_to_str(clockToChange->hours, buffer);
			putsXLCD(buffer);
			break;

		case 2: // minutes
			changed = 0;
			temp.minutes = -1;
			while(configMode == 2)
			{
				updateClockField(0x03, &temp.minutes, 60);
				if(temp.minutes != -1)
					changed = 1;
			}
			if(changed == 1)
				clockToChange->minutes = temp.minutes;
			SetDDRamAddr(0x03);
			int_to_str(clockToChange->minutes, buffer);
			putsXLCD(buffer);
			break;

		case 3: // seconds
			changed = 0;
			temp.seconds = -1;
			while(configMode == 3)
			{
				updateClockField(0x06, &temp.seconds, 60);
				if(temp.seconds != -1)
					changed = 1;
			}
			if(changed == 1)
				clockToChange->seconds = temp.seconds;
			SetDDRamAddr(0x06);
			int_to_str(clockToChange->seconds, buffer);
			putsXLCD(buffer);
			clockToChange = &clock;
			break;

		case 4: // alarm cfg
			SetDDRamAddr(0x0d);
			if(alarmMask & 0b00000100)
				putcXLCD('A');
			else
				putcXLCD('a');

			while(configMode == 4)
			{
				if(alarmONOFF(0, 'A', 0x09)) // user activated the alarm, configure it
				{
					clockToChange = &alarm;
					configMode = 1;
				}
			}
			SetDDRamAddr(0x09);
			putcXLCD('A');
			//changeValueWithS2(&temperature_treshold);
			//temperature_treshold %= 100;
			break;

		case 5: // activate temperature
			SetDDRamAddr(0x0d);
			if(alarmMask & 0b00000010)
				putcXLCD('A');
			else
				putcXLCD('a');

			while(configMode == 5)
			{
				if(alarmONOFF(1, 'T', 0x0A))
				{
					SetDDRamAddr(0x0A);
					putcXLCD('T');
					configMode = 8;
				}
			}
			SetDDRamAddr(0x0A);
			putcXLCD('T');
			//changeValueWithS2(&lumos_treshold);
			//lumos_treshold %= 6;
			break;

		case 6: // activate lumos
			SetDDRamAddr(0x0d);
			if(alarmMask & 0b00000001)
				putcXLCD('A');
				else
				putcXLCD('a');

			while(configMode == 6)
			{
				if(alarmONOFF(2, 'L', 0x0B))
				{
					SetDDRamAddr(0x0B);
					putcXLCD('L');
					configMode = 9;
				}
			}
			SetDDRamAddr(0x0B);
			putcXLCD('L');
			break;

		case 7: // energy savings
			while(configMode == 7)
			{
				SetDDRamAddr(0x0f);
				blink = cursorState(0);
				if(blink == 0)
					putcXLCD(' ');
				if(blink > 0)
					putcXLCD('P');

				if(changeValueWithS2(&blink)) //ignore blink
				{
					configMode = 0;
					configModeUpdated = 0;
					while(BusyXLCD());
					WriteCmdXLCD(DOFF);
					sleeping = 1;
					_asm sleep _endasm
				}
			}
			configMode = 10; // goes to Default, to exit
			break;

		case 8: // temperature threshold
			changed = 0;
			blink = -1;
			while(configMode == 8)
			{
				updateClockField(0x40, &blink, 100);
				if(blink != -1)
					changed = 1;
			}
			if(changed == 1)
				temperature_treshold = blink;
			SetDDRamAddr(0x40);
			readTemperature(buffer);
			updateTemp(buffer);
			configMode = 6;
			break;

		case 9: // lumos threshold
			changed = 0;
			blink = -1;
			while(configMode == 9)
			{
				updateClockField(0x4f, &blink, 6);
				if(blink != -1)
					changed = 1;
			}
			if(changed == 1)
				lumus_treshold = blink;
			SetDDRamAddr(0x4e);
			putcXLCD(' ');
			ConvertADC();
			putcXLCD((ReadADC() >> 6)/204+'0');
			configMode = 7;
			break;

		default:
			configMode = 0;
			configModeUpdated = 0; // reset the variable
			update_seconds = update_minutes = update_hours = 1;
			clearConfigScreen();
			if(alarmMask != 0)
			{
				SetDDRamAddr(0x0d);
				putcXLCD('A');
			}
			break;
	}
	update_seconds = update_minutes = update_hours = 1;
	updateLCD = 1;
}
