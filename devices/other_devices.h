/*!
 * @file other_devices.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 6<br>
 * Plik naglowkowy zawierajacy konfiguracje timerow sprzetowych oraz przetwornika ADC w trybie
 * pojedynczego odczytu. Odczytanie zawartosci ADC odbywa sie w ramach przerwania po skonczonym
 * pomiarze ADC. Za kazdym razem wymagane reczne uruchomienie kolejnego odczytu.
 */

#ifndef OTHER_DEVICES_H_
#define OTHER_DEVICES_H_

#include "../group_includes.h"
#include "../data_types/diode_matrix.h"

/*
 *
 *		Makroinstrukcje
 *
 */

//! ilosc pomiarow ADC do sredniej
#define ADC_READ_COUNT 2
// kana³y ADC
//! adres baterii w ADC
#define ADC_BAT (1 << PC0)
//! adres fotorezystora w ADC
#define ADC_PHOTO (1 << PC2)

//! maska ADC przeyslaniajaca bity kanalu ADC w ADMUX
#define ADMUX_MASK 0xF0
//! test aktywnego kanalu ADC
#define ADC_ADR_IS(x) (((~ADMUX_MASK) & ADMUX) == x)
//! stosunek liczby wyliczen sredniej od fotorezystora wzgledem obliczenia sredniej baterii
//! obliczenia poprawne gdy pojedynczy odczyt ADC co sekunde
#define PHOTO_TO_BATTERY_RATIO  59//((3600 / ADC_READ_COUNT) - (ADC_READ_COUNT * 2))


#define BLUETOOTH_DDR DDRD
#define BLUETOOTH_PORT PORTD
#define BLUETOOTH_PIN PIND
#define BLUETOOTH_ADDR (1 << PD7)
#define BLUETOOTH_IS_ON() (!(BLUETOOTH_PIN & BLUETOOTH_ADDR))

#define SQW_DDR DDRC
#define SQW_PORT PORTC
#define SQW_PIN PINC
#define SQW_ADDR (1 << PC1)
#define SQW_IS_HIGH() (SQW_PIN & SQW_ADDR)

#define ADC_PHOTO_ADR 2
/*
 *
 *		Glowne typy danych
 *
 */
//! glowny typ danych przetwornika ADC
typedef struct {
	//! aktualna ilosc pomiarow do danej sumy
	uint8_t uiActReadCount;
	//! chwilowa suma odczytow ADC dla fotorezystora
	uint16_t ui16PhotoSum;
	//! srednia wartosc ADC da fotorezystora
	uint16_t ui16PhotoAvg;
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
extern void ADCInit(volatile ADCVoltageData *a);
//! uruchomienie odczytu ADC
inline void ADCStart();
//! zaladowanie odczytanych danych do struktury ADC
inline void ReadADCToADCData(volatile ADCVoltageData *a, volatile uint8_t *bright);
//! inicjalizacja przyciskow
extern void PCINTInit();
extern void PowerReductionInit();


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

/*! jest wykonywany jako obsluga przerwania pomiaru ADC
 * @param 		a adres struktury przetwornika ADC
 * @param		bright adres jasnosci wynikowej
 * @see ADCStart()*/
inline void ReadADCToADCData(volatile ADCVoltageData *a, volatile uint8_t *bright) {
	// dodawanie skladnikow sumy
	a->ui16PhotoSum += ADC;

	// gdy zakonczono pomiary dla danej sredniej
	if (++a->uiActReadCount >= ADC_READ_COUNT) {
		a->uiActReadCount = 0;
		// gdy zakonczono pomiary fotorezystora
		a->ui16PhotoAvg = a->ui16PhotoSum / ADC_READ_COUNT;
		a->ui16PhotoSum = 0;
		HystData hystData = {800, 20, gamma_o[1], gamma_o[3], a->ui16PhotoAvg, bright};
		Hysteresis(&hystData);
	}
} // END inline void ReadADCToADCData

#endif /* OTHER_DEVICES_H_ */
