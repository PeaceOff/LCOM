#ifndef _UART_H
#define _UART_H

#include "list.h"

/** @defgroup Uart Uart module
 * @{
 *
 * Module responsible for serial port
 *
 */

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

/**
 * @brief Function to clear a buffer
 *
 * @param bufferID the id of the buffer to be cleared
 *
 * @return void
 */
void UART_ClearBuffer(char bufferID);

/**
 * @brief Function to set the uart's configuration
 *
 * @param addr the address of the uart
 * @param word_lenght the lenght of the words
 * @param stop_bits the stop bits
 * @param parity the parity to be set
 * @param bitrate the bitrate to be set
 *
 * @return An int upon success or error
 */
int UART_SetConfiguration(unsigned int addr,unsigned char word_length, unsigned char stop_bits, unsigned char parity,unsigned short bitrate);

/**
 * @brief Function to subscribe the uart's interrupts
 *
 * @param addr the address of the uart
 * @param port Port to set the interrupts
 *
 * @return An int upon success or error
 */
int UART_SubscribeInt(unsigned int addr, unsigned int port);

/**
 * @brief Function to unsubscribe the uart's interrupts
 *
 * @return An int upon success or error
 */
int UART_UnSubscribeInt();

/**
 * @brief Function to return the uart's buffer
 *
 * @return Returns a list with the buffer
 */
List* UART_ReturnBuffer();

/**
 * @brief Function to disable the uart's FIFO
 *
 *@param addr Address to disable the FIFO
 *
 * @return void
 */
void UART_DisableFIFO(unsigned int addr);

/**
 * @brief Function to enable the uart's FIFO
 *
 *@param addr Address to disable the FIFO
 *
 * @return void
 */
void UART_EnableFIFO(unsigned int addr);

/**
 * @brief Function to check if the uart is waiting for a message
 *
 * @return Returns a char indicating true or false
 */
char UART_NeedToSend();

/**
 * @brief Function to send a char via the serial port
 *
 * @param addr The address to send the char
 * @param char Character to be sent
 *
 * @return Returns a char indicating success or error
 */
int UART_SendChar(unsigned int addr, char ch);

/**
 * @brief Function to send a string via the serial port
 *
 *Sends the characters individually
 *
 * @param msg String to be sent
 *
 * @return Returns a char indicating success or error
 */
int UART_SendMsg(char* msg);

/**
 * @brief Function to handle the uart's interrupts
 *
 * @param addr Address of the serial port
 *
 * @return Returns a char indicating success or error
 */
inline char UART_IntHandler(unsigned int addr);

/**
 * @brief Function to receive a char via the serial port
 *
 *Reads the char from the receiver FIFO's
 *
 * @param addr Address
 *
 * @return Returns a char indicating success or error
 */
char UART_ReceiveChar(unsigned int addr);

/**
 * @brief Function to set the DLAB bit of the Uart
 *
 * @param addr Address
 * @param status The status which the uart will be set to
 *
 * @return void
 */
void UART_SetDLAB(unsigned int addr,unsigned char status);

#endif
