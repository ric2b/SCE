#ifndef USART
#define USART

void USARTinit (void);
void serialWrite(char c[],char n);
void serialWriteChar(char c);
void processMessage(char inBuffer[], char size);

#endif
