#ifndef _SCREEN_UPDATES_H
#define _SCREEN_UPDATES_H

#include <xlcd.h>	// need to solve warning on putrsXLCD()
#include "main.h"

void updateScreen(void);
void updateClock(time LCDtime, char * buffer);
void updateTemp(char * temperature);

#endif
