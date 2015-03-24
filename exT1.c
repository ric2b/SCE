/*
*/

#include <p18f452.h>  /* for the special function register declarations */
#include <timers.h>
#include <xlcd.h>
#include <delays.h>
#include <ADC.h>

typedef struct time
{
	char seconds;
	char minutes;
	char hours;
} time;

time clock;
char updateLCD = 1;
char changeConfigMode = 0;

void t1_isr (void);  /* prototype needed for 'goto' below */
void s3_isr (void);  /* prototype needed for 'goto' below */
/*
 * For high interrupts, control is transferred to address 0x8.
 */
#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR (void) 
{
	_asm
		goto t1_isr
	_endasm
}
#pragma code  /* allow the linker to locate the remaining code */
/*
#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR_s3 (void) 
{
	_asm
		goto s3_isr
	_endasm
}
#pragma code  *//* allow the linker to locate the remaining code */

#pragma interrupt t1_isr
void t1_isr (void) 
{
	WriteTimer1( 0x8000 );       // reload timer: 1 second : 0x8000
	
	clock.seconds++;
	if(clock.seconds == 60)
	{
		clock.seconds=0;
		clock.minutes++;
		if(clock.minutes == 60)
		{
			clock.minutes=0;
			clock.hours++;
			if (clock.hours == 24)
			{
				clock.hours = 0;
			}
		}
	} 
	 
	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
	updateLCD = 1;
}
/*
#pragma interrupt s3_isr
void s3_isr (void) 
{
	PIR1bits.INT0IF = 0;         *//* clear flag to avoid another interrupt */
/*	changeConfigMode ++;
}*/

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;    /* enable interrupt priority levels */
	INTCONbits.GIEH = 1;  /* enable all high priority interrupts */
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
	time_hms[5] = ':';
	time_hms[4] = m%10 + '0';
	time_hms[3] = m/10 + '0';
	time_hms[2] = ':';
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
void config()
{
	switch(changeConfigMode)
	{
		case 1:
			
			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:
			changeConfigMode = 0;
			break; 
	}


}

/* main */

void main (void) 
{
	char buf[9];	/* LCD buffer */
	char lumus[3];	/* luminosity buffer (ADC) */
	char temperature[5];
	char s2, s3 = 0;
	long int count;
	int adc_result;

	clock.seconds = 0;
	clock.minutes = 0;
	clock.hours   = 0;

	/* ADC init */
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_8ANA_0REF, ADC_CH0 & ADC_INT_OFF);
	//ADCON1 =0x00;
	//SetChanADC(ADC_CH0);

	/* timer */
	OpenTimer1( TIMER_INT_ON   &
			T1_16BIT_RW    &
			T1_SOURCE_EXT  &
			T1_PS_1_1      &
			T1_OSC1EN_ON   &
			T1_SYNC_EXT_ON );

	EnableHighInterrupts();

	WriteTimer1( 0x8000 ); // load timer: 1 second

	/* LCD */

	ADCON1 = 0x0E;                    // Port A: A0 - analog; A1-A7 - digital
	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters
	buf[8] = 0;  
	buf[5] = ':';
	buf[2] = ':';

	while(1) {
		if(changeConfigMode)
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
			//putsXLCD(buf);             // data memory
			timeToString(buf);
			putsXLCD(buf);             // data memory		
			
			SetDDRamAddr(0x09);
			putrsXLCD("ATL");
	
			SetDDRamAddr(0x0d);
			putrsXLCD("a");
	
			SetDDRamAddr(0x0f);
			putrsXLCD("P");
			
			putcXLCD('#');
		
			SetDDRamAddr(0x40);
			while( BusyXLCD() );
			putsXLCD(readTemperature(temperature));
	
			SetDDRamAddr(0x47);
			putrsXLCD("M");
	
			SetDDRamAddr(0x4c);		
			putrsXLCD("L ");
	
			SetDDRamAddr(0x4e);        // Second line; first column
			ConvertADC();	//perform ADC conversion
			while(BusyADC());	//wait for result
			adc_result = ReadADC() >> 6;	//get ADC result
			ADCtoString(adc_result/100, lumus);
			putsXLCD(lumus);
	
			SetDDRamAddr(0x50);
			putcXLCD('#');
			putcXLCD(' ');
			putcXLCD(s2+'0');		
			putcXLCD(s3+'0');
	
			updateLCD = 0;
		}
		
	}
}
