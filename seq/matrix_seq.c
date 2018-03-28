/*
 * matrix_functions.c
 *
 *  Created on: 4 lis 2017
 *      Author: MS
 */

#include "matrix_seq.h"


/*
 *
 * 		Funkcje wewnetrzne
 *
 */

uint8_t uitHMPos[4] = {0, 6, 14, 20};

void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds, uint8_t brightness) {
	uint8_t tens = seconds / 10;
	uint8_t ones = seconds % 10;
	for(uint8_t i = 0; i < 4; i++) {
		if (tens % 2) {
			m->uitBufferYX[2][SEC_END_POS - i] = brightness;
		}else{
			m->uitBufferYX[2][SEC_END_POS - i] = 0;
		}
		tens >>= 1;
		if (ones % 2){
			m->uitBufferYX[5][SEC_END_POS - i] = brightness;
		}else{
			m->uitBufferYX[5][SEC_END_POS - i] = 0;
		}
		ones >>= 1;
	}
}

uint8_t InsertTextToMatrix(volatile DiodeMatrix *m, char sign, uint8_t position, uint8_t brightness) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t j = start; j <= stop; j++) {
		SetXBuffer(m, position++, LoadIntSignByte(signCode, j), brightness);
		if (position >= BUFFER_X_SIZE)
			return BUFFER_X_SIZE;
	}
	return position;
}

void InsertTextToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos, uint8_t brightness) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t i = 0; i < 5; i++)
		SetXRoundBuffer(m, i, y_pos, 0, 0);
	for (uint8_t j = start; j <= stop; j++) {
		SetXRoundBuffer(m, j, y_pos, LoadIntSignByte(signCode, j), brightness);
	}
}


void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE], uint8_t brightness) {
	uint8_t i = 0;
	uint8_t addr = 0;

	//ClearBuffer(m);
	while(text[i]) {
		addr = InsertTextToMatrix(m, text[i], addr, brightness);
		i++;
		addr++;
	}
	//strcpy(ctOldBuffer, text);
	m->uiEndBufferPosition = addr;
} // END void LoadTextToMatrix

void LoadTimeToMatrix(volatile DiodeMatrix *m, Time *from, Time *to, uint8_t brightness) {
	SetXBuffer(m, SEC_SIGN_POS, (to->uiSeconds % 2) == 0 ? 0x44 : 0x00, brightness);
	for (int8_t i = 3; i >= 0; i--) {
		if (from->uitSingleTime[i] == to->uitSingleTime[i]){
			from->uiSingleProgress[i] = MAX_PROGRESS;
			//break;
		}

		if (from->uiSingleProgress[i] < MAX_PROGRESS) {
			if (from->uiSingleProgress[i] == 0) {
				InsertTextToRoundBuffer(m, from->uitSingleTime[i]+48, 0, brightness);
				InsertTextToRoundBuffer(m, to->uitSingleTime[i]+48, 8, brightness);
			}
			CopyFromRoundToBuffer(m, from->uiSingleProgress[i]++, uitHMPos[i]);
			break;
		} else {
			from->uitSingleTime[i] = to->uitSingleTime[i];
			InsertTextToMatrix(m, from->uitSingleTime[i]+48, uitHMPos[i], brightness);
		}
	}



	SecondsBinary(m, to->uiSeconds, brightness);
	LoadToGroupTime(from);
	m->uiEndBufferPosition = SEC_END_POS + 1;
} // END void LoadTimeToMatrix
