#include "eeprom.h"

char readFromEEPROM(int address)
{
	char addrHB = address >> 8;
	char addrLB = address & 0x00FF;

	do // wait for the EEPROM to finish writing, at which point it will acknowledge
	{
		Delay1KTCYx(10); // Horribly hacky, can't make it work without it, Proteus slows down to a crawl
		StartI2C(); IdleI2C();
	}	while(WriteI2C(EEPROMW) != 0);

	WriteI2C(addrHB); IdleI2C();
	WriteI2C(addrLB); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(EEPROMR); IdleI2C();
	addrLB = ReadI2C(); IdleI2C(); // Read from slave
	NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
	StopI2C(); IdleI2C();

	return addrLB;
}


void writeToEEPROM(int address, char data)
{
	char addrHB = address >> 8;
	char addrLB = address & 0x00FF;

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(EEPROMW); IdleI2C(); // slave address + write
	WriteI2C(addrHB); IdleI2C(); // Enable Read Write Config
	WriteI2C(addrLB); IdleI2C();
	WriteI2C(data); // check if writes after read still work
	IdleI2C();
	StopI2C(); IdleI2C();
}

void EEPROMTesting(char teste)
{
	unsigned int buffer = 'h';

		/* WRITE */

		if (teste == 'k') writeToEEPROM(0x0004, 'Q');
		else  writeToEEPROM(0x0004, 'q');

		/* READ */

		buffer = readFromEEPROM(0x0004);

	/* Print to Screen */

	while(BusyXLCD());
	WriteCmdXLCD(DOFF);	// Turn display off
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);	// Enable display with no cursor

	while(BusyXLCD());
	SetDDRamAddr(0x49);		// A por na posicao depois do M
	if (buffer == 'h')
		putcXLCD(teste);
	else
		putcXLCD(buffer);
}
