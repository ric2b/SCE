#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

void chooseInterrupt(void);
void EnableHighInterrupts (void);
void t1_isr (void);
void S3_isr (void);
void LVD_isr(void);
void EEwriteDisable_isr(void);
void DisableHighInterrupts(void);
#endif
