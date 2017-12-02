/*!
 * @file ds3231.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-11-10 $
 * $Created: 2017-11-10 $
 * @version 1.0
 *
 * NIETESTOWANY!!!<br>
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy obslugujacy komunikacje z DS3231 po I2C.
 * @see i2c_twi.h
 */


#ifndef DS3231_H
#define DS3231_H

#include <avr/io.h>

#include "../group_includes.h"
#include "../I2C_TWI/i2c_twi.h"

/*
 *
 * 		Makroinstrukcje
 *
 */

#define DS3231 0x68 // I2C bus address of DS3231 RTC  0xD0
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

//bity sterujace
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
#define MONTH_MASK 0x3F
//#define RAM_BEGIN 0x08
//#define RAM_END 0x3F

/*
 *
 * 		Deklaracje funkcji
 *
 */

//! inicjalizacja zegara, wybudzenie
extern uint8_t DS3231_Init(void);
//! pobranie czasu w formacie dziesietnym
extern uint8_t DS3231_GetTime(uint8_t *hour_s, uint8_t *minute_s, uint8_t *second_s);
//! pobranie daty w formacie dziesietnym
extern uint8_t DS3231_GetDate(uint8_t *day_s, uint8_t *month_s, uint8_t *year_s);
//! ustawienie czasu w formacie dziesietnym
extern uint8_t  DS3231_SetTime(uint8_t hour, uint8_t minute, uint8_t second); //time set
//! ustawienie daty w formacie dziesietnym
extern uint8_t  DS3231_SetDate(uint8_t day, uint8_t month, uint8_t year);  //set date
//! test poprawnej pracy
extern void DS3231_Test(); // test poprawnosci komunikacji/dzialania, resetuja WATCHDoga

#endif

