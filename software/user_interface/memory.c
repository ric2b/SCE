#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// initiates the lock for circular buff

void ini_circularbuf(void){
	cyg_mutex_init(&lock_circularbuf);
}

// clear all entries of the circularbuf

void clear_circularbuf(void){
	int i;
	cyg_mutex_lock(&lock_circularbuf);
	for(i=0; i< NRBUF; i++){
		memset(shared_mem.data[i], 0,8);
	}
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

// read the next event on memory

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

// read n registers, starting from rPointer position

char **read_n_regs(int n){
	char **returnString = (char**)malloc(sizeof(char)*n);
	int i;
	for(i =0; i< n; i++){
		returnString[i] = read_from_mem();
	}
	return returnString;
}

// read n registers, starting from index i

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

// List all clock alarms between t1 and t2

void getClockAlarms(int h1, int m1, int s1, int h2, int m2, int s2){
	int i;
	if(regWritten == NRBUF-1){								// if we have all the memory full, we can go up to NRBUF
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][3] == CLOCK_ALARM )		// check for code, has to be a clock alarm
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}else{
		for(i=0; i <= regWritten; i++){						// if we dont have all the memory full, we can only read up to regWritten
			if( shared_mem.data[i][3] == CLOCK_ALARM )		// check for code, has to be a clock alarm
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}
}

// List all temperature alarms between t1 and t2

void getTempeAlarms(int h1, int m1, int s1, int h2, int m2, int s2){
	int i;
	if(regWritten == NRBUF-1){
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][3] == TEMPE_ALARM )		// same as clock alarm, but different code
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}else{
		for(i=0; i <= regWritten; i++){
			if( shared_mem.data[i][3] == TEMPE_ALARM )
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}
}

// List all lumus alarms between t1 and t2

void getLumusAlarms(int h1, int m1, int s1, int h2, int m2, int s2){
	int i;
	if(regWritten == NRBUF-1){
		for(i=0; i < NRBUF; i++){
			if( shared_mem.data[i][3] == LUMUS_ALARM )		// same as alarm clock but different code
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}else{
		for(i=0; i <= regWritten; i++){
			if( shared_mem.data[i][3] == LUMUS_ALARM )
			{
				if(shared_mem.data[i][0] < h2 && shared_mem.data[i][0] > h1) // if hour is between both requested times, OK!
				{
					printf("%s", shared_mem.data[i]);
				}
				else if(shared_mem.data[i][0] == h2)		// if not, then check for equal hours for upper bound
				{
					if(shared_mem.data[i][1] < m2)			// if h = h2, then we need to check for minutes, if m < m2, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m2)	// if not, then check for equal minutes for upper bound
					{
						if(shared_mem.data[i][2] <= s2)		// if m = m2, then s <= s2 for us to print
							printf("%s", shared_mem.data[i]);
					}
				}
				else if(shared_mem.data[i][0] == h1)		// if not, then check for equal hours for lower bound
				{
					if(shared_mem.data[i][1] > m1)			// if h = h1, then we need to check for minutes, if m > m1, OK!
					{
						printf("%s", shared_mem.data[i]);
					}
					else if(shared_mem.data[i][1] == m1)	// if not, then check for equal minutes for lower bound
					{
						if(shared_mem.data[i][2] >= s1)		// if m = m1, then s >= s1 for us to print
						{
							printf("%s", shared_mem.data[i]);
						}
					}
				}
			}
		}
	}
}

void getLocalReg(void){
	printf("NRBUF - %d | nr - %d | iescrita %d | ileitura %d", NRBUF, regWritten, wPointer, rPointer);
}

