/*!
 * @file relay.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-04 $
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
#define RELAY_DDR DDRC
//! rejestr stanu przekaznika
#define RELAY_PORT PORTC
//! adres przekaznika
#define RELAY_ADDR (1 << PC3)
//! zmienia stan przekaznika na przeciwny
#define RELAY_CH() RELAY_PORT ^= RELAY_ADDR
//! sprawdza czy zlacze przekaznika aktywne
#define RELAY_IS_ON() (RELAY_PORT & RELAY_ADDR)

// stale czasowe
//! suma mnoznikow  LOW i HIGH, mnozniki zawarte
//! @see		 RELAY_LONG_HIGH_TIME
//! @see		 RELAY_LONG_LOW_TIME
#define RELAY_MUL_SUM 10

//! czas jednego taktu przekaznika liczony w ms
#define RELAY_INTERVAL_MS 1500

//! czas czesci taktu przekaznika liczony w ms
#define RELAY_STATE_MS (RELAY_INTERVAL_MS / RELAY_MUL_SUM)

#define RELAY_LONG_HIGH_TIME RELAY_STATE_MS * 5
#define RELAY_LONG_LOW_TIME RELAY_STATE_MS * 5
#define RELAY_SHORT_HIGH_TIME RELAY_STATE_MS * 1
#define RELAY_SHORT_LOW_TIME RELAY_STATE_MS * 9

//! czas dla zmiany sekwencji startowej gdy aktualnie stan wysoki liczony w ms dla minuty i godziny
//! (drganie pzekaznika)
#define RELAY_HIGH_START_MS 75
//! czas dla zmiany sekwencji testwej gdy aktualnie stan wysoki liczony w ms dla liczby
#define RELAY_HIGH_START_MS_NUMBER 35

//! ilosc zmian stanu przekaznika gdy aktualnie stan wysoki w trybie godzin, najlepiej wartosc parzysta
//! aby ostatni stan wysoki nie zostal wygaszony przez przerwe
// w przypadku zmiany nalezy skonfigurowac
#define RELAY_HIGH_START_H_COUNT 12

//! ilosc zmian stanu przekaznika gdy aktualnie stan wysoki w trybie minut
#define RELAY_HIGH_START_M_COUNT 8

//! ilosc zmian stanu przekaznika gdy aktualnie stan wysoki w trybie liczb
#define RELAY_HIGH_START_N_COUNT 12

//! sumaryczny czas przerwy miedzy sekwencja startu w stanie wysokim a wlasciwym kodowaniem czasu liczony w ms
#define RELAY_LOW_START_MS 1500

/*
 *
 *		Glowny typ danych
 *
 */

//! rodzaje typow danych wprowadzanych to przekaznika
typedef enum {
	RelayDataHours = 0,
	RelayDataMinutes,
	RelayDataNumber
} RelayDataType;



//! glowna struktura przekaznika
typedef struct {
	//! aktualny czas w ms, gdy 0 zmiana stanu i zaladowanie z ui16tTimeMSToClick
	/*! @see		ui16TimeMsToClick*/
	uint16_t ui16ActTimeMS;
	//! binarna informacja przekazywana za pomoca przekaznika
	uint8_t uiByteInfo;
	//! dlugosc binarnej informacji
	uint8_t uiByteLength;
	//! dlugosc startowej informacji
	uint8_t uiStartLength;
	//! informacja czy kodowanie minut, jesli nie to kodowanie godzin
	RelayDataType dataType;
	//! czas dla zmiany sekwencji startowej gdy aktualnie stan wysoki, drganie przekaznika
	uint8_t uiHighStartTimeMS;
	//! stan pracy przekaznika, tylko do odczytu
	//! @see SetRelayState
	BinarySwitch eState;
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
extern void RelayInit(volatile Relay *r);
//! zaladowanie potrzebnych danych do klikania w strukturze
extern void RelayStartClicking(volatile Relay *relay, uint8_t uiByteInfo, RelayDataType dataType);
//! proba klikania wg danych struktury, wyzwalana co ms
extern void RelayTryClickMS(volatile Relay *relay);
//! zmiana stanu pracy przekaznika
extern void SetRelayState(volatile Relay *relay, BinarySwitch eState);

#endif /* DEVICES_RELAY_H_ */
