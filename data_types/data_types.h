/*!
 * @file data_types.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-03-29 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Plik naglowkowy zawierajacy uniwersalne makra i typy danych.
 */

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#include "../group_includes.h"

/* Format zmiennych:
 * 		1. skrot typu
 * 		2. nazwa
 * 		3. parametr opcjonalny:
 * 			_e - edycja // domyslny przy wskaznikach
 * 			_s - set
 *
 *
 *
 */
/*
 *
 *		Makroinstrukcje
 *
 */

//! opoznienie wyrazane w sekundach
#define D_S(x) _delay_ms(x*1000)
//! opoznienie wyrazane w milisekundach [ms]
#define D_MS(x) _delay_ms(x)
//! opoznienie wyrazanie w mikrosekundach [us]
#define D_US(x) _delay_us(x)

/*
 *
 *		Uniwersalne yypy danych
 *
 */

//! osiem jednobitowych flag, natychmiastowy dostep do dowolnego bitu
/*! @see ByteFlag*/
typedef struct {
	uint8_t b0 : 1; 	//!< bit 0
	uint8_t b1 : 1; 	//!< bit 1
	uint8_t b2 : 1;		//!< bit 2
	uint8_t b3 : 1;		//!< bit 3
	uint8_t b4 : 1;		//!< bit 4
	uint8_t b5 : 1;		//!< bit 5
	uint8_t b6 : 1;		//!< bit 6
	uint8_t b7 : 1;		//!< bit 7
} Flag;

//! umozliwia odnoszenie sie do calego bajtu jak i poszczegolnych bitow
typedef union {
	//! bajt danych
	uint8_t value;
	Flag flag;
} ByteFlag;

//! informacja o dzialaniu urzadzenia
typedef enum {
	OFF = 0, 			//!< wylaczony
	ON = 1				//!< wlaczony
} BinarySwitch;

//! informacja o stanie logicznym
typedef enum {
	LOW = 0, 			//!< stan niski
	HIGH = 1			//!< stan wysoki
} LogicState;

//! dodatkowa tablica do wyswietlania danych rejestru jako debugger (jedna pozycja to jedna dioda)
extern uint8_t tabl[];

//! laduje byte do tabl, wykorzystywana jako debugger
inline void LoadToBuffer(uint8_t byte) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		tabl[i] = (byte % 2) ? 1 : 0;
		byte >>= 1;
	}
}

//! parametry wejsciowe funkcji Histereza
typedef struct {
	uint16_t ui16Threshold;	//!< wartosc liczby gdy zmiana stanu wytjsciowego
	uint8_t uiDelta;		//!< poziom histerezy
	uint8_t uiMinValue;		//!< minimalna wartosc wyjsciowa
	uint8_t uiMaxValue;		//!< maksymalna wartosc wyjsciowa
	uint16_t ui16Value;		//!< na jej podstawie generowane jest wyjscie, uiThreshold to prog dla tej liczby
	volatile uint8_t *uiOutValue;	//!< wyprowadzony za zewnatrz wynik histerezy, przyjmuje wartosci uiMinValue i uiMaxValue
} HystData;

inline void Hysteresis(HystData *sHistData) {
	if (sHistData->ui16Value < (sHistData->ui16Threshold - sHistData->uiDelta))
		*(sHistData->uiOutValue) = sHistData->uiMinValue;
	if (sHistData->ui16Value > (sHistData->ui16Threshold + sHistData->uiDelta))
			*(sHistData->uiOutValue) = sHistData->uiMaxValue;
}
#endif /* DATA_TYPES_H_ */
