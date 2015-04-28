#ifndef EEPROMINT_H
#define EEPROMINT_H

void EEPROMintDisInt(void);
void EEPROMintErrClear(void);
char EEPROMintRead(char addr);
void EEPROMintWrite(char addr, char data);
void EEPROMintUpdateW(void);
void EEPROMintUpdateR(void);

#endif
