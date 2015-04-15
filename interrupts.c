#include <p18f452.h>  /* for the special function register declarations */
#include "interrupts.h"
#include "main.h"
#include "EEPROMint.h"

// For high interrupts, control is transferred to address 0x8.
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
	if(PIR2bits.EEIF)
		expelliarmus_isr();	// internal eeprom write complete
	if(PIR2bits.LVDIF)
		phoenix_isr();	// LVD detect, so that the program can rise from the ashes
	if(INTCONbits.INT0IF)
		S3_isr();
}

#pragma code
void t1_isr (void)
{
	WriteTimer1(0x8000);       // reload timer: 1 second : 0x8000

	clock.seconds++;
	update_seconds = 1;
	if(clock.seconds >= 60)
	{
		clock.seconds=0;
		clock.minutes++;
		update_minutes = 1;
		if(clock.minutes >= 60)
		{
			clock.minutes=0;
			clock.hours++;
			update_hours = 1;
			if (clock.hours >= 24)
			{
				clock.hours = 0;
			}
		}
	}

	pmon_counter++;
	if(pmon_counter == PMON)
	{
		pmon_counter = 0;
		update_temp = update_lumus = 1;
	}

	updateTimeAlarm = 1;
	if(buzzTimer > 0)
		buzzTimer--;	

	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
}

void expelliarmus_isr(void)
{
	EECON1bits.WREN = 0;	// Disable writes on write complete (EEIF set)
	PIR2bits.EEIF = 0;		/* clear flag to avoid another interrupt */
}

void phoenix_isr(void)
{
	// /!\ change this
	EEPROMintWrite(0x00, 'A');
	while(EECON1bits.WR);	// wait for previous write to end
	EEPROMintWrite(0x07, 'B');
	while(EECON1bits.WR);
	EEPROMintWrite(0x07, 'C');
	PIR2bits.LVDIF = 0;         /* clear flag to avoid another interrupt */
}

void S3_isr (void)
{
	if(sleeping == 1)
	{
		sleeping = 0;
	}
	else
	{
		configMode++;
		configModeUpdated = 1;
	}

	INTCONbits.INT0IF = 0;	/* clear flag to avoid another interrupt. The INT0 external interrupt did not occur */
}

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;	/* The IPEN bit in the RCON register enables priority levels for interrupts. If clear, all priorities are set to high. */
	INTCONbits.GIEH = 1;  /* Enables all un-masked high interrupts */
}
