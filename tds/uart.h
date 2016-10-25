#ifndef _UART_H
#define _UART_H

#include "list.h"

#define COM1_IRQ 4
#define COM2_IRQ 3

#define COM1_BASE 0x3F8
#define COM2_BASE 0x2F8

#define BIT(n) (0x01<<(n))

#define RBR 0 // DLAB 0
#define THR 0 // DLAB 0
#define IER 1 // DLAB 0
#define IIR 2 // DLAB X
#define FCR 2 // DLAB X
#define LCR 3 // DLAB X
#define MCR 4 // DLAB X
#define LSR 5 // DLAB X
#define MSR 6
#define SR 7

#define FIFO_ENABLE_BIT 0
#define FIFO_CR_BIT 1
#define FIFO_CT_BIT 2
#define FIFO_64BITS 5
#define FIFO_INT_TRIGGER_BIT 6


#define FIFO_TRIGGER_1 0
#define FIFO_TRIGGER_4 1
#define FIFO_TRIGGER_8 2
#define FIFO_TRIGGER_14 3
#define FIFO_DISABLED 0
#define FIFO_UNUSABLE 2
#define FIFO_ENABLED 3


#define INT_MODEM 0
#define INT_TRANS_EMPTY 1
#define INT_DATA_AVAILABLE 2
#define INT_LINE_STATUS 3
#define INT_TIMEOUT 4

#define UART_BUFFER_SEND 0
#define UART_BUFFER_RECEIVE 1

#define UART_DATA 0

#define DLA_BIT 7

#define DLL 0 // DLAB 1
#define DLM 1 // DLAB 1

void UART_ClearBuffer(char bufferID);

int UART_SetConfiguration(unsigned int addr,unsigned char word_length, unsigned char stop_bits, unsigned char parity,unsigned short bitrate);

int UART_SubscribeInt(unsigned int addr, unsigned int port);

int UART_UnSubscribeInt();

List* UART_ReturnBuffer();

void UART_DisableFIFO(unsigned int addr);

void UART_EnableFIFO(unsigned int addr);

char UART_NeedToSend();

int UART_SendChar(unsigned int addr, char ch);

int UART_SendMsg(char* msg);

inline char UART_IntHandler(unsigned int addr);

char UART_ReceiveChar(unsigned int addr);

void UART_SetDLAB(unsigned int addr,unsigned char status);

#endif
