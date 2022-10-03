#include "UART.h"
#define BRGVAL ((FCY/BAUDRATE)/16)-1
void UART_Init()
{
    U1MODEbits.STSEL = 0;// 1-stop bit 
    U1MODEbits.PDSEL = 0;// No Parity, 8-data bits 
    U1MODEbits.ABAUD = 0;// Autobaud Disabled 
//    U1MODEbits.BRGH = 0;// Low Speed mode
    
    U1BRG = BRGVAL; // BAUD Rate Setting for 9600
    
    U1STAbits.URXISEL = 0; // Interrupt after a character is received
    IEC0bits.U1RXIE  =  1; // Enable UART Receive interrupt
    
    U1STAbits.UTXISEL = 0; // Interrupt for every data transfer
    IEC0bits.U1TXIE = 1; // Enable UART Transmit interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART Tx
}
void UART_Transmit_char(char c)
{
    U1TXREG = c; // Transmit one character
}
//UART Transmit ISR
//void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
//{
//    IFS0bits.U1TXIF = 0; // clear TX interrupt flag
//    U1TXREG = 'a'; // Transmit one character
//}
void UART_Transmit_Buffer(char *buff,uint8_t len)
{
    
}
char UART_Recieve_char()
{
    return 0;
}
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    uint8_t ReceivedChar;
    if(U1STAbits.OERR == 1) {
    U1STAbits.OERR = 0; // Clear Overrun Error to receive data
    } 
    else if((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0) ) {
    ReceivedChar = U1RXREG; // Read Data if there is no parity orframing
    // error
    
}
IFS0bits.U1RXIF = 0; // clear TX interrupt flag
}