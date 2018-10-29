/*!
 * @file diode_matrix.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-29 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Used uC pins: 0<br>
 * Header file containing logic structure of LED Matrix to display data.
 * @see		register.h
 */

#ifndef DIODE_MATRIX_H_
#define DIODE_MATRIX_H_

#include "../group_includes.h"

/*
 *
 *		Macros
 *
 */

//! Y size (vertical)
#define MATRIX_Y_SIZE 8
//! X size (horizontal)
#define MATRIX_X_SIZE 32
//! X size of hardware matrix buffer
#define BUFFER_X_SIZE 32 * 8
//! max number of brightness
#define MAX_MATRIX_BRIGHTNESS 19
//! max brightness in gamma correction table
//! @see gamma_o
#define MAX_GAMMA_BRIGHTNESS 10
//! time period to increment moving adress of buffer
#define INC_POS_MS 100

//! orange brightness levels from ACD brightness / 100
extern const uint8_t gamma_o[MAX_GAMMA_BRIGHTNESS+1];

/*
 *
 *		Main data types
 *
 */

//! main structure of LED Matrix
typedef struct {
	//! buffer of LED state, one byte for all Y positions (vertical)
	uint8_t uitBufferX[BUFFER_X_SIZE];
	//! actual bright level of matrix, modify by function
	//! @see IncrementBrightness
	uint8_t uiBrightCount;
	//! actual Y position (vertical), modify by function
	//! @see IncrementY
	uint8_t uiActY;
	//! buffer of LED state in actual Y position
	uint8_t uitBufferFlag[MATRIX_X_SIZE];
	//! actual matrix buffer start adress, modify by function
	//! @see IncrementBufferPosition
	int16_t i16BufferPosition;
	//! matrix buffer end address, modify manually
	uint8_t uiEndBufferPosition;
	//! mode of displaying, modify manually
	//! true - display clear matrix, increment start address to end buffer position
	//! false - display first part of buffer and increment address to see end part of buffer
	bool bIsMoving;
	//! two Y levels of round buffer
	//! first dim - [0] - visible, [1] - under visible matrix
	//! second dim - max height of letter
	uint8_t uitRoundBufferYX[2][5];
	//! position of right to left clearing line, set by
	//! @see 	RunSlowClearedPos
	//! @see 	DecrementTo0SlowClear
	uint8_t uiSlowClearedPos;
	//! brightness of matrix, modify manually
	uint8_t uiBrightness;
	//! actual PWM bright level of matrix, modify by interrupt
	//! @see 	TIMER1_COMPA_vect
	uint8_t uiPWMBrightnessCount;
	//! PWM brightness of matrix, modify manually
	uint8_t uiPWMBrightness;
} DiodeMatrix;

/*
 *
 *		Declaration of functions
 *
 */

//! increment actual matrix brightness
inline volatile bool IncrementBrightness(volatile DiodeMatrix *m);
//! increment Y position
inline void IncrementY(volatile DiodeMatrix *m);
//! return serial value to Y register from actual Y position
inline volatile BinarySwitch ReturnYValue(volatile DiodeMatrix *m);
//! initialize DiodeMNatrix structure
inline void DiodeMatrixInit(volatile DiodeMatrix *m);
//! refresh flag buffer of one Y position from uitBufferX data
inline void RefreshBufferFlag(volatile DiodeMatrix *m);
//! increment actual buffer start address
inline bool IncrementBufferStart(volatile DiodeMatrix *m);
//! set mode of matrix displaying, reset buffer start position to 0
extern void SetMoving(volatile DiodeMatrix *m, bool isMoving);
//! load to one y position of matrix buffer logic value
extern void SetYBuffer(volatile DiodeMatrix *m, uint8_t y_pos, BinarySwitch value);
//! clear all parts of matrix buffer
extern void ClearBuffer(volatile DiodeMatrix *m);
//! copy data fragment from round buffer to matrix buffer
extern void CopyFromRoundToBuffer(volatile DiodeMatrix *m, uint8_t y_round_pos, uint8_t x_buffer_pos);
//! decrement right to left clearing line
inline bool DecrementTo0SlowClear(volatile DiodeMatrix *m);
//! set right to left clearing line to matrix buffer end address
inline void RunSlowClearedPos(volatile DiodeMatrix *m);

