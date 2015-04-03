#include "setup.h"

void setup(void)
{
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
}
