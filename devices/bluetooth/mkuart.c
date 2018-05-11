/*
 * mkuart.c
 *
 *  Created on: 2010-09-04
 *       Autor: Miros≈Çaw Karda≈õ
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#include "mkuart.h"


volatile char UART_RxBuf[UART_RX_BUF_SIZE];
volatile uint8_t UART_RxHead;
volatile uint8_t UART_RxTail;
volatile char UART_TxBuf[UART_TX_BUF_SIZE];
volatile uint8_t UART_TxHead;
volatile uint8_t UART_TxTail;
volatile bool UART_FirstEndFlag;





/*! configure registers
 *  @param		baud speed of transmission*/
void USART_Init( uint16_t baud ) {
	/* set baud */
	UBRR0H = (uint8_t)(baud>>8);
	UBRR0L = (uint8_t)baud;
	/* turn on transmitter and receiver */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* set frame format: 8bits of data, 1 stop bit*/
	UCSR0C = (3<<UCSZ00);

	// when using RS485
	#ifdef UART_DE_PORT
		// init control line of transmitter
		UART_DE_DIR |= UART_DE_BIT;
		UART_DE_ODBIERANIE();
	#endif

	// when using RS485 RS485
	#ifdef UART_DE_PORT
		// unlock TXCIE interrupt
		UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
	#else
		// jeúli nie  korzystamy z interefejsu RS485
		UCSRB |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	#endif

	// reseting the buffers
	for (uint8_t i = 0; i < UART_RX_BUF_SIZE; i++)
		UART_RxBuf[i] = 0;
	for (uint8_t i = 0; i < UART_TX_BUF_SIZE; i++)
		UART_TxBuf[i] = 0;

	UART_RxHead = UART_RxTail = UART_TxHead = UART_TxTail = 0;
} // END void USART_Init

/*! @return 		value in buffer, 0 when empty buffer */
char uart_getc() {
    // sprawdzamy czy indeksy sπ rÛwne
    if (!IsUnreadData()) {
    	return 0;
    }

    // increment new tail intex
    UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
    // return byte from new tail position
    return UART_RxBuf[UART_RxTail];
} // END char uart_getc


/*! @param 		data byte to send*/
void uart_putc( char data ) {
	uint8_t tmp_head;

    tmp_head  = (UART_TxHead + 1) & UART_TX_BUF_MASK;

    // wait when write buffer is full
    while ( tmp_head == UART_TxTail ){}

    UART_TxBuf[tmp_head] = data;
    UART_TxHead = tmp_head;

    // init empty buffer interrupt to sending data
    UCSR0B |= (1<<UDRIE0);
} // END void uart_putc

/*! @param 		s byte of table pointer*/
void uart_puts(char *s)	{
  register char c;
  while ((c = *s++)) uart_putc(c);			// send when c not empty
} // END void uart_puts

/*! @param 		progmem_s byte of PROGMEM table pointer*/
void uart_puts_p(const char *progmem_s ) {
    register char c;
    while ( (c = pgm_read_byte(progmem_s++)) )
      uart_putc(c);

}/* uart1_puts_p */

/*! @param 		value number to send
 *  @param 		radix base numeral system of value*/
void uart_putint(int value, int radix) {
	char string[UART_TX_BUF_SIZE + 1];	// temp buffer
	itoa(value, string, radix);
	uart_puts(string);
} // END void uart_putint

/*!  Tx Complete interrupt, when UDR delayed
 *  compile when RS485 enabled*/
#ifdef UART_DE_PORT
ISR( USART_TX_vect ) {
  UART_DE_PORT &= ~UART_DE_BIT;	// block RS485 transmitter
}
#endif

//! transmitting interrupt, read and send data from cyclic write buffer
ISR( USART_UDRE_vect)  {
    if ( UART_TxHead != UART_TxTail ) {
    	UART_TxTail = (UART_TxTail + 1) & UART_TX_BUF_MASK;
    	// write byte to hardware buffer
    	UDR0 = UART_TxBuf[UART_TxTail];
    } else {
    // clear interrupt flag when empty buffer
	UCSR0B &= ~(1<<UDRIE0);
    }
} // END ISR( USART_UDRE_vect)

//! receiving interrupt, write data to cyclic read buffer
ISR( USART_RX_vect ) {
    uint8_t tmp_head;
    char data;

    data = UDR0; // read byte from hardware buffer
    if (data == END_FRAME_CODE)
    	UART_FirstEndFlag = true;
    tmp_head = ( UART_RxHead + 1) & UART_RX_BUF_MASK;

    // overflow buffer test, when cyclic read buffer is full
    if ( tmp_head == UART_RxTail ) {
    	UART_RxBuf[UART_RxHead] = END_FRAME_CODE;
    	uart_putc(END_FRAME_CODE);
    } else {
    	UART_RxHead = tmp_head;
    	UART_RxBuf[tmp_head] = data;
    }
} // END ISR( USART_RX_vect )

