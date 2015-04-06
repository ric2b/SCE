#include <p18f452.h>
#include <pwm.h>
#include <timers.h>
#include "main.h"
#include "buzzer.h"
#include "delays.h"

#define TMR2_PRES	16 // timer2 prescaller. used to calculate PWM frequency. prescaller must be changed in OpenTimer2 too
#define F_BUZZ	244	// buzzer base frequency. lowest achievable frequency is 244 Hz
#define F_BUZZ_INC	25
#define BUZZ_DUTY 0.5

void buzzSetup()
{
	OpenTimer2(TIMER_INT_OFF & T2_PS_1_16);	// open timer2, which controls PWM
	SetDCPWM1(BUZZ_DUTY*F_CPU*1000);	// 0.5 duty cycle
	TRISCbits.TRISC2 = 0;	// set RC2 (PWM1) as output
}

// open PWM with frequency freq
void buzzOpen(unsigned freq)
{
		// set oscillation frequency for the PWM
		OpenPWM1((char)(F_CPU*1000./(freq*4*TMR2_PRES)-1));
}

void buzzKill()
{
	ClosePWM1();
}

void buzzTest()
{
	unsigned f_buzz = F_BUZZ;
	int i;

	buzzSetup();

	for(i=0; i<9; i++)
	{
		buzzOpen(f_buzz + i*F_BUZZ_INC);
		delayms(2000);
	}

	buzzKill();
}
