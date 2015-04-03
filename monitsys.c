/*
*/

#include "main.h"
#include "interrupts.h"
#include "delays.h"
#include "sensors.h"
#include "configMode.h"

void chooseInterrupt (void);  /* prototype needed for 'goto' below */
void S3_isr (void);
void t1_isr (void);
void t3_isr (void);

/*
 * For high interrupts, control is transferred to address 0x8.
 */
#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR (void)
{
	_asm
		goto chooseInterrupt
	_endasm
}

#pragma interrupt chooseInterrupt
void chooseInterrupt(void)
{
	if(PIR1bits.TMR1IF)
		t1_isr();
	if(PIR2bits.TMR3IF)
		t3_isr();
	if(INTCONbits.INT0IF)
		S3_isr();
}

#pragma code  /* allow the linker to locate the remaining code */

void main (void)
{
	char time_buf[3];	/* LCD buffer */
	char temperature[3];
	char S2, S3 = 0;
	long int count;
	int adc_result;

	clock.seconds = 0;
	clock.minutes = 0;
	clock.hours   = 0;
	alarm.seconds = 0;
	alarm.minutes = 0;
	alarm.hours   = 0;

	/* ADC init */
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_8ANA_0REF, ADC_CH0 & ADC_INT_OFF);

	/* S2 (RA4) is output */
	TRISAbits.TRISA4 = 1;

	/* timer */
	OpenTimer1(TIMER_INT_ON   &
			T1_16BIT_RW    &
			T1_SOURCE_EXT  &
			T1_PS_1_1      &
			T1_OSC1EN_ON   &
			T1_SYNC_EXT_ON );

	OpenTimer3(TIMER_INT_ON   &
			T3_16BIT_RW    &
			T3_SOURCE_EXT  &	// to use TMR1 as input this must be set to EXT
			T3_PS_1_8      &
			T3_SYNC_EXT_ON );

	/* enable external interrupt on pin RB0/INT0 */
	TRISBbits.TRISB0 = 1;
	OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_OFF);

	EnableHighInterrupts();

	WriteTimer1(0x8000); // load timer: 1 second
	WriteTimer3(0x10000 - (0x8000/8)*PMON); // load timer: 1 second

	/* I2C */
	TRISC = 0b00011000; // I2C pins
	SSPADD = 0b00000111;    // Set baud rate
	temperature[2] = 0;
	OpenI2C(MASTER, SLEW_OFF);

	/* LCD */
	ADCON1 = 0x0E;                    // Port A: A0 - analog; A1-A7 - digital
	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters

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
