/***************************************************************************
  | File: monitor.c
  |
  | Autor: Carlos Almeida (IST), from work by Jose Rufino (IST/INESC), 
  |        from an original by Leendert Van Doorn
  | Data:  Nov 2002
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cyg/kernel/kapi.h>

/*-------------------------------------------------------------------------+
  | Headers of command functions
  +--------------------------------------------------------------------------*/ 
extern void cmd_sair (int, char** );
extern void cmd_ini (int, char** );
extern void cmd_ems (int, char** );
extern void cmd_emh (int, char** );
extern void cmd_rms (int, char** );
extern void cmd_rmh (int, char** );
extern void cmd_test (int, char** );
extern void cmd_cr (int, char** );
extern void cmd_ar (int, char** );
extern void cmd_ctl (int, char** );
extern void cmd_cp (int, char** );
extern void cmd_mpm (int, char** );
extern void cmd_ca (int, char** );
extern void cmd_dar (int, char** );
extern void cmd_dat (int, char** );
extern void cmd_dal (int, char** );
extern void cmd_aa (int, char** );
extern void cmd_ir (int, char** );
extern void cmd_trc (int, char** );
extern void cmd_tri (int, char** );
extern void cmd_irl (int, char** );
extern void cmd_lr (int, char** );
extern void cmd_er (int, char** );
extern void cmd_cpt (int, char** );
extern void cmd_mpt (int, char** );
extern void cmd_lar (int, char** );
extern void cmd_lat (int, char** );
extern void cmd_lal (int, char** );
extern void cmd_iga (int, char** );
extern void cmd_ig (int, char** );
void cmd_sos  (int, char** );
extern char *getMSG(int box);
/*-------------------------------------------------------------------------+
  | Variable and constants definition
  +--------------------------------------------------------------------------*/ 
const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";

struct 	command_d {
	void  (*cmd_fnct)(int, char**);
	char*	cmd_name;
	char*	cmd_help;
} const commands[] = {
	{cmd_sos,  "sos","                 help"},
	{cmd_sair, "sair","                sair"},
	{cmd_ini,  "ini","<d>              inicializar dispositivo (0/1) ser0/ser1"},
	{cmd_ems,  "ems","<str>            enviar mensagem (string)"},
	{cmd_emh,  "emh","<h1> <h2> <...>  enviar mensagem (hexadecimal)"},
	{cmd_rms,  "rms","<n>              receber mensagem (string)"},
	{cmd_rmh,  "rmh","<n>              receber mensagem (hexadecimal)"},
	{cmd_test, "teste","<arg1> <arg2>  comando de teste"},
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

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

/*-------------------------------------------------------------------------+
  | Function: cmd_sos - provides a rudimentary help
  +--------------------------------------------------------------------------*/ 
void cmd_sos (int argc, char **argv)
{
	int i;

	printf("%s\n", TitleMsg);
	for (i=0; i<NCOMMANDS; i++)
		printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
  | Function: getline        (called from monitor) 
  +--------------------------------------------------------------------------*/ 
int my_getline (char** argv, int argvsize)
{
	static char line[MAX_LINE];
	char *p;
	int argc;

	fgets(line, MAX_LINE, stdin);

	/* Break command line into an o.s. like argument vector,
	   i.e. compliant with the (int argc, char **argv) specification --------*/

	for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
		p = strtok(p, " \t\n");
		argv[argc] = p;
		if (p == NULL) return argc;
	}
	argv[argc] = p;
	return argc;
}

/*-------------------------------------------------------------------------+
  | Function: monitor        (called from main) 
  +--------------------------------------------------------------------------*/ 
void monitor (void)
{
	static char *argv[ARGVECSIZE+1], *p;
	int argc, i;

	printf("%s Type sos for help\n", TitleMsg);
	for (;;) {
		printf("\nCmd> ");
		/* Reading and parsing command line  ----------------------------------*/
		if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
			for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
			for (i = 0; i < NCOMMANDS; i++) 
				if (strcmp(argv[0], commands[i].cmd_name) == 0) 
					break;
			/* Executing commands -----------------------------------------------*/
			if (i < NCOMMANDS){
				commands[i].cmd_fnct (argc, argv);
				cyg_thread_yield();
			}
			else  
				printf("%s", InvalMsg);
		} /* if my_getline */
	} /* forever */
}
