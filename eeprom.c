#include "eeprom.h"

void EEPROMTesting(){
  unsigned char data = 'x';
  unsigned char result;

  unsigned char write_High_Addr = 0x00;
  unsigned char write_Low_Addr = 0x05; // random value
  
  CloseI2C(); // just in case it was open
  OpenI2C(MASTER, SLEW_OFF);
  SSPADD = 0b00000111;

  IdleI2C();
  StartI2C();
  while(SSPCON2bits.SEN); //waiting for end of start condition
  do {
    result = WriteI2C(0xA0);
  } while(result != 0); // testing for a correct write
  while(SSPCON2bits.ACKSTAT); // wait for ack
  do {
    result = WriteI2C(write_High_Addr);
  } while( result != 0);
  while(SSPCON2bits.ACKSTAT);
  do {
    result = WriteI2C(write_Low_Addr);
  } while( result != 0);
  while(SSPCON2bits.ACKSTAT);
  do {
    result = WriteI2C(data);             // Escrita done
  } while( result != 0);
  while(SSPCON2bits.ACKSTAT);
  StopI2C();
  IdleI2C();

  // Leitura
  IdleI2C();
  StartI2C();
  while(SSPCON2bits.SEN); //waiting for end of start condition
  do {
    result = WriteI2C(0xA0);
  } while(result != 0);
  while(SSPCON2bits.ACKSTAT); // wait for ack
  do {
    result = WriteI2C(write_High_Addr);
  } while(result != 0);
  while(SSPCON2bits.ACKSTAT);
  do {
    WriteI2C(write_Low_Addr);
  } while( result != 0);
  while(SSPCON2bits.ACKSTAT);
  StartI2C();         // send new start condition
  while(SSPCON2bits.SEN);   // wait for start condition to stop
  do {
    result = WriteI2C(0xA1);             //Dizer que e leitura
  } while(result != 0);
  while(SSPCON2bits.ACKSTAT);
  result = ReadI2C();
  NotAckI2C();
  StopI2C(); 
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
