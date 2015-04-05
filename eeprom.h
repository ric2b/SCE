#ifndef _EEPROM_H
#define _EEPROM_H

#include <i2c.h>

#include "main.h"

void EEPROMTesting(char teste);

char readFromEEPROM(int address);

void writeToEEPROM(int address, char data);


#endif
