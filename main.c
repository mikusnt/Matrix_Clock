/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-04 $
 * $Created: 2017-11-04 $
 * @version 0.1
 *
 * Plik glowny projektu
 * @see info.txt
 */

#include "group_includes.h"
#include "devices/other_devices.h"
#include "devices/relay.h"
#include "devices/ds3231.h"
#include "devices/register.h"
#include "devices/bluetooth/uart_processing.h"
#include "data_types/date_time.h"
#include "data_types/diode_matrix.h"
#include "seq/matrix_seq.h"

#include "tests.h"



/*
 *
 *		Zmienne
 *
 */

//! podstawa odniesienia czasu w milisekundach, [0 - 1000]
volatile uint16_t ui16Ms = 0;
//! podstawa odniesienia czasu w sekundach, w kooperacji z ui16Ms
volatile uint8_t uiSeconds;
//! matryca LED
volatile DiodeMatrix matrix;
//! przekaznik
volatile Relay relay;
//! floaga nowej sekundy, na podstawie przerwania SQW od DS3231
volatile bool bNewTime = false;
//! flaga nowego taktu odswiezenia bufora obrotowego dla czasu
volatile bool bNewRoundRefresh = false;
//! dane ADC o jasnosci
volatile ADCVoltageData adc;
//! czas pobierany z RTC
TimeDate RTCTime;
//! czas wyswietlany na matrycy
//! @see 	LoadTimeToMatrix
TimeDate actTime;
//! flaga zmiany poziomu Y
volatile uint8_t uivModifyY;
//! okresla ilosc powtorzen wyswietlania tekstu na matrycy
volatile int8_t iCountToTimer;
//! flaga zezwolenia na dekrementacje czyszczonej pozycji w trybie czyszczenia
volatile bool bEnableDecrement;

/*
 *
 *		Przerwania
 *
 */

// ISR(TIMER0_COMPA_vect)
// ISR(TIMER2_COMPA_vect)
// ISR(ADC_vect)
// ISR(PCINT1_vect)

/*
 *
 *		Funkcja glowna
 *
 */

int main (void) {
	/*
	 *
	 *		Blok inicjalizacji
	 *
	 */

	RegistersInit();
	DiodeMatrixInit(&matrix);
	RelayInit();
	Timer0Init();
	Timer2Init();
	PCINTInit();

	ADCInit(&adc);
	USART_Init(__UBRR);
	I2C_Init();

	DS3231_Init();
	SetSeqParams(&matrix, &actTime, &relay);
	RelayStartClicking(&relay, 0, RelayDataNumber);

	wdt_enable(WDTO_60MS);
	DS3231_GetTime(&RTCTime.uiHours, &RTCTime.uiMinutes, &RTCTime.uiSeconds);
	DS3231_GetDate(&RTCTime.uiDays, &RTCTime.uiMonths, &RTCTime.uiYears);
	LoadToSingleTime(&RTCTime);

	sei();

	/*
	 *
	 *		Petla glowna
	 *
	 */

	while(1) {
		wdt_reset();
		// proba obslugi komendy UART
		TryLoadCommand(&matrix, &relay, &RTCTime);
		// oczyszczanie bufora, po osiagnieciu poczatku uruchomienie sekwencji zawartej w eACtualSeq
		if (bEnableDecrement) {
			if (DecrementTo0SlowClear(&matrix)) {
				SetSeqParams(&matrix, &actTime, &relay);
				iCountToTimer = 2;

			}
			bEnableDecrement = false;
		}

		// Jesli tryb czyszczenia bufora nieaktywny
		if (matrix.uiSlowClearedPos == 0) {
			switch(eActualSeq) {
				// tryb zegara
				case SeqTimer: {
					if (bNewTime) {
						DS3231_GetTime(&RTCTime.uiHours, &RTCTime.uiMinutes, &RTCTime.uiSeconds);
						LoadToSingleTime(&RTCTime);

						if ((RTCTime.uiSeconds % 10) == 0) {
							sprintf(ctTextBuffer, "%02d-%02d-2%03d %02d:%02d:%02d\n", RTCTime.uiDays,
								RTCTime.uiMonths, RTCTime.uiYears, RTCTime.uiHours,
								RTCTime.uiMinutes, RTCTime.uiSeconds);
							uart_puts(ctTextBuffer);
						}
						// wyswietlenie daty co 5 minut w 10 sekundzie
						if ((RTCTime.uiSeconds == 10) && ((RTCTime.uiMinutes % 3) == 1)) {
							sprintf(ctTextBuffer, "%02d-%02d-2%03d", RTCTime.uiDays, RTCTime.uiMonths, RTCTime.uiYears);
							eActualSeq = SeqText;
							RunSlowClearedPos(&matrix);
						}

						// aktywacja przekaxnika i zaladowanjie daty gdy polnoc
						if (RTCTime.uiSeconds == 0) {
							ResetProgress(&actTime);
							if (RTCTime.uiMinutes == 0) {
								RelayStartClicking(&relay, RTCTime.uiHours, RelayDataHours);
								if (RTCTime.uiHours == 0)
									DS3231_GetDate(&RTCTime.uiDays, &RTCTime.uiMonths, &RTCTime.uiYears);
							}
							else
								if ((RTCTime.uiMinutes % 15) == 0)
									RelayStartClicking(&relay, RTCTime.uiMinutes, RelayDataMinutes);
						}
						bNewTime = false;
					}

					if (bNewRoundRefresh){
						LoadTimeToMatrix(&matrix, &actTime, &RTCTime);
						bNewRoundRefresh = false;
					}
				} break;
				case SeqADC: {
					// wypisanie ADC do matrycy i UART
					if (bNewTime) {
						DS3231_GetTime(&RTCTime.uiHours, &RTCTime.uiMinutes, &RTCTime.uiSeconds);
						LoadNumberToMatrix(&matrix, adc.ui16PhotoAvg);
						if ((RTCTime.uiSeconds % 10) == 0) {
							uart_puts_p(PSTR("Brightness: "));
							ctTextBuffer[4] = '\n';
							uart_puts(ctTextBuffer);
						}
						bNewTime = false;
					}
				} break;
				// zaladowanie bufora podczas SetSeqParams
				case SeqText: {} break;
				// wyslanie liczby do przekaznika
				case SeqRelayNumber: {} break;
				case SeqEmpty: {} break;
				// kolejne case'y
			}

		}
	}
} // END int main (void)

