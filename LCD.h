#ifndef _SCREEN_UPDATES_H
#define _SCREEN_UPDATES_H

#include <xlcd.h>

#include "main.h"
#include "sensors.h"

void updateScreen(void);
void updateClock(time LCDtime, char * buffer);
void updateTemp(char * temperature);

#endif
