/*!
 * @file mkuart.h
 * @author 		Miros³aw Kardaœ <br>
 * 				http://mirekk36.blogspot.com/
 * 				https://sklep.atnel.pl/
 * $Modified: 2018-03-31 $
 * $Created: 2010-09-04 $
 * @version 2.0
 *
 * Kod bazowy stworzony przez Miros³awa Kardasia zawierajacy obsluge UART.
 */

#include "../../group_includes.h"

#ifndef MKUART_H_
#define MKUART_H_

/*
 *
 *		Makroinstrukcje
 *
 */
//! predkosc UART w bitach
#define UART_BAUD 9600
//! wartosc preskalera UART
#define __UBRR F_CPU/16/UART_BAUD-1

//! rejestr stanu dla RS485
#define UART_DE_PORT PORTD
//! rejestr kierunku dla RS485
#define UART_DE_DIR DDRD
//! adres dla RS485
#define UART_DE_BIT (1<<PD2)

//! ustawienie stanu logicznego DE dla odbierania
#define UART_DE_ODBIERANIE()  UART_DE_PORT |= UART_DE_BIT
//! ustawienie stanu logicznego DE dla nadawania
#define UART_DE_NADAWANIE()  UART_DE_PORT &= ~UART_DE_BIT

//! rozmiar bufora odczytu
#define UART_RX_BUF_SIZE 32
//! maska bufora odczytu
#define UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1)
//! rozmiar bufora zapisu
#define UART_TX_BUF_SIZE 16
//! maska bufora zapisu
#define UART_TX_BUF_MASK ( UART_TX_BUF_SIZE - 1)

//! bufor cykliczny odczytu
extern volatile char UART_RxBuf[UART_RX_BUF_SIZE];
//! poczatek wszystkich danych bufora odczytu
extern volatile uint8_t UART_RxHead;
//! koniec wszystkich danych bufora odczytu
extern volatile uint8_t UART_RxTail;
//! bufor cykliczny zapisu
extern volatile char UART_TxBuf[UART_TX_BUF_SIZE];
//! poczatek wszystkich danych bufora zapisu
extern volatile uint8_t UART_TxHead;
//! poczatek wszystkich danych bufora odczytu
extern volatile uint8_t UART_TxTail;
//! flaga osiagniecia pierwszego bajtu konca
extern volatile bool UART_FirstEndFlag;
//! kod konca ramki
#define END_FRAME_CODE '$'


/*
 *
 *		Deklaracje funkcji / procedur
 *
 */
//! inicjalizacja UART
void USART_Init( uint16_t baud );
//! sprawdza czy sa jakies nieodczytane dane
inline bool IsUnreadData();
//! odczytanie pojedynczego znaku z bufora cyklicznego
char uart_getc();
//! wyslanie pojedynczego znaku
void uart_putc( char data );
//! wyslanie tekstu
void uart_puts(char *s);
//! wysylanie tekstu z pamieci programu
void uart_puts_p(const char *progmem_s ) ;
//! wyslanie liczby o danej podstawie jako tekstu
void uart_putint(int value, int radix);

/*
 *
 *		Definicje funkcji inline
 *
 */
/*! @return		true jesli w buforze cyklicznym odczytu sa jakies nieodczytane dane, false w przeciwnym wypadku*/
inline bool IsUnreadData() {
	if (UART_RxHead != UART_RxTail) return true;
	else return false;
}

#endif /* MKUART_H_ */
