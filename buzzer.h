#ifndef _BUZZER_H
#define _BUZZER_H

#define F_BUZZ	244	// buzzer base frequency. lowest achievable frequency is 244 Hz
#define F_BUZZ_INC	25
#define BUZZ_DUTY 0.5

void buzzSetup();
void buzzOpen(unsigned freq);
void buzzKill();
void buzzTest();

#endif
