/*
*/

#include <p18f452.h>  /* for the special function register declarations */
#include <timers.h>
#include <xlcd.h>
#include <delays.h>
#include <ADC.h>
#include <portb.h>

#define debug

#ifndef debug
#define F_CPU 4000	// in kHz
#else
#define F_CPU 1000	// in kHz. used when in debug mode
#endif

#define PMON 5
#define TSOM 2
#define NREG 30

typedef struct time
{
	char seconds;
	char minutes;
	char hours;
} time;

volatile time clock;
char temperature_treshold = 99;
char lumos_treshold = 10;
char updateLCD = 1;
volatile char configMode = 0;
volatile char configModeUpdated = 0;

void chooseInterrupt (void);  /* prototype needed for 'goto' below */
void S3_isr (void);
void t1_isr (void);
void config();

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
	if(INTCONbits.INT0IF)
		S3_isr();
}

#pragma code  /* allow the linker to locate the remaining code */

void t1_isr (void) 
{
	WriteTimer1( 0x8000 );       // reload timer: 1 second : 0x8000
	
	clock.seconds++;
	if(clock.seconds >= 60)
	{
		clock.seconds=0;
		clock.minutes++;
		if(clock.minutes >= 60)
		{
			clock.minutes=0;
			clock.hours++;
			if (clock.hours >= 24)
			{
				clock.hours = 0;
			}
		}
	} 
	 
	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
	updateLCD = 1;
}

void S3_isr (void) 
{
	INTCONbits.INT0IF = 0;	/* clear flag to avoid another interrupt. The INT0 external interrupt did not occur */
	configMode++;
	configModeUpdated = 1;
}

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;	/* The IPEN bit in the RCON register enables priority levels for interrupts. If clear, all priorities are set to high. */
	INTCONbits.GIEH = 1;  /* Enables all un-masked high interrupts */
}

void delayms(short millis)
{
	short i, j;
	for(i = 0; i < millis; i++)
	{
		for(j = 0; j < F_CPU/100; j++)
		{	// 98 nops, not 100
			Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
		}
	}
}

/* LCD */

void DelayFor18TCY( void )
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void DelayPORXLCD( void )
{
	Delay1KTCYx(60);
	return;
}

void DelayXLCD( void )
{
	Delay1KTCYx(20);
	return;
}

void timeToString(char * time_hms)
{
	unsigned short h, m, s;

	h = clock.hours;
	m = clock.minutes;
	s = clock.seconds;

	time_hms[7] = s%10 + '0';
	time_hms[6] = s/10 + '0';
	//time_hms[5] = ':';
	time_hms[4] = m%10 + '0';
	time_hms[3] = m/10 + '0';
	//time_hms[2] = ':';
	time_hms[1] = h%10 + '0';
	time_hms[0] = h/10 + '0';
	//time_hms[8] = 0;
}

void ADCtoString(int raw, char *lumus)
{
	char i;
	lumus[1] = raw%10 + '0';
	raw /= 10;
	lumus[0] = raw%10 + '0';	// note: tirar este %
	lumus[2] = 0;
}

char * readTemperature(char * temperature)
{
	temperature[0] = '2';
	temperature[1] = '1';
	temperature[2] = ' ';
	temperature[3] = 'C';
	temperature[4] = 0; 
	return temperature;
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

void config()
{
	TRISBbits.TRISB2 = 0;
	TRISBbits.TRISB3 = 0;
	PORTBbits.RB2 = 1;
	PORTBbits.RB3 = 0;
	switch(configMode)
	{		
		case 1:
			changeValueWithS2(&clock.hours);
			clock.hours %= 24;
			break;
		case 2:
			changeValueWithS2(&clock.minutes);
			clock.minutes %= 60;
			break;
		case 3:
			changeValueWithS2(&clock.seconds);
			clock.seconds %= 60;
			break;
		case 4:
			changeValueWithS2(&temperature_treshold);
			temperature_treshold %= 100;
			break;
		case 5:
			changeValueWithS2(&lumos_treshold);
			lumos_treshold %= 11;
			break;
		case 6:
			configMode = 0;
			configModeUpdated = 0; // reset the variable
			break; 
	}
	updateLCD = 1;
}

/* main */

void main (void) 
{
	char time_buf[9];	/* LCD buffer */
	char lumus[3];	/* luminosity buffer (ADC) */
	char temperature[5];
	char S2, S3 = 0;
	long int count;
	int adc_result;

	clock.seconds = 0;
	clock.minutes = 0;
	clock.hours   = 0;

	/* ADC init */
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_8ANA_0REF, ADC_CH0 & ADC_INT_OFF);

	/* S2 (RA4) is output */
	TRISAbits.TRISA4 = 1;

	/* timer */
	OpenTimer1( TIMER_INT_ON   &
			T1_16BIT_RW    &
			T1_SOURCE_EXT  &
			T1_PS_1_1      &
			T1_OSC1EN_ON   &
			T1_SYNC_EXT_ON );

	/* enable external interrupt on pin RB0/INT0 */
	TRISBbits.TRISB0 = 1;
	OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_OFF);

	EnableHighInterrupts();

	WriteTimer1(0x8000); // load timer: 1 second

	/* LCD */

	ADCON1 = 0x0E;                    // Port A: A0 - analog; A1-A7 - digital
	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters
	time_buf[8] = 0;  
	time_buf[5] = ':';
	time_buf[2] = ':';

	while(1) {
		if(configModeUpdated)
		{
			config();
		}
		if(updateLCD)
		{
			while( BusyXLCD() );
			WriteCmdXLCD( DOFF );      // Turn display off
			while( BusyXLCD() );
			WriteCmdXLCD( CURSOR_OFF );// Enable display with no cursor
			while( BusyXLCD() );
			SetDDRamAddr(0x00);        // First line, first column
			//putsXLCD(time_buf);             // data memory
			timeToString(time_buf);
			putsXLCD(time_buf);             // data memory		

			SetDDRamAddr(0x09);
			putrsXLCD("ATL");

			SetDDRamAddr(0x0d);
			putcXLCD('a');

			SetDDRamAddr(0x0f);
			putcXLCD('P');

			putcXLCD('#');

			SetDDRamAddr(0x40);
			while( BusyXLCD() );
			putsXLCD(readTemperature(temperature));

			SetDDRamAddr(0x47);
			putcXLCD('M');

#ifdef debug
			SetDDRamAddr(0x49);
			if(configMode)
				putcXLCD('M');
#endif

			SetDDRamAddr(0x4c);		
			putcXLCD('L');

			SetDDRamAddr(0x4e);        // Second line; first column
			ConvertADC();	//perform ADC conversion
			while(BusyADC());	//wait for result
			adc_result = ReadADC() >> 6;	//get ADC result
			ADCtoString(adc_result/100, lumus);
			putsXLCD(lumus);

			SetDDRamAddr(0x50);
			putcXLCD('#');

			updateLCD = 0;
		}

	}
}
