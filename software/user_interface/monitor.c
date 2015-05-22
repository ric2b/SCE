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
extern void cmd_sair (int, char** );		// 0
extern void cmd_ini (int, char** );			// 1
extern void cmd_ems (int, char** );			// 2
extern void cmd_emh (int, char** );			// 3
extern void cmd_rms (int, char** );			// 4
extern void cmd_rmh (int, char** );			// 5
extern void cmd_test (int, char** );		// 6
extern void cmd_cr (int, char** );			// 7
extern void cmd_ar (int, char** );			// 8
extern void cmd_ctl (int, char** );			// 9
extern void cmd_cp (int, char** );			// 10
extern void cmd_mpm (int, char** );			// 11
extern void cmd_ca (int, char** );			// 12
extern void cmd_dar (int, char** );			// 13
extern void cmd_dat (int, char** );			// 14
extern void cmd_dal (int, char** );			// 15
extern void cmd_aa (int, char** );			// 16
extern void cmd_ir (int, char** );			// 17
extern void cmd_trc (int, char** );			// 18
extern void cmd_tri (int, char** );			// 19
extern void cmd_irl (int, char** );			// 20
extern void cmd_lr (int, char** );			// 21
extern void cmd_er (int, char** );			// 22
extern void cmd_cpt (int, char** );			// 23
extern void cmd_mpt (int, char** );			// 24
extern void cmd_lar (int, char** );			// 25
extern void cmd_lat (int, char** );			// 26
extern void cmd_lal (int, char** );			// 27
extern void cmd_iga (int, char** );			// 28
extern void cmd_ig (int, char** );			// 29
void cmd_sos  (int, char** );


extern char *getMSG(int box);
extern void putMSG(char *buffer, int box);
#define PROCESSING_BOX 0

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
	{cmd_sos,  	"sos","                 help"},
	{cmd_sair, 	"sair","                sair"},
	{cmd_ini,  	"ini","<d>              inicializar dispositivo (0/1) ser0/ser1"},
	{cmd_ems,  	"ems","<str>            enviar mensagem (string)"},
	{cmd_emh,  	"emh","<h1> <h2> <...>  enviar mensagem (hexadecimal)"},
	{cmd_rms,  	"rms","<n>              receber mensagem (string)"},
	{cmd_rmh,  	"rmh","<n>              receber mensagem (hexadecimal)"},
	{cmd_test, 	"teste","<arg1> <arg2>  comando de teste"},
	{cmd_cr,	"cr","                  consultar relogio"},
	{cmd_ar,	"ar","<h> <m> <s>       acertar relogio"},
	{cmd_ctl,	"ctl","                 consultar temp e lum"},
	{cmd_cp,	"cp","                  consultar parametros"},
	{cmd_mpm,	"mpm","<p>              modificar pmon"},
	{cmd_ca,	"ca","                  consultar alarmes"},
	{cmd_dar,	"dar","<h> <m> <s>      definir alarme clock"},
	{cmd_dat,	"dat","<t>              definir alarme temp"},
	{cmd_dal,	"dal","<l>              definir alarm lumus"},
	{cmd_aa,	"aa","                  activar/desativar alarmes"},
	{cmd_ir,	"ir","                  informacao sobre registos"},
	{cmd_trc,	"trc","<n>              transferir n registos"},
	{cmd_tri,	"tri","<n> <i>          transferir n registos a partir de i"},
	{cmd_irl,	"irl","                 informacao registos locais"},
	{cmd_lr,	"lr","<n> <i>           listar n registos locais a partir de i"},
	{cmd_er,	"er","                  eliminar registos locais"},
	{cmd_cpt,	"cpt","                 consultar periodo de transferencia"},
	{cmd_mpt,	"mpt","<p>              modificar periodo de transferencia"},
	{cmd_lar,	"lar","<h> <m> <s>,<h> <m> <s> listar alarmes clk entre t1 e t2"},
	{cmd_lat,	"lat","<h> <m> <s>,<h> <m> <s> listar alarmes temp entre t1 e t2"},
	{cmd_lal,	"lal","<h> <m> <s>,<h> <m> <s> listar alarmes lumus entre t1 e t2"},
	{cmd_iga,	"iga","                 informacao gestao alarmes"},
	{cmd_ig, 	"ig","                  informacao geral"}
};

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

#define forever for(;;)

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

	for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) 
	{
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
	forever 
	{
		printf("\nCmd> ");
		/* Reading and parsing command line  ----------------------------------*/
		if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
			for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
			for (i = 0; i < NCOMMANDS; i++) 
				if (strcmp(argv[0], commands[i].cmd_name) == 0) 
					break;
			/* Executing commands -----------------------------------------------*/
			if (i < NCOMMANDS){
				if( i >= 23){
					//putMSG(, PROCESSING_BOX);
				}
				commands[i].cmd_fnct(argc, argv);
				cyg_thread_yield();
			}
			else  
				puts(InvalMsg);
		} /* if my_getline */
	} /* forever */
}
