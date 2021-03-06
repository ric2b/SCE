#include <p18f452.h>
#include <usart.h>
#include "main.h"
#include "interrupts.h"
#include "eeprom.h"

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
	DisableHighInterrupts();
	while (BusyUSART());
	putcUSART(SOM);
	for(i=0; i<n; i++)
	{
		while (BusyUSART());
		putcUSART(c[i]);              
	}
	while (BusyUSART());
	putcUSART(EOM);
	EnableHighInterrupts();
}

void serialWriteChar(char c)
{
	DisableHighInterrupts();
	while (BusyUSART());
	putcUSART(SOM);
	while (BusyUSART());
	putcUSART(c);
	while (BusyUSART());
	putcUSART(EOM);
	EnableHighInterrupts();
}

void processMessage(char inBuffer[], char size)
{
	char tmp[6];
	int i, j;
	int varNREG = NREG;

	switch(inBuffer[0])
	{
		case CRLG:
			serialWriteChar(CMD_OK);
			DisableHighInterrupts();
			tmp[0]=clock.hours;
			tmp[1]=clock.minutes;
			tmp[2]=clock.seconds;
			EnableHighInterrupts();
			serialWrite(tmp, 3);
			break;

		case ARLG:
			if(size == 4)
				serialWriteChar(CMD_OK);
			else
			{
				serialWriteChar(CMD_ERRO);
				return;
			}
			DisableHighInterrupts();
			clock.hours = inBuffer[1];
			clock.minutes = inBuffer[2];
			clock.seconds = inBuffer[3];
			EnableHighInterrupts();
			update_seconds = update_minutes = update_hours = 1;
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
			PMON = inBuffer[1];
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
			alarm.hours = inBuffer[1];
			alarm.minutes = inBuffer[2];
			alarm.seconds = inBuffer[3];
			break;

		case DALT:
			serialWriteChar(CMD_OK);
			temperature_threshold = inBuffer[1];
			break;

		case DALL:
			serialWriteChar(CMD_OK);
			lumus_threshold = inBuffer[1];
			break;

		case AALA:
			serialWriteChar(CMD_OK);
    		SetDDRamAddr(0x0d);  			
			if(alarmMask == 0)
  			{
				putcXLCD('A');
				alarmMask = 7; // all alarms on
			}
			else
			{
				putcXLCD('a');				
				alarmMask = 0;
			}
			SetDDRamAddr(0x50);			
			break;

		case IREG:
			serialWriteChar(CMD_OK);
			
			tmp[0] = NREG;
			tmp[1] = (char)numberEvents;
			tmp[2] = (char)writerPointer;
			tmp[3] = (char)readerPointer;

			serialWrite(tmp, 4);

			break;

		case TRGC:
			serialWriteChar(CMD_OK);

			DisableHighInterrupts();
			while (BusyUSART());
			putcUSART(SOM);

			if(inBuffer[1] <= 12){
				for(i = 0; i < inBuffer[1] && i < (char)numberEvents; i++)
				{
					for(j = 0; j<8; j++)
					{
						while (BusyUSART());
						putcUSART(readFromEEPROM(readerPointer+j));
					}
					readerPointer += 8;
					if(readerPointer >= (varNREG << 3))
						readerPointer = 0;
				}
			}else
				putcUSART(CMD_ERRO);

 			while (BusyUSART());
			putcUSART(EOM);
			EnableHighInterrupts();
			break;

		case TRGI:
			serialWriteChar(CMD_OK);

			DisableHighInterrupts();
			
			if(inBuffer[1] + inBuffer[2] > varNREG << 3){
				for(i = inBuffer[2]; i < inBuffer[1] + inBuffer[2] && i < (char)numberEvents; i++)
				{
					if ( i >= varNREG)
						break;
					for(j=0; j<8; j++)
					{
						while(BusyUSART());
						putcUSART(readFromEEPROM(i+j));
					}
				}
			}

			while(BusyUSART());
			putcUSART(EOM);
			EnableHighInterrupts();
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
