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
	IdleI2C();
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

	do // wait for the EEPROM to finish writing, at which point it will acknowledge
	{
		Delay1KTCYx(10); // Horribly hacky, can't make it work without it, Proteus slows down to a crawl
		StartI2C(); IdleI2C();
	}	while(WriteI2C(EEPROMW) != 0);
	IdleI2C(); // slave address + write
	WriteI2C(addrHB); IdleI2C(); // Enable Read Write Config
	WriteI2C(addrLB); IdleI2C();
	WriteI2C(data);
	IdleI2C();
	StopI2C(); IdleI2C();
}

void write8BToEEPROM(int startAddress, char * data)
{
	char i;
	char addrHB = startAddress >> 8;
	char addrLB = startAddress & 0x00FF;

	do // wait for the EEPROM to finish writing, at which point it will acknowledge
	{
		Delay1KTCYx(10); // Horribly hacky, can't make it work without it, Proteus slows down to a crawl
		StartI2C(); IdleI2C();
	}	while(WriteI2C(EEPROMW) != 0);
	IdleI2C(); // slave address + write
	WriteI2C(addrHB); IdleI2C(); // Enable Read Write Config
	WriteI2C(addrLB); IdleI2C();

	for(i=0; i<8; i++)
		WriteI2C(*(char*)(data+i));

	IdleI2C();
	StopI2C(); IdleI2C();
}


void EEPROMTesting(char teste)
{
	unsigned int buffer = 'h';
	char string[9] = "HAPPYBOT";

		if (teste == 'k') writeToEEPROM(0x0004, 'Q');
		else  writeToEEPROM(0x0004, 'q');

		buffer = readFromEEPROM(0x0004);

		write8BToEEPROM(0x8, string);

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
