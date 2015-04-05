#include "eeprom.h"

#define EEPROMW (0b10100000) // Address and WRITE command
#define EEPROMR (0b10100001)  // Address and READ command
#define ADDRHB (0x00)    // TC74 command - read temperature
#define ADDRLB (0x01)    // TC74 command - read/write (config)

void EEPROMTesting(char teste){
	unsigned int buffer = 'h';

		/* WRITE */

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(EEPROMW); IdleI2C(); // slave address + write
	WriteI2C(0x00); IdleI2C(); // Enable Read Write Config
	WriteI2C(ADDRLB); IdleI2C();

	if(teste == 'k')	WriteI2C('T'); // check if writes after read still work
	else	WriteI2C('G');

	IdleI2C();
	StopI2C(); IdleI2C();

		/* READ */

	do // wait for the EEPROM to finish writing, at which point it will acknowledge
	{
		Delay1KTCYx(10);
		StartI2C(); IdleI2C();
	}	while(WriteI2C(EEPROMW) != 0);

	WriteI2C(0x00); IdleI2C();
	WriteI2C(ADDRLB); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(EEPROMR); IdleI2C();
	buffer = ReadI2C(); IdleI2C(); // Read from slave
	NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
	StopI2C(); IdleI2C();

	/* Print to Screen */

	while(BusyXLCD());
	WriteCmdXLCD(DOFF);	// Turn display off
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);	// Enable display with no cursor

	while(BusyXLCD());
	SetDDRamAddr(0x00);		// A por na posicao depois da temperatura
	if (buffer == 'h')
		putcXLCD(teste);
	else
		putcXLCD(buffer);
}

void readFromEEPROM(char *buffer){
	EEByteWrite(0xA0,0x30,0x45);
	EEAckPolling(0xA0);
	*buffer = EERandomRead(0xA0,0x70);
	EERandomRead(0xA0, 0x30);
}

void writeToEEPROM(char buffer){
	EEByteWrite(0xA0,0x30,0x45);
	EEAckPolling(0xA0);
	EECurrentAddRead(0x40);
	EEByteWrite(0xA0, 0x70, buffer);
}
