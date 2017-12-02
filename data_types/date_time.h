/*!
 * @file date_time.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-11-10 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * NIETESTOWANY!!!<br>
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy zawierajacy obsluge czasu i daty
 */

#ifndef DEVICES_DATE_TIME_H_
#define DEVICES_DATE_TIME_H_

#include "../group_includes.h"

/*
 *
 *		Makroinstrukcje
 *
 */

//! czas dekrementacji cyfry czasu o 1 liczony w milisekundach [ms]
#define TIME_DECREMENT_MS 100


/*
 *
 *		Glowne typy danych
 *
 */

//! polozenie poszczegolnych wartosci czasu w tablicy
/*! @see Time*/
typedef enum {
	TimeH10Pos = 0,		//!< cyfra dziesiatek godziny
	TimeH0Pos,     		//!< cyfra jednosci godziny
	TimeM10Pos,   		//!< cyfra dziesiatek minuty
	TimeM0Pos,     		//!< jednosci minuty
	TimeS10Pos,    		//!< cyfra dziesiatek sekundy
	TimeS0Pos      		//!< jednosci sekundy
} SingleTimePos;

//! glowna struktura czasu
typedef struct {
	//! aktualny czas w ms, gdy 0 zmiana stanu i zaladowanie z TIME_DECREMENT_MS
	uint8_t uiActTimeMs;
	//! godzina dziesietnie
	uint8_t uiHours;
	//! minuty dziesietnie
	uint8_t uiMinutes;
	//! sekundy dziesietnie
	uint8_t uiSeconds;
	//! tablica poszczegolnych cyfr czasu
	/*! @see SingleTimePos*/
	uint8_t uitSingleTime[6];
} Time;

//! glowna struktura daty
typedef struct {
	//! dni miesiaca dziesietnie
	uint8_t uiDays;
	//! miesiace dziesietnie
	uint8_t uiMonths;
	//! trzycyfrowy rok (bez tysiecy)
	uint8_t uiYears;
} Date;

/*
 *
 *		Deklaracje funkcji
 *
 */
//! inicjalizacja struktury
inline void TimeInit(Time *t);
//! laduje do struktury stala czasowa
inline void ResetTimeDecCounter(Time *t);
//! rozbija dwucyfrowe skladniki czasu na jednocyfrowe
inline void LoadToSingleTime(Time *t);

// pakuje czas z pojedynczych cyft na liczby
//static inline void LoadToGroupTime(Time *t);

//! aktualizuje czas w buforze na podstawie oryginalnego
extern void TryTimeUpdateMS(Time *edit_from, Time *to);

/*! w tym ladowanie najpotrzebniejszych zmiennych
 * @param 		t adres struktury czasu*/
inline void TimeInit(Time *t) {
	ResetTimeDecCounter(t);
	for (int i = TimeH10Pos; i <= TimeS0Pos; i++)
		t->uitSingleTime[i] = 0;
}


inline void ResetTimeDecCounter(Time *t) {
	t->uiActTimeMs = TIME_DECREMENT_MS;
}

/*!
 * @param 		t adres struktury czasu*/
inline void LoadToSingleTime(Time *t) {
	t->uitSingleTime[TimeH10Pos] = t->uiHours / 10;
	t->uitSingleTime[TimeH0Pos] = t->uiHours % 10;

	t->uitSingleTime[TimeM10Pos] = t->uiMinutes / 10;
	t->uitSingleTime[TimeM0Pos] = t->uiMinutes % 10;

	t->uitSingleTime[TimeS10Pos] = t->uiSeconds / 10;
	t->uitSingleTime[TimeS0Pos] = t->uiSeconds % 10;
}

#endif /* DEVICES_DATE_TIME_H_ */
