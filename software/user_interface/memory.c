#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>

#define NRBUF 150
#define CLOCK_ALARM 7
#define TEMPE_ALARM 8
#define LUMUS_ALARM 9

typedef struct _circularbuf_{
	char data[NRBUF][8];
}circularbuf;

unsigned char wPointer=0;
unsigned char rPointer=0;
unsigned char regWritten=0;

circularbuf shared_mem;

cyg_mutex_t lock_circularbuf;

void ini_circularbuf(void){
	cyg_mutex_init(&lock_circularbuf);
}

void clear_circularbuf(void){
	cyg_mutex_lock(&lock_circularbuf);
	wPointer = 0;
	rPointer = 0;
	regWritten=0;
	cyg_mutex_unlock(&lock_circularbuf);
}

/* puts in circular buffer a event, caller need to make sure that the data he sends fits 8 bytes */
void put_in_mem(char to_store[8]){

	cyg_mutex_lock(&lock_circularbuf);
	memset(shared_mem.data[wPointer], 0, 8);
	strcpy(shared_mem.data[wPointer], to_store);

	wPointer++;
	if(wPointer == NRBUF)	//mem full
		wPointer = 0;
	else
		regWritten++;
	cyg_mutex_unlock(&lock_circularbuf);
}

char *read_from_mem(void){
	char *returnString = (char *)malloc(sizeof(char)*8);
	memset(returnString, 0,8);
	
	cyg_mutex_lock(&lock_circularbuf);
	strcpy(returnString, shared_mem.data[rPointer]);
	rPointer++;
	if(rPointer == NRBUF)
		rPointer = 0;
	cyg_mutex_unlock(&lock_circularbuf);
	
	return returnString;
}

char **read_n_regs(int n){
	char **returnString = (char**)malloc(sizeof(char)*n);
	int i;
	for(i =0; i< n; i++){
		returnString[i] = read_from_mem();
	}
	return returnString;
}

char **read_n_regs_from_i(int n, int i){
	char **returnString = (char**)malloc(sizeof(char)*n);
	int j;
	if( n+i > NRBUF){
		printf("Cannot return that many regs\n");
		return NULL;
	}
	
	for(j = i; j < n+i; j++){
		returnString[j] = (char *) malloc(sizeof(char)*8);
		memset(returnString[j],0,8);
		cyg_mutex_lock(&lock_circularbuf);
		strcpy(returnString[j], shared_mem.data[i+j]);
		cyg_mutex_unlock(&lock_circularbuf);
	}
	return returnString;
}


// to be changed to range [t1-t2]
void getClockAlarms(void){
	int i;
	if(regWritten == NRBUF-1){
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][0] == CLOCK_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}else{
		for(i=0; i <= regWritten; i++){
			if( shared_mem.data[i][0] == CLOCK_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}
}

// to be changed to range [t1-t2]
void getTempeAlarms(void){
	int i;
	if(regWritten == NRBUF-1){
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][0] == TEMPE_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}else{
		for(i=0; i <= regWritten; i++){
			if( shared_mem.data[i][0] == TEMPE_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}
}

// to be changed to range [t1-t2]
void getLumusAlarms(void){
	int i;
	if(regWritten == NRBUF-1){
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][0] == LUMUS_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}else{
		for(i=0; i <= regWritten; i++){
			if( shared_mem.data[i][0] == LUMUS_ALARM )
				printf("%s", shared_mem.data[i]);
		}
	}
}

void getLocalReg(void){
	printf("NRBUF - %d | nr - %d | iescrita %d | ileitura %d", NRBUF, regWritten, wPointer, rPointer);
}

