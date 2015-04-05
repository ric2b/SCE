#include "events.h"

void addToEEPROM(int code){
	char data[8];
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
			data[4]=temperature_treshold;
			data[5]=0;
			data[6]=0;
			data[7]=0;
			break;
		case 5:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=lumus_treshold;
			data[5]=0;
			data[6]=0;
			data[7]=0;
			break;
		case 6:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;
			data[6]=0;
			data[7]=0;
			break;
		case 7:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;
			data[6]=0;
			data[7]=0;
			break;
		case 8:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;								//there is no global var for lum
			data[6]=0;
			data[7]=0;
			break;
		case 9:
			data[0]=clock.hours;
			data[1]=clock.minutes;
			data[2]=clock.seconds;
			data[3]=code;
			data[4]=temp;
			data[5]=lumus;								
			data[6]=0;
			data[7]=0;
			break;
	}
	// write8BToEEPROM(writePointer, data); pointer isnt ready to use yet
	writePointer++;
	if(writePointer >= NREG)
	{
		writePointer = 0;
	}
}