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
#define ADC_READ_COUNT 20
// kana³y ADC
//! adres baterii w ADC
#define ADC_BAT (1 << PC0)
//! adres fotorezystora w ADC
#define ADC_PHOTO (1 << PC1)

//! maska ADC przeyslaniajaca bity kanalu ADC w ADMUX
#define ADMUX_MASK 0xF0
//! test aktywnego kanalu ADC
#define ADC_ADR_IS(x) (((~ADMUX_MASK) & ADMUX) == x)
//! stosunek liczby wyliczen sredniej od fotorezystora wzgledem obliczenia sredniej baterii
//! obliczenia poprawne gdy pojedynczy odczyt ADC cosekunde
#define PHOTO_TO_BATTERY_RATIO ((3600 / ADC_READ_COUNT) - (ADC_READ_COUNT * 2))


//! wartosc ADC fotorezystora przy ktorej nastepuje skok jasnosci
#define BRIGHT_CHANGE_POINT 200
//! odleglosc od BRIGHT_CHANGE_POINT dla histerezy
#define BRIGHT_HIST_DELTA 25
//! minimalna wartosc histerezy jasnosci z odczytu fotorezystora
#define HIST_LOW (BRIGHT_CHANGE_POINT - BRIGHT_HIST_DELTA)
//! maksymalna wartosc histerezy jasnosci z odczytu fotorezystora
#define HIST_HIGH (BRIGHT_CHANGE_POINT + BRIGHT_HIST_DELTA)
//! minimalna jasnosc matrycy
#define MIN_MATRIX_BRIGHT 2
//! maksymalna jasnosc matrycy
#define MAX_MATRIX_BRIGHT 15

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

#define CHARGE_DDR DDRB
#define CHARGE_PORT PORTB
#define CHARGE_ADDR (1 << PB2)
#define CHARGE_THRESHOLD 700

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
	uint8_t uiActReadCount;
	//! aktualna ilosc wyliczenia srednich dla fotorezystora, jako dzielnik dla czestotliwosci
	//! sredniej baterii
	uint16_t ui16ActPhotoCount;
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
	//! aktualna wartosc jasnosci matrycy
	uint8_t uiActBright;
} ADCVoltageData;

/*
 *
 *		Deklaracje funkcji
 *
 */

//! inicjalizacja Timera0 odpowiedzialnego za prace matrycy
extern void Timer0Init();
//! inicjalizacja Timera2 odpowiedzialnego za odniesienie czasu 1 ms
extern void Timer2Init();
//! inicjalizacja struktury ADCVoltageData
extern void ADCVoltageDataInit(ADCVoltageData *a);
//! uruchomienie odczytu ADC
inline void ADCStart();
//! laduje kanal ADC ze struktury do odpowiedniego rejestru
inline void SetADCChannel(ADCVoltageData *a);
//! zaladowanie odczytanych danych do struktury ADC
inline void ReadADCToADCData(ADCVoltageData *a);
//! zwraca jasnosc matrycy na podstawie odczytu fotorezystora
inline uint8_t MatrixBright(ADCVoltageData *a);

//! inicjalizacja przyciskow
extern void ButtonsInit();

//! na podstawie sredniego napiecia baterii decyduje czy uruchomic tryb ladowania
inline void TryCharge(ADCVoltageData *a);

/*
 *
 *		Definicje funkcji inline
 *
 */

/*! wymagane kazdorazowo jako inicjalizacja pojedynczego odczytu, nalezy wczesniej
 * zaladowac kanal lub zainicjalizowac ADCData */
inline void ADCStart() {
	ADCSRA |= (1 << ADSC); // uruchmienie konwersji
}

/*! nalezy wywolac przed dokonaniem pomiaru
 * @param 		a adres struktury przetwornika ADC*/
inline void SetADCChannel(ADCVoltageData *a) {
	ADMUX = (ADMUX & ADMUX_MASK) | a->uiActChannel; // ustawienie odpowiedniego kana³u ADC
}

/*! jest wykonywany jako obsluga przerwania pomiaru ADC
 * @param 		a adres struktury przetwornika ADC
 * @see ADCStart()*/
inline void ReadADCToADCData(ADCVoltageData *a) {
	// dodawanie skladnikow sumy
	if (a->uiActChannel == adcBatteryAdr) a->ui16BatSum += ADC;
	else if (a->uiActChannel == adcPhotoAdr) a->ui16PhotoSum += ADC;

	// gdy zakonczono pomiary dla danej sredniej
	if (++a->uiActReadCount >= ADC_READ_COUNT) {
		a->uiActReadCount = 0;
		// gdy zakonczono pomiary fotorezystora
		if (a->uiActChannel == adcPhotoAdr) {
			a->uiPhotoAvg = a->ui16PhotoSum / ADC_READ_COUNT;
			a->ui16PhotoSum = 0;
			// gdy osiagnieto czas obliczanie sredniej baterii
			if (++a->ui16ActPhotoCount >= PHOTO_TO_BATTERY_RATIO) {
				a->ui16ActPhotoCount = 0;
				a->uiActChannel = adcBatteryAdr;
			}
		}
		// gdy zakonczono pomiary baterii z powrotem do pomiarow fotorezystora
		if (a->uiActChannel == adcBatteryAdr) {
			a->uiBatAvg = a->ui16BatSum / ADC_READ_COUNT;
			a->ui16BatSum = 0;
			a->uiActChannel = adcPhotoAdr;
		}
		// zaladuj nowy kanal do rejestru ADC
		SetADCChannel(a);
	}
} // END inline void ReadADCToADCData

/*! @param 		a adres struktury przetwornika ADC*/
inline uint8_t MatrixBright(ADCVoltageData *a) {
	if (a->uiPhotoAvg < HIST_LOW) a->uiActBright = MIN_MATRIX_BRIGHT;
	else if (a->uiPhotoAvg > HIST_HIGH) a->uiActBright = MAX_MATRIX_BRIGHT;
	return a->uiActBright;
}

/*! powinno byc wyzwalane co jakis okres czasu np. pelna godzine
  @param 		a adres struktury przetwornika ADC*/
inline void TryCharge(ADCVoltageData *a) {
	if (a->uiBatAvg < CHARGE_THRESHOLD) CHARGE_PORT |= CHARGE_ADDR;
	else CHARGE_PORT &= ~CHARGE_ADDR;
}

#endif /* OTHER_DEVICES_H_ */
