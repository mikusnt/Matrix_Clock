/*!
 * @file date_time.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see date_time.h*/

#include "date_time.h"


/*! @param 		t pointer of time structure*/
void LoadToDecimalTime(TimeDate *t) {
	t->uiHour = t->uitSingleTime[TimeH10Pos] * 10 + t->uitSingleTime[TimeH0Pos];
	t->uiMinute = t->uitSingleTime[TimeM10Pos] * 10 + t->uitSingleTime[TimeM0Pos];
	t->uiSecond = t->uitSingleTime[TimeS10Pos] * 10 + t->uitSingleTime[TimeS0Pos];
} // END  void LoadToGroupTime

/*! @param 		t pointer of time structure*/
void ResetProgress(TimeDate *t) {
	for (uint8_t i = 0; i < 4; i++) {
		t->uiSingleProgress[i] = 0;
	}
} // END void ResetProgress

void TryDecrementTime(TimeDate *t) {
	if (t->uiSecond) {
		t->uiSecond--;
	} else if (t->uiMinute) {
		t->uiMinute--;
		t->uiSecond = 59;
	} else if (t->uiHour) {
		t->uiHour--;
		t->uiMinute = 59;
		t->uiSecond = 59;
	} else if (t->uiDay) {
		t->uiDay--;
		t->uiHour = 23;;
		t->uiMinute = 59;
		t->uiSecond = 59;
	}
	LoadToSingleTime(t);
}

void CopyDateTime(TimeDate *from, TimeDate *to) {
	to->uiHour = from->uiHour;
	to->uiMinute = from->uiMinute;
	to->uiSecond = from->uiSecond;

	to->uiDay = from->uiDay;
	to->uiMonth = from->uiMonth;
	to->uiYear = from->uiYear;

	for (int i = 0; i < 5; i++)
		to->uiSingleProgress[i] = from->uiSingleProgress[i];
	LoadToSingleTime(to);
}

