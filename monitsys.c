#include "main.h"
#include "setup.h"
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

// For high interrupts, control is transferred to address 0x8.
#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR (void)
{
	_asm
		goto chooseInterrupt
	_endasm
}

#pragma code  /* allow the linker to locate the remaining code */

void main (void)
{
	char time_buf[3];	/* LCD buffer */
	char temperature[3];
	char S2, S3 = 0;
	long int count;
	int adc_result;

	temperature[2] = 0;

	setup();

	while(BusyXLCD());
	WriteCmdXLCD(DOFF);	// Turn display off
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);	// Enable display with no cursor

	while(BusyXLCD());
	SetDDRamAddr(0x02);
	putcXLCD(':');

	while(BusyXLCD());
	SetDDRamAddr(0x05);
	putcXLCD(':');

	while(BusyXLCD());
	SetDDRamAddr(0x43);
	putcXLCD('C');

	while(BusyXLCD());
	SetDDRamAddr(0x4d);
	putcXLCD('L');

	while(1)
	{
		if(configModeUpdated)
		{
			config();
		}

		if(update_seconds & !configModeUpdated)
		{
			update_seconds = 0;
			while(BusyXLCD());
			SetDDRamAddr(0x06);
			int_to_str(clock.seconds, time_buf);
			putsXLCD(time_buf);

			if(update_minutes)
			{
				update_minutes = 0;
				while(BusyXLCD());
				SetDDRamAddr(0x03);
				int_to_str(clock.minutes, time_buf);
				putsXLCD(time_buf);

				if(update_hours)
				{
					update_hours = 0;
					while(BusyXLCD());
					SetDDRamAddr(0x00);
					int_to_str(clock.hours, time_buf);
					putsXLCD(time_buf);

				}
			}
		}

		if(update_alt)
		{
			update_alt = 0;
			SetDDRamAddr(0x09);
			putrsXLCD("ATL");
		}

		if(update_a)
		{
			update_a = 0;
			SetDDRamAddr(0x0d);
			putcXLCD('a');
		}

		if(update_P)
		{
			update_P = 0;
			SetDDRamAddr(0x0f);
			putcXLCD('P');
		}

		if(update_temp)
		{
			update_temp = 0;
			while(BusyXLCD());
			SetDDRamAddr(0x40);
			readTemperature(temperature);
			putsXLCD(temperature);
		}

		if(update_M)
		{
			update_M = 0;
			while(BusyXLCD());
			SetDDRamAddr(0x47);
			putcXLCD('M');
		}

#ifdef debug
		if(configMode)
		{
			while(BusyXLCD());
			SetDDRamAddr(0x49);
			putcXLCD('M');
		}
#endif

		if(update_lumus)
		{
			update_lumus = 0;
			SetDDRamAddr(0x4f);	// Second line; first column
			ConvertADC();	//perform ADC conversion
			while(BusyADC());	//wait for result
			adc_result = ReadADC() >> 6;	//get ADC result
			putcXLCD(adc_result/204+'0');	// will be on [0,5]
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
