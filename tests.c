/*
 * @file tests.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2 gru 2017 $
 * $Created: 2 gru 2017 $
 * @version 0.1
 */

#include "tests.h"

/*! @param 		r wskaznik na przekaznik
 *  @see 		relay.h
 *  @see 		register.h
 * */
void Test_Relay_Hours_X0(volatile Relay *r) {
	uint8_t i;
	do {
		LoadToBuffer(i);
		SendRegisterX(tabl, true);
		D_S(1);

		RelayStartClicking(r, i, false);
		while (r->uiByteLength > 0);
		D_S(2);
	} while (++i < 24);
} // END void Test_Relay_X0

void Test_Relay_Minutes_X0(volatile Relay *r) {
	uint8_t i;
	do {
		LoadToBuffer(i);
		SendRegisterX(tabl, true);
		D_S(1);

		RelayStartClicking(r, i, true);
		while (r->uiByteLength > 0);
		D_S(2);
		i += 10;
	} while (i < 60);
} // END void Test_Relay_X0

/*! @param 		i wskaznik na licznik potrzebny do iterowania
 *  @see 		register.h
 *  */
void Test_Y(uint8_t *i) {
	SendRegisterY((*i > 0) ? 0 : 1, true);
	D_S(1);
	if (++*i > 7) *i = 0;
} // END void Test_Y
