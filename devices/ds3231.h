/*!
 * @file ds3231.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-11 $
 * $Created: 2017-11-10 $
 * @version 1.1
 *
 * Used uC pins: 0<br>
 * Header file containing DS3231 communication by I2C interface.
 * @see i2c_twi.h
 */


#ifndef DS3231_H
#define DS3231_H

#include <avr/io.h>

#include "../group_includes.h"
#include "../I2C_TWI/i2c_twi.h"

/*
 *
 * 		Macros
 *
 */

#define DS3231 0xD0 // I2C bus address of DS3231 RTC  0xD0
#define CH_MASK 0b01100000

#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_0_REGISTER 0x0E
#define CONTROL_1_REGISTER 0x0F
#define TEMPERATURE_REGISTER 0x11

//Control bytes
#define EOSC 7
#define BBSQW 6
#define CONV 5
#define RS2 4
#define RS1 3
#define INTCN 2
#define A2IE 1
#define A1IE 0

#define HOURS_24_MASK 0x3F
#define HOURS_12_MASK 0x1F
#define MONTH_MASK 0x1F
#define CENTURY_MASK 0x80
//#define RAM_BEGIN 0x08
//#define RAM_END 0x3F

/*
 *
 * 		Declaration of functions
 *
 */

//! Initialize timer, wake up
extern uint8_t DS3231_Init(void);
//! read decimal time from RTC
extern uint8_t DS3231_GetTime(uint8_t *hour_s, uint8_t *minute_s, uint8_t *second_s);
//! read decimal date from RTC
extern uint8_t DS3231_GetDate(uint8_t *day_s, uint8_t *month_s, uint8_t *year_s);
//! write decimal time to RTC
extern uint8_t  DS3231_SetTime(uint8_t hour, uint8_t minute, uint8_t second);
//! write decimal date to RTC
extern uint8_t  DS3231_SetDate(uint8_t day, uint8_t month, uint8_t year);
//! RTC test
extern void DS3231_Test();

#endif

