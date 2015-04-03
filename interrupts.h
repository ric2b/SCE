#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "main.h"

void EnableHighInterrupts (void);
void t1_isr (void);
void t3_isr (void);
void S3_isr (void);

#endif
