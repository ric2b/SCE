#include <p18f452.h>
#include <usart.h>
#include "main.h"

volatile char intUSART, charUSART;

void USARTinit (void)
{
	// configure USART
	// Asynchronous mode is stopped during SLEEP.
	OpenUSART(USART_TX_INT_OFF &	
			USART_RX_INT_ON    &
			USART_ASYNCH_MODE  &	// NRZ format (1 start bit, 8 or 9 data bits and 1 stop bit)
			USART_EIGHT_BIT    &	// 8 bits of data
			USART_CONT_RX      &
			USART_BRGH_HIGH, 25);	// 9600 baud for operating freq 4 MHz

	usartReadIndex = 0;	// where to write in usartReadBuf[];
	usartReadFlag = 0;	// signals that EOM was read, data is ready to read
	/*
	   while(1)
	   {
	   while (!intUSART);
	   INTCONbits.GIEH = 0;       // disable interrupts
	   c = charUSART;
	   intUSART = 0;
	   INTCONbits.GIEH = 1;       // enable interrupts

	   while (BusyUSART());
	   putcUSART(c);              // echo char USART
	   }
	   */
}

void serialWrite(char c[],int n)
{
	int i;
	while (BusyUSART());
	putcUSART(0xfd);
	for(i=0; i<n; i++)
	{
		while (BusyUSART());
		putcUSART(c[i]);              
	}
	while (BusyUSART());
	putcUSART(0xfe);
}
