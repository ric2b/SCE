#include "eeprom.h"

void EEPROMTesting(){
	unsigned char buffer = 1;
	unsigned char teste;

	EEByteWrite(0xA0,0x30,0x45);	// Abrir a ligacao I2C
	EEAckPolling(0xA0);				// Tratar dos Ack's
	EECurrentAddRead(0x40);			// ???????????????
	EEByteWrite(0xA0, 0x70, buffer);// Escrever na EEPROM
	EEAckPolling(0xA0);				// Tratar de Ack's, vamos passar a ler
	teste = EERandomRead(0xA0,0x70);// Ler da EEPROM, o ponteiro de leitura é automaticamente incrementado
	EERandomRead(0xA0, 0x30);		// ???????????????

	while(BusyXLCD());
	WriteCmdXLCD(DOFF);	// Turn display off
	while(BusyXLCD());
	WriteCmdXLCD(CURSOR_OFF);	// Enable display with no cursor

	while(BusyXLCD());
	SetDDRamAddr(0x44);		// A por na posicao depois da temperatura
	putcXLCD(teste);
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