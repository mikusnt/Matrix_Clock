/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-12-07 $
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
 *		Typy danych
 *
 */

//! podstawa odniesienia czasu w milisekundach, [0 - 1000]
volatile uint16_t ui16Ms;
//! czas w milisekundach ostatniego odswiezenia LoadTimeToMatrix, przyjmuje wartosci ui16Ms
volatile uint16_t ui16MsLastRefresh = 10000;
//! podstawa odniesienia czasu w sekundach, w kooperacji z ui16Ms
volatile uint8_t uiSeconds;
//! matryca LED
volatile DiodeMatrix matrix;
//! przekaznik
volatile Relay relay;
//! floaga nowej sekundy, na podstawie przerwania SQW od DS3231
volatile bool bNewTime;
//! dane ADC o jasnosci
volatile ADCVoltageData adcVD;
//! czas pobierany z RTC
Time RTCTime;
//! czas wyswietlany na matrycy
//! @see 	LoadTimeToMatrix
Time actTime;
//! flaga zmiany poziomu Y
volatile uint8_t uivModifyY;
//! aktualnie uruchomiona sekweencja
//! @see SetSeqParams
ActualSeq eActualSeq = SeqTimer;


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

	ADCInit(&adcVD);
	SetSeqParams(eActualSeq, &matrix, &adcVD);
	TimeInit(&actTime);
	sei();

	I2C_Init();
	DS3231_Init();
	//RelayStartClicking(&relay, 12, RelayDataHours);
	wdt_enable(WDTO_2S);

	/*
	 *
	 *		Petla glowna
	 *
	 */
	while(1) {
		// zmiana jasnosci
		if (adcVD.bNewBright) {
			SetBrightness(&matrix, adcVD.uiActBright);
			adcVD.bNewBright = false;
		}
		switch(eActualSeq) {
			// tryb zegara
			case SeqTimer: {
				if (bNewTime) {

					DS3231_GetTime(&RTCTime.uiHours, &RTCTime.uiMinutes, &RTCTime.uiSeconds);
					wdt_reset();
					LoadToSingleTime(&RTCTime);
					// aktywacja przekaxnika
					if (RTCTime.uiSeconds == 0) {
						ResetProgress(&actTime);
						if (RTCTime.uiMinutes == 0)
							RelayStartClicking(&relay, RTCTime.uiHours, RelayDataHours);
						else
							if ((RTCTime.uiMinutes % 15) == 0)
								RelayStartClicking(&relay, RTCTime.uiMinutes, RelayDataMinutes);
					}
					bNewTime = false;
				}

				if ((ui16MsLastRefresh < 1000) && ((ui16Ms % TIME_DECREMENT_MS) == 0) && (ui16Ms != ui16MsLastRefresh)){
					ui16MsLastRefresh = ui16Ms;
					LoadTimeToMatrix(&matrix, &actTime, &RTCTime, adcVD.uiActBright);
				}
			} break;
			case SeqText: {

			} break;
			// kolejne case'y
		}
	}
} // END int main (void)

//! timer pracy matrycy LED
ISR(TIMER0_COMPA_vect) {
	// obsluga zmiany jasnosci
	uivModifyY = IncrementBrightness(&matrix);
	RefreshBufferFlag(&matrix);
	if (matrix.bModifyFlag) {
		if (uivModifyY) {
			SendRegistersX(matrix.etBufferFlag, true, false);
			SendRegisterY(ReturnYValue(&matrix), true);
		}
		SendRegistersX(matrix.etBufferFlag, true, true);
	}
} // END ISR(TIMER0_COMPA_vect)

//! przerwanie timera w trybie CTC, odniesieni czasowe 1 MS
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	if (++ui16Ms >= 1000) {
		ui16Ms = 0;
		if (++uiSeconds >= 60)
			uiSeconds = 0;

		//SecondsBinary(&matrix, uiSeconds, gamma_o[2]);
	}

	if ((ui16Ms % INC_POS_MS) == 0)
		IncrementBufferPosition(&matrix);
} // END ISR(TIMER2_COMPA_vect)

//! przerwanie zakonczenia pomiaru ADC
ISR(ADC_vect) {
	ReadADCToADCData(&adcVD);
}


//! przerwanie sygnalu SQW z timera DS3231
ISR(PCINT1_vect) {
	ADCStart();
	if (SQW_IS_HIGH()) {
		ui16MsLastRefresh = 0;
		bNewTime = true;
	}
} // END ISR(PCINT1_vect)

