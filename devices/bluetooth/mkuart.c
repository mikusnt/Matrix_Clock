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





/*! konfiguruje odpowiednie rejestry
 *  @param		baud predkosc transmisji*/
void USART_Init( uint16_t baud ) {
	/* Ustawienie prÍdkoúci */
	UBRR0H = (uint8_t)(baud>>8);
	UBRR0L = (uint8_t)baud;
	/* Za≥πczenie nadajnika I odbiornika */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Ustawienie format ramki: 8bitÛw danych, 1 bit stopu */
	UCSR0C = (3<<UCSZ00);

	// jeúli korzystamy z interefejsu RS485
	#ifdef UART_DE_PORT
		// inicjalizujemy liniÍ sterujπcπ nadajnikiem
		UART_DE_DIR |= UART_DE_BIT;
		UART_DE_ODBIERANIE();
	#endif

	// jeúli korzystamy z interefejsu RS485
	#ifdef UART_DE_PORT
		// jeúli korzystamy z interefejsu RS485 za≥πczamy dodatkowe przerwanie TXCIE
		UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
	#else
		// jeúli nie  korzystamy z interefejsu RS485
		UCSRB |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	#endif

	// resetowanie buforow
	for (uint8_t i = 0; i < UART_RX_BUF_SIZE; i++)
		UART_RxBuf[i] = 0;
	for (uint8_t i = 0; i < UART_TX_BUF_SIZE; i++)
		UART_TxBuf[i] = 0;

	UART_RxHead = UART_RxTail = UART_TxHead = UART_TxTail = 0;
} // END void USART_Init

/*! @return 		wartosc przechowana w buforze cyklicznym, gdy brak danych 0 */
char uart_getc() {
    // sprawdzamy czy indeksy sπ rÛwne
    if (!IsUnreadData()) {
    	return 0;
    }

    // obliczamy i zapamiÍtujemy nowy indeks Ñogona wÍøaî (moøe siÍ zrÛwnaÊ z g≥owπ)
    UART_RxTail = (UART_RxTail + 1) & UART_RX_BUF_MASK;
    // zwracamy bajt pobrany z bufora  jako rezultat funkcji
    return UART_RxBuf[UART_RxTail];
} // END char uart_getc


/*! @param 		data znak, ktory ma zostac wyslany*/
void uart_putc( char data ) {
	uint8_t tmp_head;

    tmp_head  = (UART_TxHead + 1) & UART_TX_BUF_MASK;

          // pÍtla oczekuje jeøeli brak miejsca w buforze cyklicznym na kolejne znaki
    while ( tmp_head == UART_TxTail ){}

    UART_TxBuf[tmp_head] = data;
    UART_TxHead = tmp_head;

    // inicjalizujemy przerwanie wystÍpujπce, gdy bufor jest pusty, dziÍki
    // czemu w dalszej czÍúci wysy≥aniem danych zajmie siÍ juø procedura
    // obs≥ugi przerwania
    UCSR0B |= (1<<UDRIE0);
} // END void uart_putc

/*! @param 		s adres tablicy znakowej*/
void uart_puts(char *s)		// wysy≥a ≥aÒcuch z pamiÍci RAM na UART
{
  register char c;
  while ((c = *s++)) uart_putc(c);			// dopÛki nie napotkasz 0 wysy≥aj znak
} // END void uart_puts

/*! @param 		progmem_s adres tablicy znakowej z pamieci programu*/
void uart_puts_p(const char *progmem_s )
{
    register char c;
    while ( (c = pgm_read_byte(progmem_s++)) )
      uart_putc(c);

}/* uart1_puts_p */

/*! @param 		value liczba ktora ma zostac wyslana
 *  @param 		radix podstawa systemu w jakim liczba ma zostac przeslana*/
void uart_putint(int value, int radix)	// wysy≥a na port szeregowy liczbe jako tekst o okreslonej podstawie
{
	char string[UART_TX_BUF_SIZE + 1];			// bufor na wynik funkcji itoa
	itoa(value, string, radix);		// konwersja value na ASCII
	uart_puts(string);			// wyúlij string na port szeregowy
} // END void uart_putint

/*! procedura obs≥ugi przerwania Tx Complete, gdy zostanie opÛøniony UDR
 *  kompilacja gdy uøywamy RS485*/
#ifdef UART_DE_PORT
ISR( USART_TX_vect ) {
  UART_DE_PORT &= ~UART_DE_BIT;	// zablokuj nadajnik RS485
}
#endif

//! przerwanie nadawcze pobierajace dane z bufora cyklicznego
ISR( USART_UDRE_vect)  {
    // sprawdzamy czy indeksy sπ rÛøne
    if ( UART_TxHead != UART_TxTail ) {
    	// obliczamy i zapamiÍtujemy nowy indeks ogona wÍøa (moøe siÍ zrÛwnaÊ z g≥owπ)
    	UART_TxTail = (UART_TxTail + 1) & UART_TX_BUF_MASK;
    	// zwracamy bajt pobrany z bufora  jako rezultat funkcji
    	UDR0 = UART_TxBuf[UART_TxTail];
    } else {
	// zerujemy flagÍ przerwania wystÍpujπcego gdy bufor pusty
	UCSR0B &= ~(1<<UDRIE0);
    }
} // END ISR

//! przerwanie odbiorcze zapisujace dane do bufora cyklicznego
ISR( USART_RX_vect ) {
    uint8_t tmp_head;
    char data;

    data = UDR0; //pobieramy natychmiast bajt danych z bufora sprzÍtowego
    if (data == END_FRAME_CODE)
    	UART_FirstEndFlag = true;
    // obliczamy nowy indeks Ñg≥owy wÍøaî
    tmp_head = ( UART_RxHead + 1) & UART_RX_BUF_MASK;

    // sprawdzamy, czy wπø nie zacznie zjadaÊ w≥asnego ogona
    if ( tmp_head == UART_RxTail ) {
    	// tutaj moøemy w jakiú wygodny dla nas sposÛb obs≥uøyÊ  b≥πd spowodowany
    	// prÛbπ nadpisania danych w buforze, mog≥oby dojúÊ do sytuacji gdzie
    	// nasz wπø zaczπ≥by zjadaÊ w≥asny ogon
    	// oznaczenie bajtu przed poczatkiem jako koniec
    	UART_RxBuf[UART_RxHead] = END_FRAME_CODE;
    	uart_putc(END_FRAME_CODE);
    } else {
    	UART_RxHead = tmp_head; 		// zapamiÍtujemy nowy indeks
    	UART_RxBuf[tmp_head] = data; 	// wpisujemy odebrany bajt do bufora
    }
} // END ISR

