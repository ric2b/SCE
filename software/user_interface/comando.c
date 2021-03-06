/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Maio 2008
***************************************************************************/
#include "memory.h"
#include "comando.h"
#include "prog.h"
#include "monitor.h"
/*-------------------------------------------------------------------------+
| Function: cmd_cr - Consultar relogio
+--------------------------------------------------------------------------*/ 
void cmd_cr (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = CRLG;		// can also use %02x
	buffer[1] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*9);
	buffer[0] = ARLG;
	int val;
	sscanf(argv[1], "%d", &val);
	printf("\n\n%d\n\n", val);
	buffer[1] = (unsigned char) val;
	sscanf(argv[2], "%d", &val);
	buffer[2] = (unsigned char) val;
	sscanf(argv[3], "%d", &val);
	buffer[3] = (unsigned char) val;
	buffer[4] = MSGEND;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}
/*-------------------------------------------------------------------------+
| Function: cmd_ctl - Consultar temperatura e luminosidade
+--------------------------------------------------------------------------*/ 
void cmd_ctl (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = CTEL;
	buffer[1] = MSGEND;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}
/*-------------------------------------------------------------------------+
| Function: cmd_cp - Consultar parametros
+--------------------------------------------------------------------------*/ 
void cmd_cp (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = CPAR;
	buffer[1] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*3);
	buffer[0] = MPMN;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	buffer[2] = MSGEND;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_ca - Consultar alarmes
+--------------------------------------------------------------------------*/ 
void cmd_ca (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = CALA;
	buffer[1] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*5);
	buffer[0] = DALR;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	sscanf(argv[2], "%d", &val);
	buffer[2] = val;
	sscanf(argv[3], "%d", &val);
	buffer[3] = val;
	buffer[4] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*3);
	buffer[0] = DALT;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	buffer[2] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*4);
	buffer[0] = DALL;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	buffer[2] = MSGEND;

	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_aa - Activer/Desativar alarmes
+--------------------------------------------------------------------------*/ 
void cmd_aa (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = AALA;
	buffer[1] = MSGEND;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_ir - informacao sobre registos
+--------------------------------------------------------------------------*/ 
void cmd_ir (int argc, char **argv)
{
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*2);
	buffer[0] = IREG;
	buffer[1] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*3);
	buffer[0] = TRGC;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	buffer[2] = MSGEND;
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
	unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char)*4);
	buffer[0] = TRGI;
	int val;
	sscanf(argv[1], "%d", &val);
	buffer[1] = (char) val;
	sscanf(argv[2], "%d", &val);
	buffer[2] = (char) val;
	buffer[3] = MSGEND;
	putMSG(buffer, COMMUNICATION_THREAD);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_irl - informacao registos locais
