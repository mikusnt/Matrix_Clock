

#include "ds3231.h"

/*
 *
 * 		Funkcje wewnetrzne
 *
 */

/*! konwertuje liczbe z zapisu dziesietnego na bcd */
static inline uint8_t dec2bcd(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
} // END uint8_t dec2bcd

/*! konwertuje liczbe z zapisu bcd na dziesietny */
static inline uint8_t bcd2dec(uint8_t bcd) {
	return (((bcd >> 4) & 0x0F) * 10) + (bcd & 0x0F);
} // END uint8_t bcd2dec

/*! funkcja zapisu 8 bitow danych po I2C, 0 gdy b³¹d
 * @param		deviceAddr adres urzadzenia na magistrali I2C
 * @param		registerAddr adres rejestru w danym urzadzeniu
 * @param		data dane ktore maja byc przeslane
 * @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
static uint8_t I2C_WriteRegister(uint8_t deviceAddr, uint8_t registerAddr, uint8_t data) {
        I2C_InitStart(START);  //start I2C

        I2C_WriteAdr(deviceAddr);  //wyslij adres DS3231
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Write(registerAddr);
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Write(data);  //wysy³¹ 3 ardumenty funkcji I2C_WriteRegister
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Stop();   //stop I2C
        return 1;
} // END void I2C_WriteRegister

/*! funkcja odczytu 8 bitow danych po I2C, 0 gdy b³¹d
 * @param		deviceAddr adres urzadzenia na magistrali I2C
 * @param		registerAddr adres rejestru w danym urzadzeniu
 * @param		data_s dane odczytne z urzadzenia
 * @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
static uint8_t I2C_ReadRegister(uint8_t deviceAddr, uint8_t registerAddr, uint8_t *data_s) {

        I2C_InitStart(START);  //start I2C

        I2C_WriteAdr(deviceAddr);  //wyslij adres DS3231
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Write(registerAddr);
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Start(REP_START);  // 2 start I2C, musi byæ!!!
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_WriteAdr(deviceAddr + 1); // 1 to bit odczytu
        if (I2C_Error) { I2C_FailStop(); return 0; }

        *data_s = I2C_Read(NACK); // odczytaj rejestr ze zgloszeniem ze to ostatni
        if (I2C_Error) { I2C_FailStop(); return 0; }

        I2C_Stop();   //stop I2C
        return 1;
} // END uint8_t I2C_ReadRegister

/*
 *
 *		Funkcje z pliku h
 *
 */

