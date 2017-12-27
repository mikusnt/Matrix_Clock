/*
 * mkuart.c
 *
 *  Created on: 2010-09-04
 *       Autor: Mirosław Kardaś
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





/*! konfiguruje odpowiednie rejestry
 *  @param		baud predkosc transmisji*/
void USART_Init( uint16_t baud ) {
	/* Ustawienie pr�dko�ci */
	UBRR0H = (uint8_t)(baud>>8);
	UBRR0L = (uint8_t)baud;
	/* Za��czenie nadajnika I odbiornika */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Ustawienie format ramki: 8bit�w danych, 1 bit stopu */
	UCSR0C = (3<<UCSZ00);

	// je�li korzystamy z interefejsu RS485
	#ifdef UART_DE_PORT
		// inicjalizujemy lini� steruj�c� nadajnikiem
		UART_DE_DIR |= UART_DE_BIT;
		UART_DE_ODBIERANIE();
	#endif

	// je�li korzystamy z interefejsu RS485
	#ifdef UART_DE_PORT
		// je�li korzystamy z interefejsu RS485 za��czamy dodatkowe przerwanie TXCIE
		UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
	#else
		// je�li nie  korzystamy z interefejsu RS485
		UCSRB |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	#endif
} // END void USART_Init

/*! @return 		wartosc przechowana w buforze cyklicznym, gdy brak danych 0 */
char uart_getc() {
    // sprawdzamy czy indeksy s� r�wne
    if ( UART_RxHead == UART_RxTail ) {
    	return 0;
    }

    // obliczamy i zapami�tujemy nowy indeks �ogona w�a� (mo�e si� zr�wna� z g�ow�)
    UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji
    return UART_RxBuf[UART_RxTail];
} // END char uart_getc


/*! @param 		data znak, ktory ma zostac wyslany*/
void uart_putc( char data ) {
	uint8_t tmp_head;

    tmp_head  = (UART_TxHead + 1) & UART_TX_BUF_MASK;

          // p�tla oczekuje je�eli brak miejsca w buforze cyklicznym na kolejne znaki
    while ( tmp_head == UART_TxTail ){}

    UART_TxBuf[tmp_head] = data;
    UART_TxHead = tmp_head;

    // inicjalizujemy przerwanie wyst�puj�ce, gdy bufor jest pusty, dzi�ki
    // czemu w dalszej cz�ci wysy�aniem danych zajmie si� ju� procedura
    // obs�ugi przerwania
    UCSR0B |= (1<<UDRIE0);
} // END void uart_putc

/*! @param 		s adres tablicy znakowej*/
void uart_puts(char *s)		// wysy�a �a�cuch z pami�ci RAM na UART
{
  register char c;
  while ((c = *s++)) uart_putc(c);			// dop�ki nie napotkasz 0 wysy�aj znak
} // END void uart_puts

/*! @param 		value liczba ktora ma zostac wyslana
 *  @param 		radix podstawa systemu w jakim liczba ma zostac przeslana*/
void uart_putint(int value, int radix)	// wysy�a na port szeregowy liczbe jako tekst o okreslonej podstawie
{
	char string[UART_TX_BUF_SIZE + 1];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts(string);			// wy�lij string na port szeregowy
} // END void uart_putint

/*! procedura obs�ugi przerwania Tx Complete, gdy zostanie op�niony UDR
 *  kompilacja gdy u�ywamy RS485*/
#ifdef UART_DE_PORT
ISR( USART_TX_vect ) {
  UART_DE_PORT &= ~UART_DE_BIT;	// zablokuj nadajnik RS485
}
#endif

//! przerwanie nadawcze pobierajace dane z bufora cyklicznego
ISR( USART_UDRE_vect)  {
    // sprawdzamy czy indeksy s� r�ne
    if ( UART_TxHead != UART_TxTail ) {
    	// obliczamy i zapami�tujemy nowy indeks ogona w�a (mo�e si� zr�wna� z g�ow�)
    	UART_TxTail = (UART_TxTail + 1) & UART_TX_BUF_MASK;
    	// zwracamy bajt pobrany z bufora  jako rezultat funkcji
    	UDR0 = UART_TxBuf[UART_TxTail];
    } else {
	// zerujemy flag� przerwania wyst�puj�cego gdy bufor pusty
	UCSR0B &= ~(1<<UDRIE0);
    }
} // END ISR

//! przerwanie odbiorcze zapisujace dane do bufora cyklicznego
ISR( USART_RX_vect ) {
    uint8_t tmp_head;
    char data;

    data = UDR0; //pobieramy natychmiast bajt danych z bufora sprz�towego

    // obliczamy nowy indeks �g�owy w�a�
    tmp_head = ( UART_RxHead + 1) & UART_RX_BUF_MASK;

    // sprawdzamy, czy w�� nie zacznie zjada� w�asnego ogona
    if ( tmp_head == UART_RxTail ) {
    	// tutaj mo�emy w jaki� wygodny dla nas spos�b obs�u�y�  b��d spowodowany
    	// pr�b� nadpisania danych w buforze, mog�oby doj�� do sytuacji gdzie
    	// nasz w�� zacz��by zjada� w�asny ogon
    } else {
    	UART_RxHead = tmp_head; 		// zapami�tujemy nowy indeks
    	UART_RxBuf[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
    }
} // END ISR
