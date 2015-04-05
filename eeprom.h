#ifndef _EEPROM_H
#define _EEPROM_H

#include <i2c.h>

#include "main.h"

#define EEPROMW (0b10100000) // Address and WRITE command
#define EEPROMR (0b10100001)  // Address and READ command

char readFromEEPROM(int address);
void writeToEEPROM(int address, char data);
void EEPROMTesting(char teste);

#endif
