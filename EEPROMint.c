#include <p18f452.h>  /* for the special function register declarations */

void EEPROMintDisInt(void)
{
	PIE2bits.EEIE = 0;	// Data EEPROM/FLASH Write Operation Interrupt Enable bit
}

void EEPROMintErrClear(void)
{
	EECON1bits.WRERR = 0;
}

char EEPROMintRead(char addr)
{
	EEADR = addr;           // Data Memory Address to read
	EECON1bits.EEPGD = 0;   // Point to DATA memory
	EECON1bits.CFGS = 0;    // Access program FLASH or Data EEPROM memory
	EECON1bits.RD = 1;      // EEPROM Read
	return EEDATA;          // EEDATA now has the data

//	MOVLW DATA_EE_ADDR    ;
//	MOVWF EEADR           ; Data Memory Address to read
//	BCF EECON1, EEPGD     ; Point to DATA memory
//	BCF EECON1, CFGS      ; Access program FLASH or Data EEPROM memory
//	BSF EECON1, RD        ; EEPROM Read
//	MOVF EEDATA, W        ; W = EEDATA

}

void EEPROMintWrite(char addr, char data)
{
	EEADR = addr;	// address where we want to write to
	EEDATA = data;	// what to write

	EECON1bits.EEPGD = 0;   // Point to DATA memory
	EECON1bits.CFGS = 0;    // Access program FLASH or Data EEPROM memory
	EECON1bits.WREN = 1;    // Enable writes

	INTCONbits.GIE = 0;     // Disable interrupts
	EECON2 = 0x55;          // write 55h
	EECON2 = 0xAA;          // write AAh
	EECON1bits.WR = 1;		// set WR bit to begin write
	INTCONbits.GIE = 1;		// Enable interrupts

	// BCF clears a flag. BSF sets a flag
//	_asm
//	BCF EECON1, EEPGD  ; Point to DATA memory
//	BCF EECON1, CFGS   ; Access program FLASH or Data EEPROM memory
//	BSF EECON1, WREN   ; Enable writes
//
//	BCF INTCON, GIE    ; Disable interrupts
//	MOVLW 55h          ;
//	MOVWF EECON2       ; Write 55h
//	MOVLW AAh          ;
//	MOVWF EECON2       ; Write AAh
//	BSF EECON1, WR     ; Set WR bit to begin write
//	BSF INTCON, GIE    ; Enable interrupts
//	_endasm
}

void EEPROMintTest(char c)
{
	char cenas;
	EEPROMintWrite(0x00, c);
	while(EECON1bits.WR);	// wait for previous write to end
	cenas = EEPROMintRead(0x00);
	EEPROMintWrite(0x07, cenas);
}

