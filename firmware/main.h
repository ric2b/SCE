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

//#define PMON 5
extern char PMON;
#define TSOM 1
#define NREG 6


#define writerEEPROM 0x7FF8
#define readerEEPROM 0x7FFA
#define NREGEEPROM 0x7FFC
#define usedEEPROM 0x7FFE

#define TIME_BAK_ADDR 0x10
#define VAR_BAK_ADDR 0x20

#define USART_BUF_LEN 100


/* Assume-se que os valores de SOM e EOM nao ocorrem na mensage
m */
#define SOM   0xFD  /* inicio mensagem */
#define EOM   0xFE  /* fim mensagem */

#define CRLG  0xC0  // DONE /* consultar relogio */
#define ARLG  0xC1  /* acertar relogio */
#define CTEL  0xC2  // DONE /* consultar temperatura e luminosidade */
#define CPAR  0xC3  // DONE /* consultar parametros */
#define MPMN  0xC4  /* modificar periodo monitorizacao */
#define CALA  0xC5  // DONE /* consultar alarmes */
#define DALR  0xC6  /* definir alarme relogio */
#define DALT  0xC7  /* definir alarme temperatura */
#define DALL  0xC8  /* definir alarme luminosidade */
#define AALA  0xC9  /* activar/desactivar alarmes */
#define IREG  0xCA  /* informacao sobre registos */
#define TRGC  0xCB  /* transferir registos a partir da posicao corrente */
#define TRGI  0xCC  /* transferir registos a partir do indice e specificado */
#define NMCH  0xCD  // DONE /* notificacao memoria cheia */ //done

#define CMD_OK    0            /* comando realizado com sucesso */
#define CMD_ERRO  0xFF         /* erro no comando */

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

extern volatile char usartReadIndex;
extern volatile char usartReadBuf[USART_BUF_LEN];
extern volatile char usartReadFlag;
extern volatile char usartWriteFlag;


/*----- EEPROM VARIABLES ---------------*/
extern short int writerPointer;
extern short int readerPointer;
extern short int numberEvents;
/* ----------- FUNCTIONS ---------------*/
void int_to_str(int raw, char *str);

#endif
