#ifndef PROG_H
#define PROG_H

#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <unistd.h>

#define NR_THREADS 3
#define PRIORITY_INTERFACE 8
#define PRIORITY_PROCESSING 8
#define PRIORITY_COMMUNICATION 8
#define STACKSIZE 4096
#define MAXLINE 50

#define NR_COMMANDS (sizeof(commands)/sizeof(struct command_d))

#define NAME_PROCESSING "thread processing"
#define NAME_INTERFACE "thread interface"
#define NAME_COMMUNICATION_S "thread communication s"
#define NAME_COMMUNICATION_R "thread communication r"


#define SOM   0xFD  /* inicio mensagem */
#define EOM   0xFE  /* fim mensagem */

#define forever for(;;)

Cyg_ErrNo err;
cyg_io_handle_t serH;

char periodo_transferencia; // EM MINUTOS

cyg_tick_count_t init_tick;
cyg_tick_count_t last_tick;

cyg_thread thread_s[4];
cyg_handle_t t_interface, t_processing, t_communication_s, t_communication_r;
char stack[4][STACKSIZE];

cyg_mutex_t lock_write, lock_read;

cyg_handle_t mboxH1;	//Processing Thread MailBox

cyg_mbox mbox1;

cyg_handle_t mboxH2;	//Communication Thread MailBox

cyg_mbox mbox2;

void setPerTransferencia(char a);
char getPerTransferencia(void);
void getRegistersPer(void);
void sendMSGToBoard(char *msg, char size);
char *getMSGfromBoard(void);
char getSizeOfMSG(char * msg);
void putMSG(char *buffer, int box);
void interface(cyg_addrword_t data);
void processing(cyg_addrword_t data);
void communication_s(cyg_addrword_t data);
void communication_r(cyg_addrword_t data);

#endif
