#ifndef _SENSORS_H
#define _SENSORS_H

#include <i2c.h>

#include "main.h"

#define TC74ADDR (0b10011011) // Address and READ command
#define TC74ADDW (0b10011010)  // Address and WRITE command
#define RTR (0x00)    // TC74 command - read temperature
#define RWCR (0x01)    // TC74 command - read/write (config)

void readTemperature(char * temperature);

#endif
