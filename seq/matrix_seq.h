/*!
 * @file matrix_seq.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-01-03 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Used uC pins: 0<br>
 * Header file containing functions to fill LED matrix buffer.
 * @see DiodeMatrix
 * @see alphabet.h
 */


#ifndef MATRIX_SEQ_H_
#define MATRIX_SEQ_H_

#include "../group_includes.h"
#include "../data_types/diode_matrix.h"
#include "../data_types/date_time.h"
#include "../devices/relay.h"
#include "../devices/bluetooth/mkuart.h"
#include "../devices/some_devices.h"
#include "alphabet.h"


//! size of text buffer plus 1 to end of char table
#define TEXT_BUFFER_SIZE ((BUFFER_X_SIZE / 4) + 1)
//! x position of colon in matrix buffer
#define SEC_SIGN_POS 12
//! x position of state points
#define STATE_POS 25
//! end position of binary seconds counter block in matrix buffer
#define SEC_END_POS 31
//! reduction from ASCII to number
#define DIGIT_ASCII 48


//! position of hour and minute digit in binary seconds counter in matrix buffer
extern uint8_t uitHMPos[4];
//! universal data buffer
//! @see uart_processing.h
extern char ctTextBuffer[TEXT_BUFFER_SIZE];

//! device's sequences of work
typedef enum {
	SeqTimer = 't',
	SeqADC = 'a',
	SeqText = 'e',
	SeqTextDebug = 'd',
	SeqRelayNumber = 'n',
	SeqBomb = 'b',
	SeqDeCounter = 'o',
	SeqEmpty = 'c'
} ActualSeq;

//! device's fimple tasks
typedef enum {
	TaskSetPix = 'p',
	TaskRelayNumber = 'r',
	TaskRelayMode = 'm',
	TaskWriteText = 'w',
	TaskDisplayText = 'd',
	TaskReadText = 'e',
	TaskSpectrum = 's'
} DeviceTask;

//! actual sequence displaying by device
//! @see SetSeqParams
extern ActualSeq eActualSeq;

/*
 *
 *		External functions
 *
 */
//! load text to matrix buffer from alphabet
extern void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE]);
//! load hour to matrix buffer from alphabet, roundbuffer effect on chars
extern void LoadTimeToMatrix(volatile DiodeMatrix *m, TimeDate *from, TimeDate *to);
//! load one 5 digit integer to matrix buffer
extern void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number);
//! load hour, minutes by digit and seconds by dots line to matrix buffer
extern void LoadDeCounterToMatrix(volatile DiodeMatrix *m, TimeDate *time);
//! set parametrs when sequence is changed
extern void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, TimeDate *RTCTime, volatile Relay *relay);
//! set one device state point to matrix buffer
extern void SetStatePoint(volatile DiodeMatrix *m, uint8_t y_pos, BinarySwitch state);

/*! load full time and date to ctTextBuffer
 *  @param		time pointer of time structure
 *  @param		cLast value of last char*/
inline void RTCToTextBuffer(TimeDate * time, char cLast) {
	snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, "%02d-%02d-2%03d %02d:%02d:%02d%c", time->uiDay,
			time->uiMonth, time->uiYear, time->uiHour,
			time->uiMinute, time->uiSecond, cLast);
} // END void RTCToTextBuffer

/*! load date and date to ctTextBuffer
 *  @param		time pointer of time structure
 *  @param		cLast value of last char*/
inline void DateToTextBuffer(TimeDate * time, char cLast) {
	snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, "%02d-%02d-2%03d%c", time->uiDay,
		time->uiMonth, time->uiYear, cLast);
} // END void DateToTextBuffer

#endif /* MATRIX_SEQUENCES_H_ */