/*
 *
 * Definitions of functions
 *
 */


/*! @param			m pointer of DiodeMatrix structure
 *  @return			1 when Y matrix position was incremented, 0 otherwise*/
inline volatile bool IncrementBrightness(volatile DiodeMatrix *m){
	if (++m->uiBrightCount > MAX_MATRIX_BRIGHTNESS) {
		m->uiBrightCount = 0;
		IncrementY(m);
		return 1;
	}
	return 0;
}

/*! @param			m pointer of DiodeMatrix structure*/
inline void IncrementY(volatile DiodeMatrix *m){
	if (++m->uiActY >= MATRIX_Y_SIZE) m->uiActY = 0;
}

/*! @param			m pointer of DiodeMatrix structure
 *  @return 		serial value to Y register from actual Y position*/
inline volatile BinarySwitch ReturnYValue(volatile DiodeMatrix *m) {
	return (m->uiActY) ? 0 : m->uiBrightness;
}

/*! @param			m pointer of DiodeMatrix structure*/
inline void DiodeMatrixInit(volatile DiodeMatrix *m) {
	uint16_t i;
	for (i = 0; i < BUFFER_X_SIZE; i++)
		m->uitBufferX[i] = 0;
	m->uiBrightCount = 0;
	m->uiActY = 0;
	m->uiBrightness = 0;
	m->i16BufferPosition = 0;
	m->uiEndBufferPosition = MATRIX_X_SIZE;
	m->uiBrightness = gamma_o[1];

} // END inline void DiodeMatrixInit

/*! @param			m pointer of DiodeMatrix structure*/
inline void RefreshBufferFlag(volatile DiodeMatrix *m) {
	// fast calculations
	register uint8_t i;
	register const uint8_t y_mask = 1 << m->uiActY;
	register int16_t adr;
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		adr = i+m->i16BufferPosition;
		m->uitBufferFlag[i] = 0;
		if ((adr >= 0) && (adr < BUFFER_X_SIZE)) {
			m->uitBufferFlag[i] = m->uitBufferX[adr] & y_mask;
		} else
			m->uitBufferFlag[i] = 0;
	}
} // END inline void RefreshBufferFlag

/*! @param			m pointer of DiodeMatrix structure
 *  @return			true jesli zresetowano do poczatku*/
inline bool IncrementBufferStart(volatile DiodeMatrix *m) {
	if (m->bIsMoving) {
		if (++m->i16BufferPosition > m->uiEndBufferPosition){
			m->i16BufferPosition = -MATRIX_X_SIZE;
			return true;
		}
		return false;
	} else {
		if (++m->i16BufferPosition > (m->uiEndBufferPosition - MATRIX_X_SIZE)) {
			m->i16BufferPosition = 0;
			return true;
		}
		return false;
	}
} // END inline void IncrementBufferPosition

/*! @param			m pointer of DiodeMatrix structure
 *  @return			true when decrement to 0, false otherwise*/
inline bool DecrementTo0SlowClear(volatile DiodeMatrix *m) {
	if (m->uiSlowClearedPos) {
		m->uitBufferX[m->uiSlowClearedPos--] = 0;
		if (m->uiSlowClearedPos == 0)
			return true;
		return false;
	}
	return false;
} // END inline bool DecrementTo0SlowClear

/*! @param			m pointer of DiodeMatrix structure*/
inline void RunSlowClearedPos(volatile DiodeMatrix *m) {
	m->uiSlowClearedPos = m->uiEndBufferPosition;
}

#endif /* DIODE_MATRIX_H_ */
