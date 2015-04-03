#include "interrupts.h"

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

	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
}

void t3_isr (void)
{
	WriteTimer3(0x10000 - (0x8000/8)*PMON);       // reload timer: 1 second : 0x8000
	PIR2bits.TMR3IF = 0;         // clear flag to avoid another interrupt
	update_temp = update_lumus = 1;
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
