#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>

#define NR_THREADS 2
#define PRIORITY_INTERFACE 1
#define PRIORITY_PROCESSING 1
#define STACKSIZE 4096
#define MAXLINE 50

#define NR_COMMANDS (sizeof(commands)/sizeof(struct command_d))

#define NAME_PROCESSING "thread processing"
#define NAME_INTERFACE "thread interface"

#define forever for(;;)

cyg_thread thread_s[2];
cyg_handle_t t_interface, t_processing;
char stack[2][STACKSIZE];

cyg_mutex_t lock_write, lock_read;

cyg_handle_t mboxH;
cyg_mbox mbox;

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

void getCommand(void){
	char buffer[MAXLINE];
	int i;

	printf("\nCmd> ");

	cyg_mutex_lock(&lock_read);
	fgets(buffer, MAXLINE, stdin);
	cyg_mutex_unlock(&lock_read);

	for(i=0;i<NR_COMMANDS;i++){
		if(strcasestr(line, commands[i].cmd_name) != NULL){
			commands[i].cmd_fnct(buffer);		// chama a função pretendida
		}
	}
	return;
}

void interface(cyg_addrword_t data){
	char buffer[MAXLINE];
	
	forever{
		cyg_mutex_lock(&lock_read);
		fgets(buffer, MAXLINE, stdin);
		cyg_mutex_unlock(&lock_read);

		cyg_mutex_lock(&lock_write);
		printf("[T]: read %s\tSending message to be processed\n", buffer);
		cyg_mutex_unlock(&lock_write);

		cyg_mbox_put(mboxH, buffer);

		cyg_thread_yield();

	}	
}

void processing(cyg_addrword_t data){
	char *buffer;

	forever{

		buffer = (char*) cyg_mbox_get(mboxH);

		cyg_mutex_lock(&lock_write);
		printf("[P]: read %s\tProcessing message\n", buffer);
		cyg_mutex_unlock(&lock_write);

		cyg_thread_yield();
	}
}

int main(void){

	printf("This program was designed to test the processing & interface part of SCE project Part II\n");
	printf("\t- It creates 2 threads, one for handling communication with the user and one to process all the required info\n");
	printf("\tThread 1 is the Interface thread\n
			\tThread 2 is the Processing Thread\n
			\t\tThe interface thread has higher priority so that it can catch the input the user gives immediatly\n");

	// initializing mutex's

	cyg_mutex_init(&lock_write);
	cyg_mutex_init(&lock_read);

	// creating message boxes for communication intraprocess

	cyg_mbox_create(&mboxH, &mbox);

	// creating threads

	cyg_thread_create(PRIORITY_INTERFACE, interface, (cyg_addrword_t) NULL, NAME_INTERFACE, (void *)stack[0], STACKSIZE, &t_interface, &thread_s[0]);
	cyg_thread_create(PRIORITY_PROCESSING, processing, (cyg_addrword_t) NULL, NAME_PROCESSING, (void *)stack[1], STACKSIZE, &t_processing, &thread_s[1]);

	// activating threads, the processing thread has to go first, otherwise inferface can takeover all the control
	
	cyg_thread_resume(t_processing);
	cyg_thread_resume(t_interface);

	exit(0);
}
