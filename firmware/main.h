#ifndef _MAIN_H
#define _MAIN_H

#include <p18f452.h>  /* for the special function register declarations */
#include <timers.h>
#include <xlcd.h>
#include <delays.h>
#include <ADC.h>
#include <portb.h>
#include <i2c.h>

/* ----------- DEFINES ---------------*/
//#define debug
#define WDT_DISABLED
//#define proteus
#define BusyXLCD() 0

#ifndef debug
#define F_CPU 4000	// in kHz
#else
#define F_CPU 4000	// in kHz. used when in debug mode
#endif

#define PMON 5
#define TSOM 1
#define NREG 6


#define writerEEPROM 0x7FF8
#define readerEEPROM 0x7FFA
#define NREGEEPROM 0x7FFC
#define usedEEPROM 0x7FFE

#define TIME_BAK_ADDR 0x10
#define VAR_BAK_ADDR 0x20

/* ----------- STRUCTS ---------------*/
typedef struct time
{
	char seconds;
	char minutes;
	char hours;
} time;

/* ----------- GLOBALS ---------------*/
extern time alarm;
extern time * clockToChange;
extern char temperature_threshold;
extern char lumus_threshold;
extern char updateLCD;
extern char lumus;
extern char temp;
extern char alarmMask;
extern char temp_last;
extern char lumus_last;
// these variables are changed by ISRs
extern volatile time clock;
extern volatile char configMode;
extern volatile char configModeUpdated;
extern volatile char update_hours;
extern volatile char update_minutes;
extern volatile char update_seconds;
extern volatile char update_alt;
extern volatile char update_temp;
extern volatile char update_lumus;
extern volatile char pmon_counter;
extern volatile char updateTimeAlarm;
extern volatile char buzzTimer;
extern volatile char sleeping;


/*----- EEPROM VARIABLES ---------------*/
extern short int writerPointer;
extern short int readerPointer;
extern short int numberEvents;
/* ----------- FUNCTIONS ---------------*/
void int_to_str(int raw, char *str);

#endif
