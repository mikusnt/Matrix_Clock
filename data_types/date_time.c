/*!
 * @file date_time.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see date_time.h*/

#include "date_time.h"

/*
 *
 *		Internal data types, functions
 *
 */
/*! internal EEMEM addr of date eand time modification, to manual update before
 compile, data from m$ by Bluetooth */
TimeDate EEMEM eeTimeDate = {15, 10, 18};

/*! @param 		t pointer of time structure*/
inline void LoadToDecimalTime(TimeDate *t) {
	t->uiHour = t->uitSingleTime[TimeH10Pos] * 10 + t->uitSingleTime[TimeH0Pos];
	t->uiMinute = t->uitSingleTime[TimeM10Pos] * 10 + t->uitSingleTime[TimeM0Pos];
	t->uiSecond = t->uitSingleTime[TimeS10Pos] * 10 + t->uitSingleTime[TimeS0Pos];
} // END  void LoadToGroupTime

/*
 *
 *		Definitions of functions
 *
 */

/*! @param 		t pointer of time structure*/
void ResetProgress(TimeDate *t) {
	for (uint8_t i = 0; i < 4; i++) {
		t->uiSingleProgress[i] = 0;
	}
} // END void ResetProgress

/*! one second decrement time if it is possible
 * @param 		t out pointer of time structure
 */
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
} // END void TryDecrementTime

/*! @param		from source structure
 *  @param		to destination structure */
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
} // END void CopyDateTime

/*! @param		time pointer of time structure to write */
void WriteDateTimeToEEProm(TimeDate *time) {
	eeprom_update_block(time, &eeTimeDate, sizeof(TimeDate));
	eeprom_busy_wait();
} // END void WriteDateTimeEEProm

/*! @param		time pointer of timer structure to read */
void ReadDateTimeFromEEProm(TimeDate *time) {
	eeprom_read_block(time, &eeTimeDate, sizeof(TimeDate));
	eeprom_busy_wait();
} // END void ReadDateTimeEEProm

