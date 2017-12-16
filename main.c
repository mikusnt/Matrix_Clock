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
#include "data_types/date_time.h"
#include "devices/register.h"
#include "tests.h"



/*
 *
 *		Typy danych
 *
 */

//volatile uint16_t ui16Ms;
//DiodeMatrix matrix;
volatile Relay relay;
volatile ADCVoltageData adcData;
//Time RTCTime;
//Time actTime;
//Date mainDate;
//ADCData mainADC;

// flaga zmiany pozycji Y
//volatile uint8_t uivModifyY;
volatile bool bLoadTime;


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
//	DiodeMatrixInit(&matrix);
	RelayInit();
	RegistersInit();
	ADCVoltageDataInit(&adcData);
	//loadToBuffer(0);
	//SendRegisterX(tabl, true);
//	Timer0Init();
	Timer2Init();

	//IncrementBrightness(&matryca);
	//IncrementX(&matrix);
	//ReturnBufferFlag(&matryca, 0, 0);
	//ReturnXValue(&matrix);
	//RelayStartClicking(&relay, 15, false);
	//RefreshBufferFlag(&matrix);
	SendRegisterX(tabl, true);
//	PYRegisterSW(1);
	//DDRD |= 1 << PD7;
	//
	//PYRegisterSW(HIGH);
	/*
	 *
	 *		Petla glowna
	 *
	 */
	sei();
	while(1) {
		Test_Relay_Minutes_X0(&relay);
		Test_Relay_Hours_X0(&relay);

		//Test_Y(&i);
	}
} // END int main (void)

//! timer pracy matrycy LED
ISR(TIMER0_COMPA_vect) {
//	// obsluga zmiany jasnosci
//	uivModifyY = IncrementBrightness(&matrix);
//	RefreshBufferFlag(&matrix);
//	if (uivModifyY) PYRegisterSW(0);
//	if (matrix.bModifyFlag) SendRegistersX(ReturnOneYBufferFlag(&matrix));
//	if (uivModifyY) {
//		SendRegisterY(ReturnYValue(&matrix));
//		PYRegisterSW(1);
//	}
}

//! przerwanie timera w trybie CTC, odniesieni czasowe 1 MS
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
}

//! przerwanie zakonczenia pomiaru ADC
ISR(ADC_vect) {
	ReadADCToADCData(&adcData);
}

// przerwania przelacznikow, flagi nowej sekundy bLoadTime
ISR(PCINT0_vect) {

}

ISR(PCINT1_vect) {

}

ISR(PCINT2_vect) {

}
