#include "memory.h"
#include "comando.h"
#include "prog.h"
#include "monitor.h"

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
			argv[0] = strdup(line);
		return 1;
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
				}
				commands[i].cmd_fnct(argc, argv);
				cyg_thread_yield();
			}
			else  
				puts(InvalMsg);
		} /* if my_getline */
	} /* forever */
}
