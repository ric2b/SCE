#include "memory.h"
#include "comando.h"
#include "prog.h"
#include "monitor.h"

/* definir periodo de transferencia */

void setPerTransferencia(char a){
	periodo_transferencia = a;
	init_tick = cyg_current_time();
	return;
}

/* get periodo de transferencia, as outras funcoes nao tem acesso directo a esta informacao*/

char getPerTransferencia(void){
	return periodo_transferencia;
}

/* transferencia de registos periodicamente */

void getRegistersPer(void){
	last_tick = cyg_current_time(); // cada tick = 10ms
	char *buffer;

	if( ((last_tick - init_tick)*10/1000/60) < (int) periodo_transferencia ){
		// pedir 5 registos
		// trc 5
		buffer = (char*) malloc(sizeof(char)*6);

		memset(buffer, 0, 6);
		sprintf(buffer, "trc 5");
		putMSG(buffer, 1);
// FREES?
		init_tick = cyg_current_time();
	}
	return;
}



/* send a message to the board. limited to 98 bytes */
void sendMSGToBoard(char *msg){
	unsigned int i;
	char size = getSizeOfMSGsend(msg);
	
	if( size > 98){
		printf("String too big to safely send to board\n");
	}else{
		cyg_mutex_lock(&lock_write);
		printf("sending msg to board\n");
		cyg_mutex_unlock(&lock_write);
		
		unsigned char buffer[100];
		memset(buffer, 0, 100);
		
		buffer[0] = SOM;
		for(i=0; i< size; i++)
		{
			buffer[i+1] = (unsigned char)msg [i];
			cyg_mutex_lock(&lock_write);
			printf("%x - ", buffer[i+1]);

			unsigned char copia = buffer[i+1];
			while (copia) {
			    if (copia & 1)
			        printf("1");
			    else
			        printf("0");
			
			    copia >>= 1;
			}
			printf("\n");

			cyg_mutex_unlock(&lock_write);
		}
		buffer[size+1] = EOM;
		cyg_mutex_lock(&lock_write);
		printf("%x\n", buffer[size+1]);
		cyg_mutex_unlock(&lock_write);

		i=size+2; // # of unsent bytes
		//while(i != 0)
		{ /* keep trying to send until all bytes are sent */
			err = cyg_io_write(serH, buffer, &i);
		}
	}
	//return i;
}

/* get a message from the board, limited to 98 bytes + SOM and EOM */
char *getMSGfromBoard(void){
	char bufr[100];
	unsigned int n = 100;
	char *ret;
	err = cyg_io_read(serH, bufr, &n);
	
	ret = malloc(sizeof(char)*n);
	strcpy(ret, bufr);

	return ret;
	
}

/* get the size of a message, the buffer should include SOM and EOM */
char getSizeOfMSGrecv(char * msg){
	int i;
	//int size;
	
	if(msg[0] != (char)SOM)
		return 0;
	for(i=1; i<100; i++){
		if(msg[i] == (char)EOM)
		return i-1; // SOM and EOM aren't part of the message
	}
	return 0;
}

char getSizeOfMSGsend(unsigned char * msg){
	char i;
	for(i = 0; i < 100; i++){
		if(msg[(int)i] == (unsigned char)MSGEND)
			return i;
	}
	return 0;
}



void putMSG(char *buffer, int box){
	if(box == 0){					// PROCESSING THREAD
		cyg_mbox_put(mboxH1, buffer);
	}else if(box == 1){				// COMMUNICATION THREAD
		cyg_mbox_put(mboxH2, buffer);
	}
}


void interface(cyg_addrword_t data){
	cmd_ini(0, NULL);
	monitor();
	return;
}

void processing(cyg_addrword_t data){
	char *buffer;
	int argc;
	char *p;
	static char *argv[11];

	forever
	{
		getRegistersPer();
		buffer = cyg_mbox_get(mboxH1);

		cyg_mutex_lock(&lock_write);
		printf("[PROCESSING] Processing request %s\n", buffer);
		cyg_mutex_unlock(&lock_write);

		for (argc=0,p=buffer; (*buffer != '\0') && (argc < 11); p=NULL,argc++) 
		{
			p = strtok(p, " \t\n");
			argv[argc] = p;
			if (p == NULL) break;
		}
		argv[argc] = p;



		cyg_thread_yield();
	}
}

void communication_s(cyg_addrword_t data){
	char *buffer;
	forever{
		// After receiving a message we need to enclose it in a SOM & EOM
		buffer = cyg_mbox_get(mboxH2);

		cyg_mutex_lock(&lock_write);
		printf("[COMMUNICATION_S] Sending to board %s\n", buffer);
		cyg_mutex_unlock(&lock_write);

		sendMSGToBoard(buffer);

		cyg_thread_yield();

	}
}

void communication_r(cyg_addrword_t data){
	unsigned char buffer[50];
	unsigned int n;
	n=50;
	forever{
		err = cyg_io_read(serH, buffer, &n);

		cyg_mutex_lock(&lock_write);
		printf("[COMMUNICATION_S] Receiving from board %s\n", buffer);
		cyg_mutex_unlock(&lock_write);

		cyg_thread_yield();

	}
}

int main(void){

	periodo_transferencia=0;

	// initializing mutex's

	cyg_mutex_init(&lock_write);
	cyg_mutex_init(&lock_read);

	// creating message boxes for communication intraprocess

	cyg_mbox_create(&mboxH1, &mbox1);
	cyg_mbox_create(&mboxH2, &mbox2);

	// creating threads

	cyg_thread_create(PRIORITY_INTERFACE, interface, (cyg_addrword_t) NULL, NAME_INTERFACE, (void *)stack[0], STACKSIZE, &t_interface, &thread_s[0]);
	cyg_thread_create(PRIORITY_PROCESSING, processing, (cyg_addrword_t) NULL, NAME_PROCESSING, (void *)stack[1], STACKSIZE, &t_processing, &thread_s[1]);
	cyg_thread_create(PRIORITY_COMMUNICATION, communication_s, (cyg_addrword_t) NULL, NAME_COMMUNICATION_S, (void *)stack[2], STACKSIZE, &t_communication_s, &thread_s[2]);
	cyg_thread_create(PRIORITY_COMMUNICATION, communication_r, (cyg_addrword_t) NULL, NAME_COMMUNICATION_R, (void *)stack[3], STACKSIZE, &t_communication_r, &thread_s[3]);

	// activating threads, the processing thread has to go first, otherwise inferface can takeover all the control
	
	cyg_thread_resume(t_processing);
	cyg_thread_resume(t_interface);
	cyg_thread_resume(t_communication_s);
	cyg_thread_resume(t_communication_r);

	puts("");
	exit(0); // isto confunde-me bastante, mas se funciona funciona xD
}
