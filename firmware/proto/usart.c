#include <p18f452.h>
#include <usart.h>

#define SOM 0xFD	// start of message
#define EOM 0xFE	// end of message

void chooseInterrupt(void);
void USARTread_isr(void);

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
	if(PIR1bits.RCIF)
		USARTread_isr();	// USART read something
}
#pragma code

void USARTread_isr(void)
{
	// The RCIF flag bit is a read only bit that is cleared by the hardware. It is cleared when the RCREG register has been read and is empty.
	PIE1bits.RCIE = 0;	// disables USART read interrupt

	// dummy code to see USART read interrupt
	TRISBbits.TRISB1 = 0;
	PORTBbits.RB1 = 1;
}

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;	/* The IPEN bit in the RCON register enables priority levels for interrupts. If clear, all priorities are set to high. */
	INTCONbits.GIEH = 1;  /* Enables all un-masked high interrupts */
}

void main()
{
	int i;
	char data[40];	// change to 8 or something
	// Asynchronous mode is stopped during SLEEP.
	OpenUSART(USART_TX_INT_OFF &
			USART_RX_INT_ON    &
			USART_ASYNCH_MODE  &	// NRZ format (1 start bit, 8 or 9 data bits and 1 stop bit)
			USART_EIGHT_BIT    &	// 8 bits of data
			USART_CONT_RX      &
			USART_BRGH_HIGH, 25);	// 9600 baud for operating freq 4 MHz

	EnableHighInterrupts();

	// PIE1bits.RCIE = 0;	// disables USART read interrupt

	while(1)
	{

		i = -1;
		do
		{
			i++;
			while(!PIR1bits.RCIF);	// wait until data is available to be read
			data[i] = RCREG;
		}while(data[i] != EOM);

		TRISBbits.TRISB2 = 0;
		PORTBbits.RB2 = 1;

		i = 0;
		do
		{
			while(!TXSTAbits.TRMT);	// wait until previous data was sent
			TXREG = data[i];
			i++;
		}while(TXREG != EOM);

	}
}
