/*
 * io_devices.c
 *
 *  Created on: 4 lis 2017
 *      Author: MS
 */

#include "other_devices.h"

void Timer0Init() {
	// tryb CTC
	TCCR0A |= (1 << WGM01);
	// preskaler 8
	TCCR0B |= (1 << CS01);
	// odblokowanie przerwan
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 157;
} // END void Timer0Init

void Timer2Init() {
	// tryb CTC
	TCCR2A |= (1 << WGM21);
	// preskaler 256
	TCCR2B |= (1 << CS22) | (1 < CS21);
	// odblokowanie przerwan
	TIMSK2 |= (1 << OCIE2A);
	OCR2A = 78;
} // END void Timer2Init

/*! konfiguruje rejestr ladowania, rejestry przetwornika ADC oraz inicjalizuje strukture ADCData
 * @param		a adres struktury przetwornika ADC*/
void ADCVoltageDataInit(ADCVoltageData *a) {
	CHARGE_DDR |= CHARGE_ADDR;
	// domyslnie Vref rowne zasilaniu, niezaleznie od pinu Aref; ADMUX
	// domyslnie tryb pojedynczego odczytu
	ADCSRA |= (1 << ADEN) // aktywacja ADC
			| (1 << ADPS2)
			| (1 << ADPS1)
			| (1 << ADPS0) // preskaler 128
			| (1 << ADIE); // uruchomienie przerwan
	// rozdzielczosc 10bit
	//ADMUX |= (1 << ADLAR); // rozdzieczosc 8 bitowa
	a->uiActBright = MAX_MATRIX_BRIGHT;
	SetADCChannel(a);
} // END void ADCInit

/*! ustawia rejestr przyciskow na wejscie z podciagnieciem do zasilania i konfiguruje przerwania
 * sprzetowe PCINT*/
void ButtonsInit() {
	BUTTON_LEFT_PORT |= BUTTON_LEFT_ADDR;
	BUTTON_RIGHT_PORT |= BUTTON_RIGHT_ADDR;
	BUTTON_CENTER_PORT |= BUTTON_CENTER_ADDR;

	PCICR |= (1 << PCIE2)
			| (1 << PCIE1)
			| (1 << PCIE0); // odblokowanie wszystkich grup przerwan PCINT
	PCMSK2 |= 0; // nalezy ustawic odpowiednie bity mask
	PCMSK1 |= 0; // nalezy ustawic odpowiednie bity mask
	PCMSK0 |= 0; // nalezy ustawic odpowiednie bity mask
}
