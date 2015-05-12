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
	if(PIR2bits.LVDIF)
		LVD_isr();	// LVD detect, so that the program can rise from the ashes
	if(PIR1bits.TMR1IF)
		t1_isr();
	if(PIR1bits.RCIF)
		USARTread_isr();	// USART read something
	if(PIR2bits.EEIF)
		EEwriteDisable_isr();	// wait for internal eeprom write complete
	if(INTCONbits.INT0IF)
		S3_isr();
}
#pragma code

void USARTread_isr(void)
{
	char tmp;

	tmp = RCREG;

	if(tmp == SOM)
	{
		usartReadIndex = 0;
	}
	else if(tmp == EOM)
	{
		usartReadFlag = 1;
		// The RCIF flag bit is a read only bit that is cleared by the hardware. It is cleared when the RCREG register has been read and is empty.
		PIE1bits.RCIE = 0;	// disables USART read interrupt
	}
	else
	{
		if(usartReadIndex < USART_BUF_LEN){	
			usartReadBuf[usartReadIndex] = tmp;
			usartReadIndex++;
		}
	}

	// dummy code to see USART read interrupt
	TRISBbits.TRISB1 = 0;
	PORTBbits.RB1 = 1;
}

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
	if(pmon_counter >= PMON)
	{
		pmon_counter = 0;
		update_temp = update_lumus = 1;
	}

	updateTimeAlarm = 1;
	if(buzzTimer > 0)
		buzzTimer--;	

	PIR1bits.TMR1IF = 0;         /* clear flag to avoid another interrupt */
}

void EEwriteDisable_isr(void)
{
	EECON1bits.WREN = 0;	// Disable writes on write complete (EEIF set)
	PIR2bits.EEIF = 0;		/* clear flag to avoid another interrupt */
}

void LVD_isr(void)
{
	while(EECON1bits.WR);
	EEPROMintWrite(TIME_BAK_ADDR+2, clock.seconds);
	PIR2bits.LVDIF = 0;         /* clear flag to avoid another interrupt */
	while(1);
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

void DisableHighInterrupts (void)
{
	INTCONbits.GIEH = 0;  /* Disables all un-masked high interrupts */
}
