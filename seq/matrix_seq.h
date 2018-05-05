/*!
 * @file matrix_seq.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-04 $
 * $Created: 2017-11-04 $
 * @version 1.0
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
#include "../devices/other_devices.h"
#include "../devices/relay.h"
#include "../devices/bluetooth/mkuart.h"
#include "alphabet.h"


//! size of text buffer plus 1 to end of char table
#define TEXT_BUFFER_SIZE ((BUFFER_X_SIZE / 4) + 1)
//! position x of colon in matrix buffer
#define SEC_SIGN_POS 12
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
	SeqEmpty = 'c'
} ActualSeq;

//! device's fimple tasks
typedef enum {
	TaskSetPix = 'p',
	TaskRelayNumber = 'r',
	TaskRelayMode = 'm'
} DeviceTask;

//! actual sequence displaying by device
//! @see SetSeqParams
extern ActualSeq eActualSeq;

/*
 *
 * 		Internal functions
 *
 */
// laduje do bufora matrycy sekundy w binarnym kodzie BCD
//static inline void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds, uint8_t brightness)
// laduje do bufora matrycy wybrany znak ASCII
//static uint8_t InsertCharToMatrix(volatile DiodeMatrix *m, char sign, uint8_t position, uint8_t brightness)
// laduje do bufora obrotowego matrycy wskazany znak na odpowiedniej pozycji
//static void InsertCharToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos, uint8_t brightness)
// laduje do bufora matrycy informacje o wartosci ADC w postaci kropek na pozycji 7
//void LoadADCToMatrix(volatile DiodeMatrix *m, uint16_t adcValue, uint8_t brightness)

/*
 *
 *		External functions
 *
 */
//! load text to matrix buffer from alphabet
extern void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE]);
//! load hour to matrix buffer from alphabet, roundbuffer effect on chars
extern void LoadTimeToMatrix(volatile DiodeMatrix *m, volatile Relay *r, TimeDate *from, TimeDate *to);
//! load one 5 digit integer to matrix buffer
extern void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number);
//! set parametrs when sequence is changed
extern void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, volatile Relay *relay);

#endif /* MATRIX_SEQUENCES_H_ */