//! timer pracy matrycy LED, konieczna optymalizacja
ISR(TIMER0_COMPA_vect) {
	// obsluga zmiany jasnosci
	uivModifyY = IncrementBrightness(&matrix);
	if (uivModifyY) {
		SendRegisterY(ReturnYValue(&matrix), true);
		if (matrix.uiBrightness) {
			RefreshBufferFlag(&matrix);
			SendRegistersX(matrix.uitBufferFlag, true);
		}
	} else if (matrix.uiBrightCount == matrix.uiBrightness)
		ClearRegistersX(true);
} // END ISR(TIMER0_COMPA_vect)

//! przerwanie timera w trybie CTC, odniesieni czasowe 1 MS
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	if (++ui16Ms >= 1000) {
		ui16Ms = 0;
		if (++uiSeconds >= 60)
			uiSeconds = 0;
	}
	// ustawienie flagi nowego taktu odswiezenia bufora obrotowego
	if ((ui16Ms % TIME_DECREMENT_MS) == 0)
		bNewRoundRefresh = true;
	// zmiana w pozycji bufora i powrot do SeqTimer w przypadku wyswietlania tekstu
	if ((ui16Ms % INC_POS_MS) == 0)
		if (IncrementBufferPosition(&matrix)) {
			if ((eActualSeq == SeqText) && (matrix.uiSlowClearedPos == 0) && ((--iCountToTimer) == 0) ) {
				eActualSeq = SeqTimer;
				SetSeqParams(&matrix, &actTime, &relay);
			}
		}
	if ((ui16Ms % 30) == 15)
		bEnableDecrement = true;
} // END ISR(TIMER2_COMPA_vect)

//! przerwanie zakonczenia pomiaru ADC, odczytanie zawartosci ADC
ISR(ADC_vect) {
	ReadADCToADCData(&adc, &matrix.uiBrightness);
}


//! przerwanie sygnalu SQW z timera DS3231, ustawienie flagi nowego czasu
ISR(PCINT1_vect) {
	ADCStart();
	if (SQW_IS_HIGH()) {
		bNewTime = true;
	}
} // END ISR(PCINT1_vect)

//! przerwanie stanu Bluetooth, inicjalizacja UART gdy uruchomiono Bluetooth
ISR(PCINT2_vect) {
	if (BLUETOOTH_IS_ON()) {
		USART_Init(__UBRR);
	}
} // ISR(PCINT2_vect)


