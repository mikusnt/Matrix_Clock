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
}
