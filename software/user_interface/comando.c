/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Maio 2008
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <cyg/io/io.h>

Cyg_ErrNo err;
cyg_io_handle_t serH;

#define SOM   0xFD  /* inicio mensagem */
#define EOM   0xFE  /* fim mensagem */
#define CRLG  0xC0  /* consultar relogio */
#define ARLG  0xC1  /* acertar relogio */
#define CTEL  0xC2  /* consultar temperatura e luminosidade */
#define CPAR  0xC3  /* consultar parametros */
#define MPMN  0xC4  /* modificar periodo monitorizacao */
#define CALA  0xC5  /* consultar alarmes */
#define DALR  0xC6  /* definir alarme relogio */
#define DALT  0xC7  /* definir alarme temperatura */
#define DALL  0xC8  /* definir alarme luminosidade */
#define AALA  0xC9  /* activar/desactivar alarmes */
#define IREG  0xCA  /* informacao sobre registos */
#define TRGC  0xCB  /* transferir registos a partir da posicao corrente */
#define TRGI  0xCC  /* transferir registos a partir do indice especificado */
#define NMCH  0xCD  /* notificacao memoria cheia */
#define CMD_OK    0            /* comando realizado com sucesso */
#define CMD_ERRO  0xFF         /* erro no comando */

#define COMMUNICATION_THREAD 1
#define PROCESSING_THREAD 0

extern void putMSG(char *buffer, int box); // Communication thread -> box == 1

/*-------------------------------------------------------------------------+
| Function: cmd_cr - Consultar relogio
+--------------------------------------------------------------------------*/ 
void cmd_cr (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	//sprintf(buffer, "%x", CRLG);		// can also use %02x
	buffer[0] = CRLG;
	buffer[1] = 0;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}
