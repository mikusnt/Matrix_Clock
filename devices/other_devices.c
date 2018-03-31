/*!
 * @file other_devices.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see other_devices.h*/

#include "other_devices.h"

/*! konfiguracja 8-bitowego timera0 jako licznika dla matrycy LED, przerwanie co 1250 taktot zegara*/
void Timer0Init() {
	// tryb CTC
	TCCR0A |= (1 << WGM01);
	// preskaler 8
	TCCR0B |= (1 << CS01) | (1 << CS00);
	// odblokowanie przerwan
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 33; // 33
} // END void Timer0Init

//! konfiguracja 8-bitowego timera2 jako podstawowego odniesienia czasu 1 ms
void Timer2Init() {
	// tryb CTC
	TCCR2A |= (1 << WGM21);
	// preskaler 256
	TCCR2B |= (1 << CS22) | (1 << CS21);
	// odblokowanie przerwan
	TIMSK2 |= (1 << OCIE2A);
	OCR2A = 78; //dla 20 MHz
	//OCR2A = 39; // 8 MHz
} // END void Timer2Init

/*! konfiguruje rejestr ladowania, rejestry przetwornika ADC oraz inicjalizuje strukture ADCData
 * @param		a adres struktury przetwornika ADC*/
void ADCInit(volatile ADCVoltageData *a) {
	// domyslnie Vref rowne zasilaniu, niezaleznie od pinu Aref; ADMUX
	// domyslnie tryb pojedynczego odczytu
	ADCSRA |= (1 << ADEN) // aktywacja ADC
			| (1 << ADPS2)
			| (1 << ADPS1)
			| (1 << ADPS0) // preskaler 128
			| (1 << ADIE); // uruchomienie przerwan
	// rozdzielczosc 10bit
	//ADMUX |= (1 << ADLAR); // rozdzieczosc 8 bitowa
	a->uiActBright = gamma_o[1];
	// zrodlo wewnetrzne 1.1V
	ADMUX |= (1 << REFS0) | (1 << REFS1);
	// ustawienie kanalu ADC
	ADMUX = (ADMUX & ADMUX_MASK) | ADC_PHOTO_ADR;

} // END void ADCInit

/*! ustawia rejestr przyciskow na wejscie z podciagnieciem do zasilania i konfiguruje przerwania
 * sprzetowe PCINT*/
void PCINTInit() {
	SQW_DDR &= ~SQW_ADDR;
	BLUETOOTH_DDR &= ~BLUETOOTH_ADDR;
	BLUETOOTH_PORT |= BLUETOOTH_ADDR;

	PCICR |= (1 << PCIE1) | (1 << PCIE2); // odblokowanie przerwan od nr 8-14 i 16-23
	PCMSK1 |= (1 << PCINT9); // SQW
	PCMSK2 |= (1 << PCINT23); // Bluetooth
}
