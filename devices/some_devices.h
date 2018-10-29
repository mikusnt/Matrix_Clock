/*!
 * @file some_devices.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-29 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Used uC pins: 6<br>
 * Header file containing configuration of timers, ADC in single mode to photoresistor brightness lvl.
 * Turning on ADC by manual action.
 */

#ifndef SOME_DEVICES_H_
#define SOME_DEVICES_H_

#include "../group_includes.h"
#include "../data_types/diode_matrix.h"

/*
 *
 *		Macros
 *
 */

//! number of adc measurement to average
#define ADC_READ_COUNT 2
//! photoresistor address
#define ADC_PHOTO (1 << PC2)
//! ADC channel mask
#define ADMUX_MASK 0xF0

//! direction register of bluetooth power supply
#define BLUETOOTH_DDR DDRD
//! state register of bluetooth power supply
#define BLUETOOTH_PORT PORTD
//! value register of bluetooth power supply
#define BLUETOOTH_PIN PIND
//! address of bluetooth power supply
#define BLUETOOTH_ADDR (1 << PD7)
//! checks state of bluetooth power supply
#define BLUETOOTH_IS_ON() (!(BLUETOOTH_PIN & BLUETOOTH_ADDR))


#define SQW_DDR DDRC
#define SQW_PORT PORTC
#define SQW_PIN PINC
#define SQW_ADDR (1 << PC1)
#define SQW_IS_HIGH() (SQW_PIN & SQW_ADDR)

#define ADC_PHOTO_ADR 2

#define PWM_MATRIX_OVF 3
#define PWM_MATRIX_DDR DDRD
#define PWM_MATRIX_PORT PORTD
#define PWM_MATRIX_ADDR (1 << PD5)
#define PWM_MATRIX_LOW() PWM_MATRIX_PORT &= ~PWM_MATRIX_ADDR
#define PWM_MATRIX_HIGH() PWM_MATRIX_PORT |= PWM_MATRIX_ADDR


/*
 *
 *		Main data types
 *
 */

//! Main ADC structure
typedef struct {
	//! actual number of measurements
	uint8_t uiActReadCount;
	//! actual sum of photoresistor measurements
	uint16_t ui16PhotoSum;
	//! average ADC value of photoresistor
	uint16_t ui16PhotoAvg;
} ADCVoltageData;

//! 0 or 1 to hystheresis on ReadADCToADCData
//! @see ReadADCToADCData
extern uint8_t uiGammaShift;
//! incremented value of pwm brightness output to BRIGHTNESS_OVF value
extern volatile uint8_t uivBrightCount;

/*
 *
 *		Declaration of functions
 *
 */

//! Initialize Timer0 to matrix refresh with bright pin
extern void Timer0Init();
//! Initialize Timer1 to pwm matrix bright
extern void Timer1Init();
//! Initialize Timer2 to cunting time between DS3231 interrupts
extern void Timer2Init();
//! Initialize ADC structure
extern void ADCInit(volatile ADCVoltageData *a);
//! run new ADC measurement
inline void ADCStart();
//! update ADC structure and refresh brightness, should called in end ADC measurement interrupt
inline void ReadADCToADCData(volatile ADCVoltageData *a, volatile uint8_t *bright_s, volatile uint8_t *pwmBright_s);
//! Initialize PCINT interrupt
extern void PCINTInit();
//! Power reduction
extern void PowerReduction();


/*
 *
 *		Definitions of inline functions
 *
 */

/*! required to load new measurement, should be triggered after ReadADCToADCData or channel change */
inline void ADCStart() {
	ADCSRA |= (1 << ADSC); // uruchmienie konwersji
}

/*!@param 		a pointer of ADC structure
 * @param		bright_s output brightness
 * @param		pwmBright_s output PWM brightness
 * @see ADCStart()*/
inline void ReadADCToADCData(volatile ADCVoltageData *a, volatile uint8_t *bright_s,
		volatile uint8_t *pwmBright_s) {
	// new part of sum
	a->ui16PhotoSum += ADC;

	// when done all measurements to average
	if (++a->uiActReadCount >= ADC_READ_COUNT) {
		a->uiActReadCount = 0;
		a->ui16PhotoAvg = a->ui16PhotoSum / ADC_READ_COUNT;
		a->ui16PhotoSum = 0;
		HystData hystData = {50, 20, 0, 1, a->ui16PhotoAvg % 100, &uiGammaShift};
		Hysteresis(&hystData);
		uint8_t addr = uiGammaShift + (a->ui16PhotoAvg / 100);
		*bright_s = gamma_o[addr];
		//*pwmBright_s = gamma_o[addr] % 10;
	}
} // END inline void ReadADCToADCData

#endif /* SOME_DEVICES_H_ */
