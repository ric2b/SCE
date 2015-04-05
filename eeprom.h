#ifndef _EEPROM_H
#define _EEPROM_H

#include <i2c.h>

#include "main.h"

void EEPROMTesting(char teste);

void readFromEEPROM(char *buffer);

void writeToEEPROM(char buffer);


#endif
