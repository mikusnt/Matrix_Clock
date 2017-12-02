/*!
 * @file relay.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-12-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 1<br>
 * Plik naglowkowy obslugujacy przekaznik akustycznie przekazujacy dane binarne z wstapna sekwencja
 * startu.
 * Bit 1 jest reprezentowany przez pewna proporcje stanu wysokiego do niskiego.
 * Bit 0 posiada inna proporcje.
 * Przed sygnalem wlasciwym nadawana jest sekwencja startu - drganie przekaznika i przerwa.
 */

#ifndef DEVICES_RELAY_H_
#define DEVICES_RELAY_H_

#include "../group_includes.h"


/*
 *
 * 		makroinstrukcje
 *
 */

// IO
//! rejestr kierunku przekaznika
#define RELAY_DDR DDRD
//! rejestr stanu przekaznika
#define RELAY_PORT PORTD
//! adres przekaznika
#define RELAY_ADDR (1 << PD7)
//! zmienia stan przekaznika na przeciwny
#define RELAY_CH() RELAY_PORT ^= RELAY_ADDR
//! sprawdza czy zlacze przekaznika aktywne
#define RELAY_IS_ON() (RELAY_PORT & RELAY_ADDR)

// stale czasowe
//! mnoznik stalej czasowej informacji 1 gdy aktualnie stan wysoki
#define RELAY_LONG_HIGH_MUL 5
//! mnoznik stalej czasowej informacji 1 gdy aktualnie stan niski
#define RELAY_LONG_LOW_MUL 5
//! mnoznik stalej czasowej informacji 0 gdy aktualnie stan wysoki
#define RELAY_SHORT_HIGH_MUL 1
//! mnoznik stalej czasowej informacji 0 gdy aktualnie stan niski
#define RELAY_SHORT_LOW_MUL 9
//! suma mnoznikow  LOW i HIGH
#define RELAY_MUL_SUM 10

//! czas jednego taktu przekaznika liczony w ms
#define RELAY_INTERVAL_MS 1500

//! czas czesci taku przekaznika liczony w ms
#define RELAY_STATE_MS (RELAY_INTERVAL_MS / RELAY_MUL_SUM)

//! czas dla zmiany sekwencji startowej gdy aktualnie stan wysoki liczony w ms
//! (drganie pzekaznika)
#define RELAY_HIGH_START_MS 50

//! ilosc zmian stanu przekaznika gdy aktualnie stan wysoki, najlepiej wartosc parzysta
//! aby ostatni stan wysoki nie zostal wygaszony przez przerwe
#define RELAY_HIGH_START_COUNT 10

//! sumaryczny czas przerwy miedzy sekwencja startu w stanie wysokim a wlasciwym kodowaniem czasu liczony w ms
#define RELAY_LOW_START_MS 1000





//! pozycje w tablicy stalych czasowych stanow wysokiego i niskiego dla informacji 1 i 0
typedef enum {
	//! informacja 1 stan wysoki
	relayLongHPos, //!< relayLongHPos
	//! informacja 1 stan niski
	relayLongLPos, //!< relayLongLPos
	//! informacja 0 stan wysoki
	relayShortHPos,//!< relayShortHPos
	//! informacja 0 stan niski
	relayShortLPos //!< relayShortLPos
} RelayTime;


/*
 *
 *		Glowny typ danych
 *
 */
//! glowna struktura przekaznika
typedef struct {
	//! aktualny czas w ms, gdy 0 zmiana stanu i zaladowanie z ui16tTimeMSToClick
	/*! @see		ui16TimeMsToClick*/
	uint16_t ui16ActTimeMS;
	//! tablica stalych czasowych stanow wysokiego i niskiego
	/*! @see 		RelayTime*/
	uint16_t ui16tTimeMSToClick[4];
	//! binarna informacja przekazywana za pomoca przekaznika
	uint8_t uiByteInfo;
	//! dlugosc binarnej informacji
	uint8_t uiByteLength;
	//! dlugosc startowej informacji
	uint8_t uiStartLength;
} Relay;

/*
 *
 *		Deklaracje funkcji
 *
 */

// steruje wyjsciem cyfrowym przekaznika
//static inline void RelaySW(uint8_t uiValue);

// obraca bajt wejsciowy i zwraca jego dlugosc
//static inline uint8_t RoundByte(uint8_t byte, uint8_t *uiByteLength)

//! inicjalizacja przekaznika
extern void RelayInit();
//! zaladowanie potrzebnych danych do klikania w strukturze
extern void RelayStartClicking(volatile Relay *relay, uint8_t uiByteInfo, LogicState bIsMinutes);
//! proba klikania wg danych struktury, wyzwalana co ms
extern void RelayTryClickMS(volatile Relay *relay);

#endif /* DEVICES_RELAY_H_ */
