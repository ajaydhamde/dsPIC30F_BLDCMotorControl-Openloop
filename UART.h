#ifndef __UART_H__
#define __UART_H__
#include <stdint.h>
#include <xc.h>
#include "peripheral.h"
#define BAUDRATE 9600

void UART_Init();
void UART_Transmit_char(char );
void UART_Transmit_Buffer(char *,uint8_t );
char UART_Recieve_char();

#endif      //__UART_H__