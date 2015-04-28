#include "events.h"
#include "eeprom.h"
#include "main.h"

void addToEEPROM(int code){
	char data[8];
	int varNREG = NREG;
	switch(code)
	{
		case 1:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;
			data[6]=0;
			data[7]=0;
		case 2:		// this case looks dumb.. need a double check
			data[0]=clock.hours;			// afterwars we can change to data[0]=data[4]=clock.hours should be more efficient
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=clock.hours;
			data[5]=clock.minutes;
			data[6]=clock.seconds;
			data[7]=0;
			break;
		case 3:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=alarm.hours;
			data[5]=alarm.minutes;
			data[6]=alarm.seconds;
			data[7]=0;
			break;
		case 4:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temperature_threshold;
			data[5]=0;
			data[6]=0;
			data[7]=0;
			break;
		case 5:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=lumus_threshold;
			data[5]=0;
			data[6]=0;
			data[7]=0;
			break;
		default:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;
			data[6]=0;
			data[7]=0;
			/* FOR TESTING ONLY
			data[0]='h';
			data[1]='e';
			data[2]='l';
			data[3]='l';
			data[4]='o';
			data[5]='w';
			data[6]='o';
			data[7]='r';
			*/
			break;
	}

	write8BToEEPROM(writerPointer, data);
	writerPointer+=8;
	if(writerPointer >= (varNREG << 3))
		writerPointer = 0;
	else
		numberEvents++;

	if(numberEvents > NREG>>1) //if more than NREG/2 events are stored
	{
		SetDDRamAddr(0x47);
		putcXLCD('M');
	}
}

void writeUpdate(void){
	writeToEEPROM(writerEEPROM, (writerPointer & 0xFF00) >> 8);
	writeToEEPROM(writerEEPROM+1, writerPointer & 0x00FF);
}
void readUpdate(void){
	writeToEEPROM(readerEEPROM, (readerPointer & 0xFF00) >> 8);
	writeToEEPROM(readerEEPROM+1, readerPointer & 0x00FF);
}
void NREGUpdate(void){
	writeToEEPROM(NREGEEPROM, (NREG & 0xFF00) >> 8);
	writeToEEPROM(NREGEEPROM+1, NREG & 0x00FF);
}
void usedUpdate(void){
	writeToEEPROM(usedEEPROM, (numberEvents & 0xFF00) >> 8);
	writeToEEPROM(usedEEPROM+1, numberEvents & 0x00FF);
}