/*! @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
uint8_t DS3231_Init(void){
    uint8_t byte;
    if (I2C_ReadRegister(DS3231, CONTROL_0_REGISTER, &byte)) {

    	byte &= CH_MASK;
    	byte |= (1 << BBSQW)
    		| (1 << CONV);
    	if (I2C_WriteRegister(DS3231, CONTROL_0_REGISTER, byte) &&
    			I2C_WriteRegister(DS3231, CONTROL_1_REGISTER, 0)) {
    		return 1;
    	}
    	return 0;
    }
    return 0;
} // END void DS3231_init

/*! @param 			hour_s wskaznik pobieranych godzin
 *  @param			minute_s wskaznik pobieranych minut
 *  @param			second_s wskaznik pobieranych sekund
 *  @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
uint8_t DS3231_GetTime(uint8_t *hour_s, uint8_t *minute_s, uint8_t *second_s)
{
	uint8_t temp_hour, temp_minute, temp_second;

	if (I2C_ReadRegister(DS3231,HOURS_REGISTER, &temp_hour)
			&& I2C_ReadRegister(DS3231,MINUTES_REGISTER, &temp_minute)
			&& I2C_ReadRegister(DS3231,SECONDS_REGISTER, &temp_second)) {

		//temp_hours = bcd2dec(temp_hours);
		temp_minute = bcd2dec(temp_minute);
		temp_second = bcd2dec(temp_second);

		if (temp_hour & 0x40) // gdy tryb am/pm
			temp_hour &= HOURS_12_MASK; // tryb 12 godzinny
		else temp_hour &= HOURS_24_MASK; // tryb 24 godzinny, wykorzystywane 6 bitow
		temp_hour = bcd2dec(temp_hour);

		// kontrola poprawnosci wartosci
		if ((temp_hour < 24) && (temp_minute < 60) && (temp_second < 60)) {
			*hour_s = temp_hour;
			*minute_s = temp_minute;
			*second_s = temp_second;
			return 1;
		}
	}
	return 0;

} // END void DS3231_GetTime

/*! @param 			month_s wskaznik pobieranego miesiaca
 *  @param			day_s wskaznik pobieranego dnia miesiaca
 *  @param			year_s wskaznik pobieranego roku
 *  @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
uint8_t DS3231_GetDate(uint8_t *day_s, uint8_t *month_s, uint8_t *year_s)
{
	if (I2C_ReadRegister(DS3231,MONTHS_REGISTER, month_s)
			&& I2C_ReadRegister(DS3231,DAYS_REGISTER, day_s)
			&& I2C_ReadRegister(DS3231,YEARS_REGISTER, year_s)) {
		*year_s = bcd2dec(*year_s);
		if (*month_s & CENTURY_MASK) {
			*year_s += 100;
		}
		*month_s = bcd2dec(*month_s & MONTH_MASK);		// pozbycie sie wieku na 7 bicie, 6 i 5 pusty
		*day_s = bcd2dec(*day_s);

		return 1;
	}
	else return 0;

} // END void DS3231_GetDate

/*! @param  		hour ustawiana godzina dziesietna
 *  @param			minute ustawiana minuta dziesietna
 *  @param			second ustawiana sekunda dziesietna
 *  @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
uint8_t  DS3231_SetTime(uint8_t hour, uint8_t minute, uint8_t second) {
	// test poprawnosci
	if ((hour > 23) || (minute > 59) || (second > 59)) return 0;

	if (I2C_WriteRegister(DS3231,HOURS_REGISTER, dec2bcd(hour) & HOURS_24_MASK)
			&& I2C_WriteRegister(DS3231,MINUTES_REGISTER, dec2bcd(minute))
			&& I2C_WriteRegister(DS3231,SECONDS_REGISTER, dec2bcd(second)))
		return 1;
	return 0;
} // END void  DS3231_SetTime

/*! @param  		day ustawiany dzien miesiaca dziesietnie
 *  @param			month ustawiany miesiac dziesietnie
 *  @param			year ustawiany rok dziesietnie
 *  @return 		1 gdy powodzenie, 0 gdy niepowodzenie*/
uint8_t  DS3231_SetDate(uint8_t day, uint8_t month, uint8_t year) {
	if ((day > 31) || (month > 12) || (year > 199)) return 0;
	// ustawienie bitu kolejnego wieku
	dec2bcd(month);
	if (year > 99) {
		year -= 100;
		month |= CENTURY_MASK;
	}
	if (I2C_WriteRegister(DS3231,MONTHS_REGISTER, month)
			&& I2C_WriteRegister(DS3231,DAYS_REGISTER, dec2bcd(day))
			&& I2C_WriteRegister(DS3231,YEARS_REGISTER, dec2bcd(year)))
		return 1;
	else return 0;
} // END void  DS3231_SetDate

void DS3231_Test() {
	/*  Test poprawnej pracy RTC, resetuje WATCHDoga  */
	uint8_t hour, min, sec, nhour, nmin, nsec;
	uint8_t count = 10; // maksymalna ilosc powtorzen
	DS3231_GetTime(&hour, &min, &sec);
	D_MS(1200);
	DS3231_GetTime(&nhour, &nmin, &nsec);


	/*  Jeœli RTC nie pracuje to wgraj now¹ godzinê, jeœli nie pracuje po wgraniu nowego czasu to daj sygna³ awaryjny  */
	while ((sec == nsec) && (count--)) {
		wdt_disable();
		wdt_enable(WDTO_2S);
		DS3231_Init();
		//DS3231_SetTime(12, 0, 0);
		DS3231_GetTime(&hour, &min, &sec);
		D_MS(1200);
		DS3231_GetTime(&nhour, &nmin, &nsec);
		wdt_reset();
	}
	wdt_disable();
	if (!count) {
		wdt_enable(WDTO_15MS);
		D_MS(50);
	}
}
