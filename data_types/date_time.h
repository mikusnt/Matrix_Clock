/*!
 * @file date_time.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-05-05 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Used uC pins: 0<br>
 * Header file containing date and time structure.
 */

#ifndef DEVICES_DATE_TIME_H_
#define DEVICES_DATE_TIME_H_

#include "../group_includes.h"

/*
 *
 *		Macros
 *
 */

//! time of decrement time digit in milliseconds
#define TIME_DECREMENT_MS 100
//! max value of progress between displaying time and actual RTC time
#define MAX_PROGRESS 8


/*
 *
 *		Main data types
 *
 */

//!position of time digit in single digit mode
/*! @see Time*/
typedef enum {
	TimeH10Pos = 0,		//!< digit of tens of hour
	TimeH0Pos,     		//!< digit of ones of hour
	TimeM10Pos,   		//!< digit of tens of minute
	TimeM0Pos,     		//!< digit of ones of minute
	TimeS10Pos,    		//!< digit of tens of second
	TimeS0Pos      		//!< digit of ones of second
} SingleTimePos;

//! main time structure
typedef struct {
	//! decimal hour
	uint8_t uiHour;
	//! decimal minute
	uint8_t uiMinute;
	//! decimal second
	uint8_t uiSecond;
	//! table of single digit time
	/*! @see SingleTimePos*/
	uint8_t uitSingleTime[6];
	//! table of progress between displaying time and actual RTC time
	uint8_t uiSingleProgress[6];
	//! decimal day
	uint8_t uiDay;
	//! decimal month
	uint8_t uiMonth;
	//! three digit year (hundreds, tens, ones)
	uint8_t uiYear;
} TimeDate;

/*
 *
 *		Declaration of functions
 *
 */

//! initialization of time structure
inline void TimeInit(TimeDate *t);
//! unpack decimal time to one digit time
inline void LoadToSingleTime(TimeDate *t);
//! reset table of progress to 0
extern void ResetProgress(TimeDate *time);
//! pack one digit time to decimal time
extern void LoadToDecimalTime(TimeDate *t);

/*
 *
 *		Inline functions
 *
 */

/*! @param 		t pointer of time structure*/
inline void TimeInit(TimeDate *t) {
	for (int i = TimeH10Pos; i <= TimeS0Pos; i++) {
		t->uitSingleTime[i] = 0;
		t->uiSingleProgress[i] = 0;
	}
} // END inline void TimeInit

/*! @param 		t pointer of time structure*/
inline void LoadToSingleTime(TimeDate *t) {
	t->uitSingleTime[TimeH10Pos] = t->uiHour / 10;
	t->uitSingleTime[TimeH0Pos] = t->uiHour % 10;

	t->uitSingleTime[TimeM10Pos] = t->uiMinute / 10;
	t->uitSingleTime[TimeM0Pos] = t->uiMinute % 10;

	t->uitSingleTime[TimeS10Pos] = t->uiSecond / 10;
	t->uitSingleTime[TimeS0Pos] = t->uiSecond % 10;
} // END inline void LoadToSingleTime


#endif /* DEVICES_DATE_TIME_H_ */
