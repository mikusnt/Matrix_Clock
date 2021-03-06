/*!
 * @file diode_matrix.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see diode_matrix.h*/

#include "diode_matrix.h"


const uint8_t gamma_o[MAX_GAMMA_BRIGHTNESS+1] = {1, 1, 1, 2, 2, 2, 3, 5, 8, 11, 19};

/*! @param			m pointer of DiodeMatrix structure
 *  @param			isMoving new mode of displaying*/
void SetMoving(volatile DiodeMatrix *m, bool isMoving) {
	m->bIsMoving = isMoving;
	if (isMoving)
		m->i16BufferPosition = -MATRIX_X_SIZE;
	else
		m->i16BufferPosition = 0;
} // END void SetMoving

/*! @param			m pointer of DiodeMatrix structure
 *  @param			y_pos y position in matrix buffer
 *  @param 			value of pixels*/
void SetYBuffer(volatile DiodeMatrix *m, uint8_t y_pos, BinarySwitch value) {
	register uint16_t i;
	for (i = 0; i < BUFFER_X_SIZE; i++) {
		m->uitBufferX[i] &= ~(1 << y_pos);
		m->uitBufferX[i] |= (value << y_pos);
	}
} // END void SetYBuffer

/*! @param			m pointer of DiodeMatrix structure*/
void ClearBuffer(volatile DiodeMatrix *m) {
	register uint16_t i;
	for (i = 0; i < BUFFER_X_SIZE; i++)
		m->uitBufferX[i] = 0;
	m->i16BufferPosition = 0;
} // END void ClearBuffer

/*! @param 			m pointer of DiodeMatrix structure
 *  @param			y_round_pos y start position in round buffer
 *  @param			x_buffer_pos x start position in matrix buffer*/
void CopyFromRoundToBuffer(volatile DiodeMatrix *m, uint8_t y_round_pos, uint8_t x_buffer_pos) {
	uint8_t y0_round_mask = 0xFF << y_round_pos;
	uint8_t y1_round_mask = ~y0_round_mask;
	for (uint8_t j = 0; j < 5; j++) {
		m->uitBufferX[x_buffer_pos + j] = 0;
		m->uitBufferX[x_buffer_pos + j] |= (m->uitRoundBufferYX[0][j] & y0_round_mask) >> y_round_pos;
		m->uitBufferX[x_buffer_pos + j] |= (m->uitRoundBufferYX[1][j] & y1_round_mask) << (8 - y_round_pos);
	}
} // END void CopyFromRoundToBuffer
