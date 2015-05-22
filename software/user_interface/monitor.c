#include "memory.h"
#include "comando.h"
#include "prog.h"
#include "monitor.h"

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

	// funcoes thread de comunicacao
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

	// funcoes thread de interface
	{cmd_irl,	"irl","                 informacao registos locais"},
	{cmd_lr,	"lr","<n> <i>           listar n registos locais a partir de i"},
	{cmd_er,	"er","                  eliminar registos locais"},

	// funcoes thread de processamento
	{cmd_cpt,	"cpt","                 consultar periodo de transferencia"},
	{cmd_mpt,	"mpt","<p>              modificar periodo de transferencia"},
	{cmd_lar,	"lar","<h> <m> <s>,<h> <m> <s> listar alarmes clk entre t1 e t2"},
	{cmd_lat,	"lat","<h> <m> <s>,<h> <m> <s> listar alarmes temp entre t1 e t2"},
	{cmd_lal,	"lal","<h> <m> <s>,<h> <m> <s> listar alarmes lumus entre t1 e t2"},
	{cmd_iga,	"iga","                 informacao gestao alarmes"},
	{cmd_ig, 	"ig","                  informacao geral"}
};

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
	int i;

	fgets(line, MAX_LINE, stdin);

	for(i = 24; i < NCOMMANDS; i++){
		if( strstr(line, commands[i].cmd_name) != NULL )
		{
			argv[0] = strdup(line);
			return 1;
		}
	}

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
					putMSG(argv[0], PROCESSING_BOX);
				}else{
					commands[i].cmd_fnct(argc, argv);
				}
				cyg_thread_yield();
			}
			else  
				puts(InvalMsg);
		} /* if my_getline */
	} /* forever */
}
