/*
 * date_time.c
 *
 *  Created on: 9 lis 2017
 *      Author: MS
 */

#include "date_time.h"


/*! pakuje czas z pojedynczych cyft na liczby*/
static inline void LoadToGroupTime(Time *t) {
	t->uiHours = t->uitSingleTime[TimeH10Pos] * 10 + t->uitSingleTime[TimeH0Pos];
	t->uiMinutes = t->uitSingleTime[TimeM10Pos] * 10 + t->uitSingleTime[TimeM0Pos];
	t->uiSeconds = t->uitSingleTime[TimeS10Pos] * 10 + t->uitSingleTime[TimeS0Pos];
}

/*! inkrementuje i dekrementuje z okreslonym opoznieniem cyfry czasu
 * @param 		from_e adres struktury dazacej do aktualnej
 * @param 		to aktualna struktura czasu, czas powiniec byc rozpakowanydo pojedynczych cyfr
 * najlepiej podczas przerwania RTC
 * @see 		LoadToSingleTime()*/
extern void TryTimeUpdateMS(Time *from_e, Time *to) {
	bool bDec = true;
	// gdy odliczanie sie zakonczylo sprawdz czy trzeba dekrementowac cyfre
	if (!(--from_e->uiActTimeMs)) {
		// pojedyncze czesci minuty
		if(from_e->uitSingleTime[TimeM0Pos] > to-> uitSingleTime[TimeM0Pos])
			from_e->uitSingleTime[TimeM0Pos]--;
		else
			// dziesietne czesci minuty
			if(from_e->uitSingleTime[TimeM10Pos] > to-> uitSingleTime[TimeM10Pos])
				from_e->uitSingleTime[TimeM10Pos]--;
			else
				// pojedyncze czesci godziny
				if(from_e->uitSingleTime[TimeH0Pos] > to-> uitSingleTime[TimeH0Pos])
					from_e->uitSingleTime[TimeH0Pos]--;
				else
					// dziesietne czesci godziny
					if(from_e->uitSingleTime[TimeH10Pos] > to-> uitSingleTime[TimeH10Pos])
						from_e->uitSingleTime[TimeH10Pos]--;
					else bDec = false;

		from_e->uiActTimeMs = TIME_DECREMENT_MS;
	}
	// inkrementacja gdy nie dokonano zadnej dekrementacji
	if (!bDec) {
		// inkrementacja aktualnego czasu
		if(from_e->uitSingleTime[TimeM0Pos] < to->uitSingleTime[TimeM0Pos])
			from_e->uitSingleTime[TimeM0Pos]++;
		if(from_e->uitSingleTime[TimeM10Pos] < to->uitSingleTime[TimeM10Pos])
			from_e->uitSingleTime[TimeM10Pos]++;
		if(from_e->uitSingleTime[TimeH0Pos] < to->uitSingleTime[TimeH0Pos])
			from_e->uitSingleTime[TimeH0Pos]++;
		if(from_e->uitSingleTime[TimeH10Pos] < to->uitSingleTime[TimeH10Pos])
			from_e->uitSingleTime[TimeH10Pos]++;
	}
	LoadToGroupTime(from_e);
}
