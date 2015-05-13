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

void serialWrite(char c[], char n)
{
	int i;
	while (BusyUSART());
	putcUSART(SOM);
	for(i=0; i<n; i++)
	{
		while (BusyUSART());
		putcUSART(c[i]);              
	}
	while (BusyUSART());
	putcUSART(EOM);
}

void serialWriteChar(char c)
{
	while (BusyUSART());
	putcUSART(SOM);
	while (BusyUSART());
	putcUSART(c);
	while (BusyUSART());
	putcUSART(EOM);
}

void processMessage(char inBuffer[], char size)
{
	char tmp[6];
	switch(inBuffer[0])
	{
		case CRLG:
			serialWriteChar(CMD_OK);
			tmp[0]=clock.hours;
			tmp[1]=clock.minutes;
			tmp[2]=clock.seconds;
			serialWrite(tmp, 3);
			break;

		case ARLG:
			serialWriteChar(CMD_OK);
			break;

		case CTEL:
			serialWriteChar(CMD_OK);
			tmp[0]=temp;
			tmp[1]=lumus;
			serialWrite(tmp, 2);
			break;

		case CPAR:
			serialWriteChar(CMD_OK);
			tmp[0]=NREG;
			tmp[1]=PMON;
			tmp[2]=TSOM;
			serialWrite(tmp, 3);
			break;

		case MPMN:
			serialWriteChar(CMD_OK);
			break;

		case CALA:
			serialWriteChar(CMD_OK);
			tmp[0]=alarm.hours;
			tmp[1]=alarm.minutes;
			tmp[2]=alarm.seconds;
			tmp[3]=temperature_threshold;
			tmp[4]=lumus_threshold;
			if(alarmMask)
				tmp[5]=1;
			else 
				tmp[5]=0;
			serialWrite(tmp, 6);
			break;

		case DALR:
			serialWriteChar(CMD_OK);
			break;

		case DALT:
			serialWriteChar(CMD_OK);
			break;

		case DALL:
			serialWriteChar(CMD_OK);
			break;

		case AALA:
			serialWriteChar(CMD_OK);
			break;

		case IREG:
			serialWriteChar(CMD_OK);
			break;

		case TRGC:
			serialWriteChar(CMD_OK);
			break;

		case TRGI:
			serialWriteChar(CMD_OK);
			break;

		case NMCH:
			serialWriteChar(CMD_OK);
			if(numberEvents > NREG>>1)
				serialWriteChar(1);
			else 
				serialWriteChar(0);
			break;

		default:
			serialWriteChar(CMD_ERRO);
			break;
	}
}
