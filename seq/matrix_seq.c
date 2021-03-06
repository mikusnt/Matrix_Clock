/*!
 * @file matrix_seq.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see matrix_seq.h*/

#include "matrix_seq.h"


uint8_t uitHMPos[4] = {0, 6, 14, 20};

char ctTextBuffer[TEXT_BUFFER_SIZE] = {0};

ActualSeq eActualSeq = SeqTimer;

/*
 *
 * 		Internal functions
 *
 */

/*! load binary secoinds in BCD to matrix buffer
 *  @param		m pointer of DiodeMatrix structure
 *  @param		seconds to load*/
static inline void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds) {
	uint8_t tens = seconds / 10;
	uint8_t ones = seconds % 10;
	for(uint8_t i = 0; i < 4; i++) {
		m->uitBufferX[SEC_END_POS - i] = 0;
		if (tens % 2)
			m->uitBufferX[SEC_END_POS - i] |= 1 << 1;
		tens >>= 1;
		if (ones % 2)
			m->uitBufferX[SEC_END_POS - i] |= 1 << 4;
		ones >>= 1;
	}
} // END static inline void SecondsBinary

/*! load one ASCII sign to matrix buffer
 *  @param		m pointer of DiodeMatrix structure
 *  @param		sign to load
 *  @param		x_pos x start of sign in matrix buffer position
 *  @return		end sign position in matrix buffer*/
static uint8_t InsertCharToMatrix(volatile DiodeMatrix *m, char sign, uint16_t x_pos) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t j = start; j <= stop; j++) {
		m->uitBufferX[x_pos++] = LoadIntSignByte(signCode, j);
		if (x_pos >= BUFFER_X_SIZE)
			return BUFFER_X_SIZE - 1;
	}
	return x_pos;
} // END static uint8_t InsertCharToMatrix

/*! load to round matrix buffer ose sign on one Y position
 *  @param		m pointer of DiodeMatrix structure
 *  @param		sign to load
 *  @param		y_pos Y position in round buffer {0, 1}*/
static void InsertCharToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos) {
	if (y_pos < 2) {
		uint8_t start, stop, signCode;
		LoadSign(sign, &start, &stop, &signCode);
		for (uint8_t i = 0; i < 5; i++)
			m->uitRoundBufferYX[y_pos][i] = 0;
		for (uint8_t j = start; j <= stop; j++) {
			m->uitRoundBufferYX[y_pos][j] = LoadIntSignByte(signCode, j);
		}
	}
} // END static void InsertCharToRoundBuffer

/*! load info about ADC letel to last Y position in matrix buffer
 *  @param		m pointer of DiodeMatrix structure
 *  @param		adcValue 10-bit ADC value*/
void LoadADCToMatrix(volatile DiodeMatrix *m, uint16_t adcValue) {
	adcValue /= 32;
	for (int8_t i = 31; i >= 0; i--){
		m->uitBufferX[i] &= ~(1 << 7);
		if (i >= MATRIX_X_SIZE - adcValue)
			m->uitBufferX[i] |= 1 << 7;
	}
} // END void LoadADCToMatrix

/*! load number of dots horizontally from right to left
 *  @param		m poiner of DiodeMatrix structure
 *  @param		number to load
 *  @param		position of horizontal line of dots*/
static inline void LoadDotsToMatrix(volatile DiodeMatrix *m, uint8_t number, uint8_t position) {
	for (int8_t i = MATRIX_X_SIZE; i >= 0; i--) {
		if ((number >= (MATRIX_X_SIZE - i)) && (position < MATRIX_Y_SIZE)) {
			m->uitBufferX[i] |= (1 << position);
		} else {
			m->uitBufferX[i] &= ~(1 << position);
		}
	}
} // END void LoadDotsToMatrix

/*
 *
 *		External functions
 *
 */


/*! load all of the signs parameters from text, aoad signs to next buffer position
 *  @param		m poiner of DiodeMatrix structure
 *  @param		text to load to buffer*/
void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE]) {
	uint16_t i = 0;
	uint16_t addr = 0;
	while(text[i] && (addr < BUFFER_X_SIZE)) {
		addr = InsertCharToMatrix(m, text[i], addr);
		i++;
		if (addr < BUFFER_X_SIZE - 1)
			m->uitBufferX[addr++] = 0;
	}
	m->uiEndBufferPosition = addr;
} // END void LoadTextToMatrix

/*! when two structures of time are not the same, load old one to matrix buffer
 *  uses round buffer to gradual transition between two signs
 *  @param		m poiner of DiodeMatrix structure
 *  @param		actual time to load to matrix buffer, approaches to real time
 *  @param		real time*/
