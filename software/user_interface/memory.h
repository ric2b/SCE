#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cyg/kernel/kapi.h>

#define NRBUF 150
#define CLOCK_ALARM 7
#define TEMPE_ALARM 8
#define LUMUS_ALARM 9
#define MONIT_ALARM 6

typedef struct _circularbuf_{
	char data[NRBUF][8];
}circularbuf;

unsigned char wPointer;
unsigned char rPointer;
unsigned char regWritten;

circularbuf shared_mem;

cyg_mutex_t lock_circularbuf;

void ini_circularbuf(void);
void clear_circularbuf(void);
void put_in_mem(char to_store[8]);
char *read_from_mem(void);
char **read_n_regs(int n);
char **read_n_regs_from_i(int n, int i);
void getClockAlarms(int h1, int m1, int s1, int h2, int m2, int s2);
void getTempeAlarms(int h1, int m1, int s1, int h2, int m2, int s2);
void getLumusAlarms(int h1, int m1, int s1, int h2, int m2, int s2);
void getLocalReg(void);
void infoGestaoAlarms(void);

#endif
