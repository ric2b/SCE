#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cyg/kernel/kapi.h>

void cmd_sos (int argc, char **argv);
int my_getline (char** argv, int argvsize);
void monitor (void);

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

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

#define forever for(;;)

#endif
