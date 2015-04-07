#include "interrupts.h"
#include "main.h"

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

	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
}

void S3_isr (void)
{
	INTCONbits.INT0IF = 0;	/* clear flag to avoid another interrupt. The INT0 external interrupt did not occur */
	if(sleeping == 1)
	{
		sleeping = 0;
	}
	else
	{
		configMode++;
		configModeUpdated = 1;
	}
}

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;	/* The IPEN bit in the RCON register enables priority levels for interrupts. If clear, all priorities are set to high. */
	INTCONbits.GIEH = 1;  /* Enables all un-masked high interrupts */
}