void LoadTimeToMatrix(volatile DiodeMatrix *m, TimeDate *actual, TimeDate *real) {
	m->uitBufferX[SEC_SIGN_POS] = (real->uiSecond % 2) == 0 ? 0x24 : 0x00;
	for (int8_t i = 3; i >= 0; i--) {
		// when signs are the same
		if ((actual->uitSingleTime[i] == real->uitSingleTime[i]) && (actual->uiSingleProgress[i] < MAX_PROGRESS))
			actual->uiSingleProgress[i] = MAX_PROGRESS;

		// gradual transition from round buffer
		if (actual->uiSingleProgress[i] < MAX_PROGRESS) {
			if (actual->uiSingleProgress[i] == 0) {
				InsertCharToRoundBuffer(m, actual->uitSingleTime[i] + DIGIT_ASCII, 0);
				InsertCharToRoundBuffer(m, real->uitSingleTime[i] + DIGIT_ASCII, 1);
			}
			CopyFromRoundToBuffer(m, actual->uiSingleProgress[i]++, uitHMPos[i]);
			break;
		// load right sign
		} else if (actual->uiSingleProgress[i] == MAX_PROGRESS){
			actual->uitSingleTime[i] = real->uitSingleTime[i];
			LoadToDecimalTime(actual);
			InsertCharToMatrix(m, actual->uitSingleTime[i] + DIGIT_ASCII, uitHMPos[i]);
			// block next insertion when inserted
			actual->uiSingleProgress[i] = MAX_PROGRESS + 1;
		}
	}

	SecondsBinary(m, real->uiSecond);
	//LoadDotsToMatrix(m, real->uiSecond / 2, 7);
	//LoadADCToMatrix(m, adcValue, brightness);

	m->uiEndBufferPosition = SEC_END_POS + 1;
} // END void LoadTimeToMatrix

/*! @param		m poiner of DiodeMatrix structure
 *  @param		number to load*/
void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number) {
	sprintf(ctTextBuffer, "%04d%c", number, 0);
	LoadTextToMatrix(m, ctTextBuffer);
} // END void LoadNumberToMatrix

//! load hour, minutes by digit and seconds by dots line to matrix buffer
void LoadDeCounterToMatrix(volatile DiodeMatrix *m, TimeDate *time) {
	sprintf(ctTextBuffer, "%03d:%02d%c", (time->uiDay * 24) + time->uiHour, time->uiMinute, 0);
	if (time->uiSecond % 2) ctTextBuffer[3] = ' ';
		else ctTextBuffer[3] = 0x80;
	LoadTextToMatrix(m, ctTextBuffer);
	LoadDotsToMatrix(m, time->uiSecond / 2, 7);
} // END void LoadDeCounterToMatrix

/*! @param		m poiner of DiodeMatrix structure
 *  @param		actTime pointer of actual time structure
 *  @param		RTCTime pointer of real time structure
 *  @param		relay pointer of relay structure*/
void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, TimeDate *RTCTime, volatile Relay *relay) {
	ClearBuffer(m);
	uart_puts_p(PSTR("Seq: "));

	switch(eActualSeq) {
		case SeqTimer: {
			LoadTextToMatrix(m, "00 00");
			TimeInit(actTime);
			SetMoving(m, false);
			uart_puts_p(PSTR("Timer"));
		} break;
		case SeqADC: {
			SetMoving(m, false);
			uart_puts_p(PSTR("ADC brightness"));
		} break;
		case SeqText: {
			LoadTextToMatrix(m, ctTextBuffer);
			SetMoving(m, true);
			uart_puts_p(PSTR("Text \""));
			uart_puts(ctTextBuffer);
			uart_putc('\"');
		} break;
		case SeqTextDebug: {
			LoadTextToMatrix(m, ctTextBuffer);
			uart_puts_p(PSTR("Text debug \""));
			uart_puts(ctTextBuffer);
			uart_putc('\"');
		} break;
		case SeqRelayNumber: {
			uart_puts_p(PSTR("Relay with number "));
			uart_putint(ctTextBuffer[0], 10);
			RelayStartClicking(relay, ctTextBuffer[0], RelayDataNumber);
			LoadNumberToMatrix(m, ctTextBuffer[0]);
			SetMoving(m, false);
		} break;
		case SeqBomb: {
			LoadTextToMatrix(m, "00 00");
			TimeInit(actTime);
			actTime->uiMinute = ctTextBuffer[1];
			LoadToSingleTime(actTime);
			SetMoving(m, false);
			uart_puts_p(PSTR("Bomb"));

		} break;
		case SeqDeCounter: {
			LoadTextToMatrix(m, "00 00");
			TimeInit(actTime);
			actTime->uiDay = ctTextBuffer[1];
			actTime->uiHour = ctTextBuffer[2];
			actTime->uiMinute = ctTextBuffer[3];
			SetMoving(m, false);
			uart_puts_p(PSTR("DeCounter"));
		} break;
		case SeqEmpty: {
			SetMoving(m, false);
			uart_puts_p(PSTR("Clear matrix"));
		}
	}
	uart_putc('\n');
} // END void SetSeqParams

/*! @param 		m poiner of DiodeMatrix structure
 *  @param		y_pos y position in matrix buffer
 *  @param		state (logic) of point in matrix buffer*/
inline void SetStatePoint(volatile DiodeMatrix *m, uint8_t x_pos, BinarySwitch state) {
	if (state == ON) {
		//m->uitBufferX[26] |= (1 << x_pos);
		m->uitBufferX[x_pos] |= 1 << 6;
	} else {
		//m->uitBufferX[26] &= ~(1 << x_pos);
		m->uitBufferX[x_pos] &= ~(1 << 6);
	}
} // END inline void SetStatePoint


