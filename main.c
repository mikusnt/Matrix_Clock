/*
 * main.c
 *
 *  Created on: 3 lut 2016
 *      Author: MS-2
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

//DiodeMatrix matrix;
volatile Relay relay;
//Time RTCTime;
//Time actTime;
//Date mainDate;
//ADCData mainADC;

// flaga zmiany pozycji Y
//volatile uint8_t uivModifyY;


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
	uint8_t i = 0, value = 1;
	uint16_t j = 0;
//	DiodeMatrixInit(&matrix);
	RelayInit();
	RegistersInit();
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
	sei();

	while(1) {
		Test_Relay_X0(&relay, &i);
		//Test_Y(&i);
	}
}

// timer pracy matrycy LED
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

// timer odniesienia czasowego 1 MS
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
}

// przerwanie pomiaru ADC
ISR(ADC_vect) {

}

ISR(PCINT0_vect) {

}

ISR(PCINT1_vect) {

}

ISR(PCINT2_vect) {

}
