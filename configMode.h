#ifndef _CONFIG_MODE_H
#define _CONFIG_MODE_H

#include "main.h"
#include "sensors.h"
#include "delays.h"

void config(void);
char cursorState( char ignore );
char changeValueWithS2(char * value);

#endif
