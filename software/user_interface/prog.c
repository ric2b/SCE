#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>

#define NR_THREADS 3
#define PRIORITY_INTERFACE 10
#define PRIORITY_PROCESSING 10
#define PRIORITY_COMMUNICATION 10
#define STACKSIZE 4096
#define MAXLINE 50

#define NR_COMMANDS (sizeof(commands)/sizeof(struct command_d))

#define NAME_PROCESSING "thread processing"
#define NAME_INTERFACE "thread interface"
#define NAME_COMMUNICATION "thread communication"

#define forever for(;;)

extern void cmd_ini (int, char** );
extern void monitor(void);

cyg_thread thread_s[3];
cyg_handle_t t_interface, t_processing, t_communication;
char stack[3][STACKSIZE];

cyg_mutex_t lock_write, lock_read;

cyg_handle_t mboxH1;

cyg_mbox mbox1;

cyg_handle_t mboxH2;

cyg_mbox mbox2;
/*
struct command_d{
	void (*cmd_fnct)(char *message); // podemos inserir argumentos aqui
	char *cmd_name;
} const commands[] ={
	{cmd_cr,	"cr"},		// consultar relogio
	{cmd_ar,	"ar"},		// acertar relogio 													- precisa de argumentos
	{cmd_ctl,	"ctl"},		// consultar temperatura e luminosidade
	{cmd_cp,	"cp"},		// consultar parametros (NREG, PMON, TSOM)
	{cmd_mpm,	"mpm"},		// modificar perıodo de monitorizacao (segundos - 0 desactiva) 		- precisa de argumentos
	{cmd_ca,	"ca"},		// consultar alarmes (relogio, temp., lumi., activos/inactivos)
	{cmd_dar,	"dar"},		// definir alarme relogio											- precisa de argumentos
	{cmd_dat,	"dat"},		// definir alarme temperatura										- precisa de argumentos
	{cmd_dal,	"dal"},		// definir alarme luminosidade										- precisa de argumentos
	{cmd_aa,	"aa"},		// activar/desactivar alarmes
	{cmd_ir,	"ir"},		// informacao sobre registos (NREG, nr, iescrita, ileitura)
	{cmd_trc,	"trc"},		// transferir n registos (ind. leit. corrente)						- precisa de argumentos
	{cmd_tri,	"tri"},		// transferir n registos a partir do ındice i						- precisa de argumentos
	{cmd_irl,	"irl"},		// informacao registos locais (NRBUF, nr, iescrita, ileitura)
	{cmd_lr,	"lr"},		// listar n registos (mem. local) a partir do ındice i				- precisa de argumentos	
	{cmd_er,	"er"},		// eliminar registos locais
	{cmd_cpt,	"cpt"},		// consultar perıodo de transferencia
	{cmd_mpt,	"mpt"},		// modificar perıodo de transferencia (minutos - 0 desactiva)		- precisa de argumentos
	{cmd_lar,	"lar"},		// listar alarmes relogio entre t1 e t2								- precisa de argumentos
	{cmd_lat,	"lat"},		// listar alarmes temperatura entre t1 e t2							- precisa de argumentos
	{cmd_lal,	"lal"},		// listar alarmes luminosidade entre t1 e t2						- precisa de argumentos
	{cmd_iga,	"iga"},		// informacao gestao alarmes (definicao e activacao)
	{cmd_ig, 	"ig"}		// informacao geral (inıcio, relogio, memoria, perıodo monit.)
};
*/

void putMSG(char *buffer, int box){
	if(box == 0){
		cyg_mbox_put(mboxH1, buffer);
	}else if(box == 1){
		cyg_mbox_put(mboxH2, buffer);
	}
}

char *getMSG(int box){
	if(box == 0){
		return cyg_mbox_get(mboxH1);
	}else if(box == 1){
		return cyg_mbox_get(mboxH2);
	}
	return "N";
}

void interface(cyg_addrword_t data){
	cmd_ini(0, NULL);
	monitor();
	return;
}

void processing(cyg_addrword_t data){
	char *buffer;

	forever{

		buffer = cyg_mbox_get(mboxH1);

		cyg_mutex_lock(&lock_write);
		printf("[PROCESSING] Processing request %s\n", buffer);
		cyg_mutex_unlock(&lock_write);

		cyg_thread_yield();

	}
}

void communication(cyg_addrword_t data){
	char *buffer;
	forever{

		buffer = cyg_mbox_get(mboxH2);

		cyg_mutex_lock(&lock_write);
		printf("[COMMUNICATION] Talking with board %s\n", buffer);
		cyg_mutex_unlock(&lock_write);

		cyg_thread_yield();

	}
}

int main(void){
	// initializing mutex's

	cyg_mutex_init(&lock_write);
	cyg_mutex_init(&lock_read);

	// creating message boxes for communication intraprocess

	cyg_mbox_create(&mboxH1, &mbox1);
	cyg_mbox_create(&mboxH2, &mbox2);

	// creating threads

	cyg_thread_create(PRIORITY_INTERFACE, interface, (cyg_addrword_t) NULL, NAME_INTERFACE, (void *)stack[0], STACKSIZE, &t_interface, &thread_s[0]);
	cyg_thread_create(PRIORITY_PROCESSING, processing, (cyg_addrword_t) NULL, NAME_PROCESSING, (void *)stack[1], STACKSIZE, &t_processing, &thread_s[1]);
	cyg_thread_create(PRIORITY_COMMUNICATION, communication, (cyg_addrword_t) NULL, NAME_COMMUNICATION, (void *)stack[2], STACKSIZE, &t_communication, &thread_s[2]);

	// activating threads, the processing thread has to go first, otherwise inferface can takeover all the control
	
	cyg_thread_resume(t_processing);
	cyg_thread_resume(t_interface);
	cyg_thread_resume(t_communication);

	exit(0);
}
