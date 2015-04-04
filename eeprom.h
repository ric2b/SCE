#ifndef _SENSORS_H
#define _SENSORS_H

#include <i2c.h>

#include "main.h"

void EEPROMTesting();

void readFromEEPROM(char *buffer);

void writeToEEPROM(char buffer);


#endif