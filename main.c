/*!
 * @file main.h
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

volatile uint16_t ui16Ms;
uint8_t ui16MsLastRefresh;
volatile uint8_t uiSeconds;
volatile DiodeMatrix matrix;
volatile Relay relay;
volatile bool bNewTime;
//volatile ADCVoltageData adcData;
Time RTCTime;
Time actTime;
//Date mainDate;
//ADCData mainADC;

// flaga zmiany pozycji Y
volatile uint8_t uivModifyY;

//volatile bool bLoadTime;


/*
 *
 *		Deklaracje funkcji
 *
 */

// ISR(TIMER0_COMPA_vect)
// ISR(TIMER2_COMPA_vect)
// ISR(ADC_vect)

/*
 *
 *		Pêtla g³ówna
 *
 */


int main (void) {
	/*
	 *
	 *		Blok inicjalizacji
	 *
	 */
	DiodeMatrixInit(&matrix);
	RelayInit();
	RegistersInit();
	Timer0Init();
	Timer2Init();
	PCINTInit();

	I2C_Init();
	DS3231_Init();
	DS3231_Test();
	wdt_enable(WDTO_2S);
	LoadTextToMatrix(&matrix, "00 00", gamma_o[1]);
	sei();
	while(1) {
		if (bNewTime) {
			DS3231_GetTime(&RTCTime.uiHours, &RTCTime.uiMinutes, &RTCTime.uiSeconds);
			wdt_reset();
			LoadToSingleTime(&RTCTime);
			// aktywacja przekaxnika
			if (RTCTime.uiSeconds == 0) {
				ResetProgress(&actTime);
				if (RTCTime.uiMinutes == 0)
					RelayStartClicking(&relay, RTCTime.uiHours, false);
				else
					if ((RTCTime.uiMinutes % 15) == 0)
						RelayStartClicking(&relay, RTCTime.uiMinutes, true);
			}
			ui16Ms = 0;
			bNewTime = false;
		}
		if (((ui16Ms % TIME_DECREMENT_MS) == 0) && (ui16Ms != ui16MsLastRefresh)){
			ui16MsLastRefresh = ui16Ms;
			LoadTimeToMatrix(&matrix, &actTime, &RTCTime, gamma_o[1]);
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


}

//! przerwanie timera w trybie CTC, odniesieni czasowe 1 MS
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	if (++ui16Ms >= 10000) {
		ui16Ms = 0;
		if (++uiSeconds >= 60)
			uiSeconds = 0;

		//SecondsBinary(&matrix, uiSeconds, gamma_o[2]);
	}

	if ((ui16Ms % INC_POS_MS) == 0)
		IncrementBufferPosition(&matrix);


}
/*
//! przerwanie zakonczenia pomiaru ADC
ISR(ADC_vect) {
	//ReadADCToADCData(&adcData);
}

// przerwania przelacznikow, flagi nowej sekundy bLoadTime
ISR(PCINT0_vect) {

}
*/

ISR(PCINT1_vect) {
	if (SQW_IS_HIGH()) {
		//actTime.uiSeconds++;
		bNewTime = true;
	}
}
/*
ISR(PCINT2_vect) {

}
*/
