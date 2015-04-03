#ifndef _MAIN_H
#define _MAIN_H

#include <p18f452.h>  /* for the special function register declarations */
#include <timers.h>
#include <xlcd.h>
#include <delays.h>
#include <ADC.h>
#include <portb.h>

/* ----------- DEFINES ---------------*/
#define debug

#ifndef debug
#define F_CPU 4000	// in kHz
#else
#define F_CPU 1000	// in kHz. used when in debug mode
#endif

#define PMON 5
#define TSOM 2
#define NREG 30

#define TC74ADDR (0b10011011) // Address and READ command
#define TC74ADDW (0b10011010)  // Address and WRITE command
#define RTR (0x00)    // TC74 command - read temperature
#define RWCR (0x01)    // TC74 command - read/write (config)

/* ----------- STRUCTS ---------------*/
typedef struct time
{
	char seconds;
	char minutes;
	char hours;
} time;

/* ----------- GLOBALS ---------------*/
volatile time clock;
time alarm;
char temperature_treshold = 99;
char lumos_treshold = 10;
char updateLCD = 1;
// these variables are changed by ISRs
volatile char configMode = 0;
volatile char configModeUpdated = 0;
volatile char update_hours = 1;
volatile char update_minutes = 1;
volatile char update_seconds = 1;
volatile char update_alt = 1;
volatile char update_a = 1;
volatile char update_P = 1;
volatile char update_temp = 1;
volatile char update_M = 1;
volatile char update_lumus = 1;


/* ----------- FUNCTIONS ---------------*/
void int_to_str(int raw, char *str);

#endif
