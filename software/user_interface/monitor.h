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


#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

#define forever for(;;)

#endif
