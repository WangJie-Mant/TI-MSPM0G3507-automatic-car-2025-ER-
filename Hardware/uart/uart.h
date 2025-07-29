#ifndef __UART_H__
#define __UART_H__

void uart_sendChar(char ch);
void uart_sendString(char* str);

extern volatile unsigned char uart_data;

#endif
