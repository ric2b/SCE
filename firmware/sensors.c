#include "sensors.h"

void readTemperature(char * temperature)
{
	char value;
	do{
		IdleI2C();
		StartI2C(); IdleI2C();
		WriteI2C(TC74ADDW); IdleI2C(); // slave address + write
		WriteI2C(RWCR); IdleI2C(); // Enable Read Write Config
		RestartI2C(); IdleI2C(); // ?????

		WriteI2C(TC74ADDR); IdleI2C(); // slave address + read
		value = ReadI2C(); IdleI2C(); // Read from slave
		NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
		StopI2C(); IdleI2C();
	} while (!(value & 0x40)); // Verify if the values are valid

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(TC74ADDW); IdleI2C(); // slave address + write
	WriteI2C(RTR); IdleI2C(); // Enable Read Write Config
	RestartI2C(); IdleI2C(); // 

	WriteI2C(TC74ADDR); IdleI2C(); // slave address + read
	value = ReadI2C(); IdleI2C(); // Read from slave
	NotAckI2C(); IdleI2C(); // No ACK from master means end of transmission
	StopI2C(); IdleI2C();

	if(value>=0)
	{
		temperature[0] = value/10 + '0';
		temperature[1] = value%10 + '0';
	}
	else
	{
		temperature[0] = 'N';
		temperature[1] = 'E';
	}
	temp = value;	// updated global variable for temperature

	return;
}
