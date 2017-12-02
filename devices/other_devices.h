/*!
 * @file other_devices.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-11-10 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * NIETESTOWANY!!!<br>
 * Uzyte piny procesora: 6<br>
 * Plik naglowkowy zawierajacy konfiguracje timerow sprzetowych oraz przetwornika ADC w trybie
 * pojedynczego odczytu. Odczytanie zawartosci ADC odbywa sie w ramach przerwania po skonczonym
 * pomiarze ADC.
 */

#ifndef OTHER_DEVICES_H_
#define OTHER_DEVICES_H_

#include "../group_includes.h"

/*
 *
 *		Makroinstrukcje
 *
 */

//! ilosc pomiarow ADC do sredniej
#define ADC_READ_COUNT 10
// kana³y ADC
//! adres baterii w ADC
#define ADC_BAT (1 << PC0)
//! adres fotorezystora w ADC
#define ADC_PHOTO (1 << PC1)

//! maska ADC przeyslaniajaca bity kanalu ADC w ADMUX
#define ADMUX_MASK 0xF0
//! test aktywnego kanalu ADC
#define ADC_ADR_IS(x) (((~ADMUX_MASK) & ADMUX) == x)

/*  Zlacza we  */
#define BUTTON_LEFT_DDR DDRD
#define BUTTON_LEFT_PORT PORTD
#define BUTTON_LEFT_PIN PIND
#define BUTTON_LEFT_ADDR (1 << PD0)

#define BUTTON_RIGHT_DDR DDRD
#define BUTTON_RIGHT_PORT PORTD
#define BUTTON_RIGHT_PIN PIND
#define BUTTON_RIGHT_ADDR (1 << PD0)

#define BUTTON_CENTER_DDR DDRD
#define BUTTON_CENTER_PORT PORTD
#define BUTTON_CENTER_PIN PIND
#define BUTTON_CENTER_ADDR (1 << PD0)

#define LEFT_PRESSED (!(BUTTON_LEFT_PIN & BUTTON_LEFT_ADDR))
#define RIGHT_PRESSED (!(BUTTON_RIGHT_PIN & BUTTON_RIGHT_ADDR))
#define CENTER_PRESSED (!(BUTTON_CENTER_PIN & BUTTON_CENTER_ADDR))

// KONIECZNIE USTAWIC MASKI PRZERWAN PO WYBRANIU ADRESOW WEJSC PRZYCISKOW!!!

/*
 *
 *		Glowne typy danych
 *
 */

//! adresy kanalow ADC
/*! @see ADCData*/
typedef enum {
	adcBatteryAdr = 0,	//!< adres kanalu odczytu baerii
	adcPhotoAdr			//!< adres kanalu odczytu fotorezystora
} ADCAdr;

//! glowny typ danych przetwornika ADC
typedef struct {
	//! aktualna ilosc pomiarow do danej sumy
	uint8_t uiActReadNumber;
	//! aktualny numer kanalu do ktorego ladowane sa odczyty ADC
	/*! @see ADCAdr*/
	uint8_t uiActChannel;
	//! chwilowa suma odczytow ADC dla baterii
	uint16_t ui16BatSum;
	//! srednia wartosc ADC da baterii
	uint16_t uiBatAvg;
	//! chwilowa suma odczytow ADC dla fotorezystora
	uint16_t ui16PhotoSum;
	//! srednia wartosc ADC da fotorezystora
	uint16_t uiPhotoAvg;
} ADCData;

/*
 *
 *		Deklaracje funkcji
 *
 */

//! inicjalizacja Timera0 odpowiedzialnego za prace matrycy
extern void Timer0Init();
//! inicjalizacja Timera2 odpowiedzialnego za odniesienie czasu 1 ms
extern void Timer2Init();

//! inicjalizacja ADC
extern void ADCInit(ADCData *a);
//! uruchomienie odczytu ADC
inline void ADCStart();
//! laduje aktualny kanal ADC do odpowiedniego rejestru
inline void SetADCChannel(ADCData *a);
//! zaladowanie odczytanych danych do struktury ADC
inline void ReadADCToADCData(ADCData *a);

//! inicjalizacja przyciskow
extern void ButtonsInit();
/*
 *
 *		Definicje funkcji inline
 *
 */

// uruchomienie odczytu ADC
/*! wymagane wczesniejsze zaladowanie kanalu lub inicjalizacja ADCData */
inline void ADCStart() {
	ADCSRA |= (1 << ADSC); // uruchmienie konwersji
}

/*! nalezy wywolac przed dokonaniem pomiaru
 * @param 		a adres struktury przetwornika ADC*/
inline void SetADCChannel(ADCData *a) {
	ADMUX = (ADMUX & ADMUX_MASK) | a->uiActChannel; // ustawienie odpowiedniego kana³u ADC
}

/*! jest wykonywany jako obsluga przerwania pomiaru ADC
 * @param 		a adres struktury przetwornika ADC
 * @see ADCStart()*/
inline void ReadADCToADCData(ADCData *a) {
	// dodawanie skladnikow sumy
	if (a->uiActChannel == adcBatteryAdr)
		a->ui16BatSum += ADC;
	else
		if (a->uiActChannel == adcPhotoAdr)
			a->ui16PhotoSum += ADC;

	// gdy wszystkie pomiary w ramach kanalu ustaw adres zerowy
	if (++a->uiActReadNumber > ADC_READ_COUNT) {
		a->uiActReadNumber = 0;
		// gdy wszystkie kanaly zrealizowane oblicz srednie
		if (++a->uiActChannel > adcPhotoAdr) {
			a->uiActChannel = adcBatteryAdr;
			a->uiBatAvg = a->ui16BatSum / ADC_READ_COUNT;
			a->uiPhotoAvg = a->ui16PhotoSum / ADC_READ_COUNT;

			a->ui16BatSum = 0;
			a->ui16PhotoSum = 0;
		}
		// zaladuj nowy kanal do rejestru ADC
		SetADCChannel(a);
	}
} // END inline void ReadADCToADCData

#endif /* OTHER_DEVICES_H_ */
