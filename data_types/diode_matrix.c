/*!
 * @file diode_matrix.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see diode_matrix.h*/

#include "diode_matrix.h"

const uint8_t gamma_o[MAX_GAMMA_BRIGHTNESS+1] = {0, 1, 3, 10, 19};

/*! @param			m adres struktury macierzy LED
 *  @param			isMoving sposob wyswietlania*/
void SetMoving(volatile DiodeMatrix *m, bool isMoving) {
	m->bIsMoving = isMoving;
	if (isMoving)
		m->i16BufferPosition = -MATRIX_X_SIZE;
	else
		m->i16BufferPosition = 0;
} // END void SetMoving

/*! @param			m adres struktury macierzy LED
 *  @param			y_pos wspolrzedna y
 *  @param 			value wartosc jasnosci*/
void SetYBuffer(volatile DiodeMatrix *m, uint8_t y_pos, uint8_t value) {
	for (uint8_t i = 0; i < BUFFER_X_SIZE; i++)
		m->uitBufferYX[y_pos][i] = value;
} // END void SetYBuffer

/*! @param			m adres struktury macierzy LED
 *  @param			x_pos wspolrzedna x
 *  @param			signMask maska danej linijki, bit 0 to brak swiecenia, 1 to swiecenie o danej jasnosci
 *  @param 			value wartosc jasnosci*/
void SetXBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t signMask, uint8_t brightness) {
	for (uint8_t i = 0; i < MATRIX_Y_SIZE; i++){
		if (signMask % 2)
			m->uitBufferYX[i][x_pos] = brightness;
		else
			m->uitBufferYX[i][x_pos] = 0;
		signMask >>= 1;
	}
} // END void SetYBuffer

/*! @param			m adres struktury macierzy LED
 *  @param			x_pos wspolrzedna x
 *  @param			y_pos poczatkowa wspolrzedna Y
 *  @param			signMask maska danej linijki, bit 0 to brak swiecenia, 1 to swiecenie o danej jasnosci
 *  @param 			value wartosc jasnosci*/
void SetXRoundBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t y_pos, uint8_t signMask, uint8_t value) {
	for (uint8_t i = 0; i < MATRIX_Y_SIZE; i++){
		if (signMask % 2)
			m->uitRoundBufferYX[i+y_pos][x_pos] = value;
		else
			m->uitRoundBufferYX[i+y_pos][x_pos] = 0;
		signMask >>= 1;
	}
} // END void SetYBuffer

/*! @param			m adres struktury macierzy LED*/
void ClearBuffer(volatile DiodeMatrix *m) {
	for (uint8_t i = 0; i < MATRIX_Y_SIZE; i++)
		for (uint8_t j = 0; j < BUFFER_X_SIZE; j++)
			m->uitBufferYX[i][j] = 0;
	m->i16BufferPosition = 0;
}

void CopyFromRoundToBuffer(volatile DiodeMatrix *m, uint8_t y_round, uint8_t x_buffer) {
	for(uint8_t i = 0; i < 5; i++)
		for(uint8_t j = 0; j < MATRIX_Y_SIZE; j++) {
			m->uitBufferYX[j][i + x_buffer] = m->uitRoundBufferYX[j + y_round][i];
		}

}
