#ifndef _EVENTS_H
#define _EVENTS_H

#include "main.h"

#define NREG 20		// We must change this to be a parameter that the user inputs, but for the sake of testing lets leave it like this for now

short int readPointer;
short int writePointer;

void addToEEPROM(int code);

#endif