/*-------------------------------------------------------------------------+
| Function: cmd_ar - Acertar relogio
+--------------------------------------------------------------------------*/ 
void cmd_ar (int argc, char **argv)
{
	if(argc < 4){
		printf("Not enough arguments to cmd ar\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*9);
	sprintf(buffer, "%x %c %c %c", ARLG, argv[1][0], argv[2][0], argv[3][0]);
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}
/*-------------------------------------------------------------------------+
| Function: cmd_ctl - Consultar temperatura e luminosidade
+--------------------------------------------------------------------------*/ 
void cmd_ctl (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	sprintf(buffer, "%x", CTEL);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}
/*-------------------------------------------------------------------------+
| Function: cmd_cp - Consultar parametros
+--------------------------------------------------------------------------*/ 
void cmd_cp (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	sprintf(buffer, "%x", CPAR);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_mpm - Modificar periodo de monitorizacao
+--------------------------------------------------------------------------*/ 
void cmd_mpm (int argc, char **argv)
{
	if( argc < 2){
		printf("Not enough arguments to call mpm\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*5);
	sprintf(buffer, "%x %c", MPMN, argv[1][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_ca - Consultar alarmes
+--------------------------------------------------------------------------*/ 
void cmd_ca (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	sprintf(buffer, "%x", CALA);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_dar - Definir alarme relogio
+--------------------------------------------------------------------------*/ 
void cmd_dar (int argc, char **argv)
{
	if(argc < 4){
		printf("Not enough arguments to call dar\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*9);
	sprintf(buffer, "%x %c %c %c", DALR, argv[1][0], argv[2][0], argv[3][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_dat - definir alarme temperatura
+--------------------------------------------------------------------------*/ 
void cmd_dat (int argc, char **argv)
{
	if( argc < 2){
		printf("Not enough arguments to call dat\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*4);
	sprintf(buffer, "%x %c", DALT, argv[1][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_dal - definir alarme temperatura
+--------------------------------------------------------------------------*/ 
void cmd_dal (int argc, char **argv)
{
	if( argc < 2 ){
		printf("Not enough arguments to call dal");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*4);
	sprintf(buffer, "%x %c", DALL, argv[1][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_aa - Activer/Desativar alarmes
+--------------------------------------------------------------------------*/ 
void cmd_aa (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	sprintf(buffer, "%x", AALA);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_ir - informacao sobre registos
+--------------------------------------------------------------------------*/ 
void cmd_ir (int argc, char **argv)
{
	char *buffer = (char*) malloc(sizeof(char)*2);
	sprintf(buffer, "%x", IREG);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_trc - transferir n registos
+--------------------------------------------------------------------------*/ 
void cmd_trc (int argc, char **argv)
{
	if( argc < 2 ){
		printf("Not enough arguments to call trc\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*4);
	sprintf(buffer, "%x %c", TRGC, argv[1][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_tri - transferir n registos apartir do indice i
+--------------------------------------------------------------------------*/ 
void cmd_tri (int argc, char **argv)
{
	if( argc < 3 ){
		printf("Not enough arguments to call tri\n");
		return;
	}
	char *buffer = (char*) malloc(sizeof(char)*6);
	sprintf(buffer, "%x %c %c", TRGI, argv[1][0], argv[2][0]);		// can also use %02x
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_irl - informacao registos locais
+--------------------------------------------------------------------------*/ 
void cmd_irl (int argc, char **argv)
{
	// TO BE CHECKED
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_lr
+--------------------------------------------------------------------------*/ 
void cmd_lr (int argc, char **argv)
{
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_er
+--------------------------------------------------------------------------*/ 
void cmd_er (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_cpt
+--------------------------------------------------------------------------*/ 
void cmd_cpt (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_mpt
+--------------------------------------------------------------------------*/ 
void cmd_mpt (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_lar
+--------------------------------------------------------------------------*/ 
void cmd_lar (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_lat
+--------------------------------------------------------------------------*/ 
void cmd_lat (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_lal
+--------------------------------------------------------------------------*/ 
void cmd_lal (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_iga
+--------------------------------------------------------------------------*/ 
void cmd_iga (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_ig
+--------------------------------------------------------------------------*/ 
void cmd_ig (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/ 
void cmd_sair (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_test (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nargv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_ems - enviar mensagem (string)
+--------------------------------------------------------------------------*/ 
void cmd_ems (int argc, char **argv)
{
  unsigned int n;

  if (argc > 1) {
    n = strlen(argv[1]) + 1;
    err = cyg_io_write(serH, argv[1], &n);
    printf("io_write err=%x, n=%d str=%s\n", err, n, argv[1]);
  }
  else {
    n = 10;
    err = cyg_io_write(serH, "123456789", &n);
    printf("io_write err=%x, n=%d str=%s\n", err, n, "123456789");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_emh - enviar mensagem (hexadecimal)
+--------------------------------------------------------------------------*/ 
void cmd_emh (int argc, char **argv)
{
  unsigned int n, i;
  unsigned char bufw[50];

  if ((n=argc) > 1) {
    n--;
    if (n > 50) n = 50;
    for (i=0; i<n; i++)
      //      sscanf(argv[i+1], "%hhx", &bufw[i]);
      {unsigned int x; sscanf(argv[i+1], "%x", &x); bufw[i]=(unsigned char)x;}
    err = cyg_io_write(serH, bufw, &n);
    printf("io_write err=%x, n=%d\n", err, n);
    for (i=0; i<n; i++)
      printf("buf[%d]=%x\n", i, bufw[i]);
  }
  else {
    printf("nenhuma mensagem!!!\n");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_rms - receber mensagem (string)
+--------------------------------------------------------------------------*/ 
void cmd_rms (int argc, char **argv)
{
  unsigned int n;
  char bufr[50];

  if (argc > 1) n = atoi(argv[1]);
  if (n > 50) n = 50;
  err = cyg_io_read(serH, bufr, &n);
  printf("io_read err=%x, n=%d buf=%s\n", err, n, bufr);
}

/*-------------------------------------------------------------------------+
| Function: cmd_rmh - receber mensagem (hexadecimal)
+--------------------------------------------------------------------------*/ 
void cmd_rmh (int argc, char **argv)
{
  unsigned int n, i;
  unsigned char bufr[50];

  if (argc > 1) n = atoi(argv[1]);
  if (n > 50) n = 50;
  err = cyg_io_read(serH, bufr, &n);
  printf("io_read err=%x, n=%d\n", err, n);
  for (i=0; i<n; i++)
    printf("buf[%d]=%x\n", i, bufr[i]);
}


/*-------------------------------------------------------------------------+
| Function: cmd_ini - inicializar dispositivo
+--------------------------------------------------------------------------*/ 
void cmd_ini(int argc, char **argv)
{
  printf("io_lookup\n");
  if ((argc > 1) && (argv[1][0] = '1'))
    err = cyg_io_lookup("/dev/ser1", &serH);
  else err = cyg_io_lookup("/dev/ser0", &serH);
  printf("lookup err=%x\n", err);
}

