/*!
 * @file some_devices.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see other_devices.h*/

#include "some_devices.h"

uint8_t uiGammaShift;
volatile uint8_t uivBrightCount;

/*! interrupt period is 1250 cycles*/
void Timer0Init() {
	// CTC mode
	TCCR0A |= (1 << WGM01);
	// prescaler 8
	TCCR0B |= (1 << CS01);
	// unlock interrupt
	TIMSK0 |= (1 << OCIE0A);
	OCR0A = 156;
} // END void Timer0Init

//! 1ms period
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


/*! @param		a pointer of ADC structure*/
void ADCInit(volatile ADCVoltageData *a) {
	// default Vref equals power supply
	// default one measurement mode
	ADCSRA |= (1 << ADEN) // activate ADC
			| (1 << ADPS2)
			| (1 << ADPS1)
			| (1 << ADPS0) // prescaler 128
			| (1 << ADIE); // uunlock interrupts
	// 10bit resolution
	//ADMUX |= (1 << ADLAR); // 8 bit resolution
	// internal 1.1V source
	ADMUX |= (1 << REFS0) | (1 << REFS1);
	// set ADC channel
	ADMUX = (ADMUX & ADMUX_MASK) | ADC_PHOTO_ADR;
} // END void ADCInit

void PCINTInit() {
	SQW_DDR &= ~SQW_ADDR;
	BLUETOOTH_DDR &= ~BLUETOOTH_ADDR;
	BLUETOOTH_PORT |= BLUETOOTH_ADDR;

	PCICR |= (1 << PCIE1) | (1 << PCIE2); // unlock interrupt group 8-14 and 16-23
	PCMSK1 |= (1 << PCINT9); // SQW
	PCMSK2 |= (1 << PCINT23); // Bluetooth
} // END void PCINTInit()

void PowerReduction() {
	PRR |= (1 << PRTIM1); // turn off Timer1
}
