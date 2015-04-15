#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

void chooseInterrupt(void);
void EnableHighInterrupts (void);
void t1_isr (void);
void S3_isr (void);
void phoenix_isr(void);
void expelliarmus_isr(void);

#endif
