/*!
 * @file tests.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see tests.h*/

#include "tests.h"

/*! @param 		r wskaznik na przekaznik
 *  @see 		relay.h*/
void Test_Relay_Hours(volatile Relay *r) {
	uint8_t i = 0;
	do {
		RelayStartClicking(r, i, false);
		while (r->uiByteLength > 0);
		D_S(1);
	} while (++i < 24);
} // END void Test_Relay

/*! @param 		r wskaznik na przekaznik
 *  @see 		relay.h*/
void Test_Relay_Minutes(volatile Relay *r) {
	uint8_t i = 0;
	do {
		D_S(1);
		RelayStartClicking(r, i, true);
		while (r->uiByteLength > 0);
		D_S(2);
		i += 15;
	} while (i < 60);
} // END void Test_Relay

/*! @param 		i wskaznik na licznik potrzebny do iterowania
 *  @see 		register.h*/
void Test_Y(uint8_t *i) {
	SendRegisterY((*i > 0) ? 0 : 1, true);
	D_S(1);
	if (++*i > 7) *i = 0;
} // END void Test_Y

/*! @param		m adres struktury macierzy LED
 *  @see		diode_matrix.h*/
void Test_MatrixBuffer(volatile DiodeMatrix *m) {

	for (uint8_t i = 0; i < MATRIX_X_SIZE; i++) {
		for (uint8_t j = 0; j < MATRIX_Y_SIZE; j++) {
			if (i < 16) {
			if (j < 4)
				m->uitBufferYX[j][i] = gamma_o[4];
			else
				m->uitBufferYX[j][i] = gamma_o[2];
			} else {
				if (j < 4)
					m->uitBufferYX[j][i] = gamma_o[3];
				else
					m->uitBufferYX[j][i] = gamma_o[1];
			}
		}
	}
	m->uitBufferYX[0][MATRIX_X_SIZE+3] = gamma_o[4];
	m->uitBufferYX[7][MATRIX_X_SIZE+3] = gamma_o[1];
	m->uiEndBufferPosition = MATRIX_X_SIZE+4;
} // END void Test_MatrixBuffer
