/*!
 * @file mkuart.h
 * @author 		Miros³aw Kardaœ <br>
 * 				http://mirekk36.blogspot.com/
 * 				https://sklep.atnel.pl/
 * $Modified: 2018-04-09 $
 * $Created: 2010-09-04 $
 * @version 2.0
 *
 * Kod bazowy stworzony przez Miros³awa Kardasia zawierajacy obsluge UART.
 * Code writed by Miros³aw Kardaœ containing UART staff.
 */

#include "../../group_includes.h"

#ifndef MKUART_H_
#define MKUART_H_

/*
 *
 *		Macros
 *
 */
//! UART speed [bit]
#define UART_BAUD 9600
//! UART prescaler value
#define __UBRR F_CPU/16/UART_BAUD-1

//! RS485 state register
#define UART_DE_PORT PORTD
//! RS485 direction register
#define UART_DE_DIR DDRD
//! RS485 address
#define UART_DE_BIT (1<<PD2)

//! setting DE logic state to receive
#define UART_DE_ODBIERANIE()  UART_DE_PORT |= UART_DE_BIT
//! setting DE logic state to send
#define UART_DE_NADAWANIE()  UART_DE_PORT &= ~UART_DE_BIT

//! size of read buffer
#define UART_RX_BUF_SIZE 64
//! mask of read buffer
#define UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1)
//! size of write buffer
#define UART_TX_BUF_SIZE 32
//! mask of write buffer
#define UART_TX_BUF_MASK ( UART_TX_BUF_SIZE - 1)

//! cyclic read buffer
extern volatile char UART_RxBuf[UART_RX_BUF_SIZE];
//! begin data of read buffer
extern volatile uint8_t UART_RxHead;
//! end data of read buffer
extern volatile uint8_t UART_RxTail;
//! cyclic write buffer
extern volatile char UART_TxBuf[UART_TX_BUF_SIZE];
//! begin data of write buffer
extern volatile uint8_t UART_TxHead;
//! end data of write buffer
extern volatile uint8_t UART_TxTail;
//! first end byte flag
extern volatile bool UART_FirstEndFlag;
//! end frame code
#define END_FRAME_CODE '$'


/*
 *
 *		Functions declarations
 *
 */
//! initialize UART
void USART_Init( uint16_t baud );
//! checks if they is unreaded data
inline bool IsUnreadData();
//! read one byte from cyclic read buffer
char uart_getc();
//! write one byte to cyclic write buffer
void uart_putc( char data );
//! send text
void uart_puts(char *s);
//! send text from PROGMEM
void uart_puts_p(const char *progmem_s ) ;
//! send number which one numeral system
void uart_putint(int value, int radix);

/*
 *
 *		Definitions of inline functions
 *
 */

/*! @return		true when are unreaded data in cyclic read buffer, false otherwise*/
inline bool IsUnreadData() {
	if (UART_RxHead != UART_RxTail) return true;
	else return false;
}

#endif /* MKUART_H_ */