+--------------------------------------------------------------------------*/ 
void cmd_irl (int argc, char **argv)
{
	getLocalReg();
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_lr - listar n registos locais a partir do indice i
+--------------------------------------------------------------------------*/ 
void cmd_lr (int argc, char **argv)
{
	int n;
	int i;
	char **all_reg;
	int j;

	if( argc < 3)
		printf("Not enough arguments to lr\n");

	sscanf(argv[1], "%d", &n);
	sscanf(argv[2], "%d", &i);
	all_reg = read_n_regs_from_i(n,i);

	printf("Registers from %d:\n", i);
	for(j=0; j< n; j++){
		printf("[REG %d] - %s", j,all_reg[j]);
	}

	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_er - eliminar registos locais
+--------------------------------------------------------------------------*/ 
void cmd_er (int argc, char **argv)
{
	clear_circularbuf();
	return;
}


// A PARTIR DAQUI TEM QUE SER PARA POR NA THREAD DE PROCESSAMENTO


/*-------------------------------------------------------------------------+
| Function: cmd_cpt - consultar período de transferência
+--------------------------------------------------------------------------*/ 
void cmd_cpt (int argc, char **argv)
{
	printf("Periodo de transferência: %c", getPerTransferencia());
	return;
}	

/*-------------------------------------------------------------------------+
| Function: cmd_mpt - modificar período de transferência
+--------------------------------------------------------------------------*/ 
void cmd_mpt (int argc, char **argv)
{
	int new_monit_time=0;
	if( argc <2){
		printf("Not enough arguments to mpt\n");
		return;
	}

	sscanf(argv[1], "%d", &new_monit_time);

	setPerTransferencia((char) new_monit_time);
	return;
}

/*-------------------------------------------------------------------------+
| Function: cmd_lar - listar alarmes de relógio entre t1 e t2
+--------------------------------------------------------------------------*/ 
void cmd_lar (int argc, char **argv)
{
	int h1=0,m1=0,s1=0,h2=0,m2=0,s2=0;

	if(argc < 7){
		if( argc < 4){
			h1=25;
			m1=61;
			s1=61;
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}else{
			sscanf(argv[1], "%d",&h1);
			sscanf(argv[2], "%d",&m1);
			sscanf(argv[3], "%d",&s1);
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}
	}else{
		sscanf(argv[1], "%d", &h1);
		sscanf(argv[2], "%d", &m1);
		sscanf(argv[3], "%d", &s1);
		sscanf(argv[4], "%d", &h2);
		sscanf(argv[5], "%d", &m2);
		sscanf(argv[6], "%d", &s2);
	}

	getClockAlarms(h1,m1,s1,h2,m2,s2);
}

/*-------------------------------------------------------------------------+
| Function: cmd_lat - listar alarmes de temperatura entre t1 e t2
+--------------------------------------------------------------------------*/ 
void cmd_lat (int argc, char **argv)
{
	int h1=0,m1=0,s1=0,h2=0,m2=0,s2=0;

	if(argc < 7){
		if( argc < 4){
			h1=25;
			m1=61;
			s1=61;
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}else{
			sscanf(argv[1], "%d", &h1);
			sscanf(argv[2], "%d", &m1);
			sscanf(argv[3], "%d", &s1);
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}
	}else{
		sscanf(argv[1], "%d", &h1);
		sscanf(argv[2], "%d", &m1);
		sscanf(argv[3], "%d", &s1);
		sscanf(argv[4], "%d", &h2);
		sscanf(argv[5], "%d", &m2);
		sscanf(argv[6], "%d", &s2);
	}

	getTempeAlarms(h1,m1,s1,h2,m2,s2);
}

/*-------------------------------------------------------------------------+
| Function: cmd_lal - listar alarmes de luminosidade entre t1 e t2
+--------------------------------------------------------------------------*/ 
void cmd_lal (int argc, char **argv)
{
	int h1=0,m1=0,s1=0,h2=0,m2=0,s2=0;

	if(argc < 7){		// no caso de não terem sido passados argumentos suficientes
		if( argc < 4){	// queremos todos até ao fim, dai substituir pelos valores de limite de t1 e t2
			h1=0;
			m1=0;
			s1=0;
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}else{
			sscanf(argv[1], "%d", &h1);
			sscanf(argv[2], "%d", &m1);
			sscanf(argv[3], "%d", &s1);
			h2 = 25;
			m2 = 61;
			s2 = 61;
		}
	}else{
		sscanf(argv[1], "%d", &h1);
		sscanf(argv[2], "%d", &m1);
		sscanf(argv[3], "%d", &s1);
		sscanf(argv[4], "%d", &h2);
		sscanf(argv[5], "%d", &m2);
		sscanf(argv[6], "%d", &s2);
	}

	getLumusAlarms(h1,m1,s1,h2,m2,s2);
}

/*-------------------------------------------------------------------------+
| Function: cmd_iga - informação geral de alarmes (activacao e desativacao)
+--------------------------------------------------------------------------*/ 
void cmd_iga (int argc, char **argv)
{
	infoGestaoAlarms();
}

/*-------------------------------------------------------------------------+
| Function: cmd_ig - informacao geral (inicio, relogio, memoria, periodo de monit)
+--------------------------------------------------------------------------*/ 
void cmd_ig (int argc, char **argv)
{

	return;
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
  unsigned int n=0, i;
  unsigned char bufw[50];
  cyg_mutex_lock(&lock_write);
  if ((n=argc) > 1) {
    n--;
    if (n > 50) n = 50;
    for (i=0; i<n; i++)
      //      sscanf(argv[i+1], "%hhx", &bufw[i]);
    {
    	unsigned int x; 
    	sscanf(argv[i+1], "%x", &x);
    	bufw[i]=(unsigned char)x;
    }
    err = cyg_io_write(serH, bufw, &n);
    
    printf("io_write err=%x, n=%d\n", err, n);
    for (i=0; i<n; i++)
    {
      printf("buf[%d]=%x\n", i, bufw[i]);

		unsigned char copia = bufw[i];
		while (copia) {
		    if (copia & 1)
		        printf("1");
		    else
		        printf("0");
		
		    copia >>= 1;
		}
		printf("\n");
    }
  }
  else {
    printf("nenhuma mensagem!!!\n");
  }
  cyg_mutex_unlock(&lock_write);
}

/*-------------------------------------------------------------------------+
| Function: cmd_rms - receber mensagem (string)
+--------------------------------------------------------------------------*/ 
void cmd_rms (int argc, char **argv)
{
  unsigned int n=0;
  char bufr[50];

  if (argc > 1) n = atoi(argv[1]);
  if (n > 50) n = 50;
  err = cyg_io_read(serH, bufr, &n);
  printf("io_read err=%x, n=%d buf=%s\n", err, n, bufr);
}

/*-------------------------------------------------------------------------+
| Function: cmd_rmh - receber mensagem (hexadecimal)
+--------------------------------------------------------------------------*/ 
char* cmd_rmh (int a)
{
  unsigned int n=0, i;
  unsigned char *bufr = (unsigned char*) malloc(sizeof(unsigned char)*50);

  if (a > 50) n = 50;
  else n = a;
  err = cyg_io_read(serH, bufr, &n);
  printf("io_read err=%x, n=%d\n", err, n);
  
  for (i=0; i<n; i++)
    printf("buf[%d]=%x\n", i, bufr[i]);

  return bufr;
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

