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
	uint8_t i;
	for (i = 0; i < MATRIX_Y_SIZE; i++) {
		SetYBuffer(m, i, ON);
		D_MS(100);
	}
	for (i = 0; i < MATRIX_Y_SIZE; i++) {
		SetYBuffer(m, i, OFF);
		D_MS(100);
	}
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		m->uitBufferX[i] = 0xFF;
		D_MS(1000);
	}
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		m->uitBufferX[i] = 0;
		D_MS(1000);
	}
} // END void Test_MatrixBuffer
