#ifndef EEPROMINT_H
#define EEPROMINT_H

void EEPROMintDisInt(void);
void EEPROMintErrClear(void);
void EEPROMintWrite(char addr, char data);
char EEPROMintRead(char addr);

#endif
