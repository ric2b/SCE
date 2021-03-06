//#include <reset.h>
#include "main.h"
#include "interrupts.h"
#include "sensors.h"
#include "EEPROMint.h"
#include "communication.h"

void setup(void)
{
	char notUsed[2];
	int adc_result;
	clock.seconds = 0;
	clock.minutes = 0;
	clock.hours   = 0;
	alarm.seconds = 0;
	alarm.minutes = 0;
	alarm.hours   = 0;

	/* disable watchdog in software (can still be set in HW) */
	WDTCONbits.SWDTEN = 0;

	/* ADC init */
	OpenADC(ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_8ANA_0REF, ADC_CH0 & ADC_INT_OFF);

	/* S2 (RA4) is output */
	TRISAbits.TRISA4 = 1;

	/* timer */
	#ifdef proteus
	OpenTimer1(TIMER_INT_ON   &
			T1_16BIT_RW    &
			T1_SOURCE_EXT  &
			T1_PS_1_1      &
			T1_OSC1EN_ON   &
			T1_SYNC_EXT_ON );
	#else
	OpenTimer1(TIMER_INT_ON   &
			T1_16BIT_RW    &
			T1_SOURCE_EXT  &
			T1_PS_1_1      &
			T1_OSC1EN_ON   &
			T1_SYNC_EXT_OFF );
	#endif

	/* enable external interrupt on pin RB0/INT0 */
	TRISBbits.TRISB0 = 1;
	OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_OFF);

	/* enable low voltage detection (LVD) interrupt */
	PIE2bits.LVDIE = 0;         /* disable LVD interrupt */
	LVDCON = 0b1100;            /* 4.5 V - 4.77 V */
	LVDCONbits.LVDEN = 1;       /* enable LVD */
	#ifndef proteus
	while(!LVDCONbits.IRVST);   /* wait initialization .Internal Reference Voltage Stable Flag bit. 1 = Indicates that the Low Voltage Detect logic will generate the interrupt flag at the specified voltage range */
	#endif
	PIR2bits.LVDIF = 0;         /* clear interrupt flag */
	PIE2bits.LVDIE = 1;         /* enable LVD interrupt */

	/* LCD */
	ADCON1 = 0x0E;                    // Port A: A0 - analog; A1-A7 - digital
	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters
	while(BusyXLCD());
	WriteCmdXLCD(DOFF); // Turn display off. will be turned on later in update_screen()

	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);

	while(BusyXLCD());
	SetDDRamAddr(0x02);
	putcXLCD(':');
	SetDDRamAddr(0x05);
	putcXLCD(':');
	SetDDRamAddr(0x43);
	putcXLCD('C');
	SetDDRamAddr(0x4d);
	putcXLCD('L');

	// check LVD flag
	if((unsigned char) EEPROMintRead(TIME_BAK_ADDR+2) < 60)
	{
		clock.hours = EEPROMintRead(TIME_BAK_ADDR);
		clock.minutes = EEPROMintRead(TIME_BAK_ADDR+1);
		clock.seconds = EEPROMintRead(TIME_BAK_ADDR+2);
		EEPROMintUpdateR();
	}

	while(EECON1bits.WR);
	EEPROMintWrite(TIME_BAK_ADDR+2, 0xff);	// write some nonsense seconds. will be checked on startup as if it was a magic cookie

	/* I2C */
	TRISC = 0b00011000; // I2C pins
	//SSPADD = 0b00000111;    // Set baud rate
	SSPADD = 9;
	OpenI2C(MASTER, SLEW_ON);  

	/* Initialize lumus and temp */
	readTemperature(notUsed);

	adc_result = ReadADC() >> 6;  //get ADC result
	lumus = adc_result/204;

	/* start USART */
	USARTinit();

	/* interrupts enabled */
	EnableHighInterrupts();

	WriteTimer1(0x8000); // load timer: 1 second
}
