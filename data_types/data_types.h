/*!
 * @file data_types.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-31 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Header file containing universal macros and data types.
 */

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#include <util/delay.h>
#include <stdlib.h>
#include <avr/wdt.h>

/* Variables name format:
 * 		1. type shortcut
 * 		2. name
 * 		3. pointer optional parameters:
 * 			_e - edit // default in pointers
 * 			_s - set
 */

/*
 *
 *		Macros
 *
 */

//! uC delay in seconds
#define D_S(x) _delay_ms(x*1000)
//! uC delay in milliseconds
#define D_MS(x) _delay_ms(x)
//! uC delay in microseconds
#define D_US(x) _delay_us(x)

/*
 *
 *		Universal data types
 *
 */

//! 8 one bit flags, direct access into bits
/*! @see ByteFlag*/
typedef struct {
	uint8_t b0 : 1; 	//!< bit 0
	uint8_t b1 : 1; 	//!< bit 1
	uint8_t b2 : 1;		//!< bit 2
	uint8_t b3 : 1;		//!< bit 3
	uint8_t b4 : 1;		//!< bit 4
	uint8_t b5 : 1;		//!< bit 5
	uint8_t b6 : 1;		//!< bit 6
	uint8_t b7 : 1;		//!< bit 7
} Flag;

//! union between one byte and bit flags
typedef union {
	uint8_t value;		//!< data byte
	Flag flag;			//!< bit flag
} ByteFlag;

//! information about device state
typedef enum {
	OFF = 0, 			//!< deactivated
	ON = 1				//!< activated
} BinarySwitch;

//! information about logic state
typedef enum {
	LOW = 0, 			//!< slow state
	HIGH = 1			//!< high state
} LogicState;

//! arguments of hystheresis
typedef struct {
	uint16_t ui16Threshold;	//!< center point value (of input)
	uint8_t uiDelta;		//!< delta
	uint8_t uiMinValue;		//!< minimum output value
	uint8_t uiMaxValue;		//!< maximum output value
	uint16_t ui16Value;		//!< input value
	volatile uint8_t *uiOutValue_s;	//!< output value (min or max)
} HystData;

/*! @param			sHistData input parameters and output*/
inline void Hysteresis(HystData *sHistData) {
	if (sHistData->ui16Value < (sHistData->ui16Threshold - sHistData->uiDelta))
		*(sHistData->uiOutValue_s) = sHistData->uiMinValue;
	if (sHistData->ui16Value > (sHistData->ui16Threshold + sHistData->uiDelta))
			*(sHistData->uiOutValue_s) = sHistData->uiMaxValue;
} // END inline void Hysteresis

//! reset uC
inline void Reset_UC() {
	while(1){
		wdt_enable(WDTO_15MS);
		while(1);
	}
} // END inline void Reset_UC

#endif /* DATA_TYPES_H_ */
