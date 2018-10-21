/*!
 * @file register.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see register.h*/

#include "register.h"


/*! initialize IO registers to work which 74HC595*/
void RegistersInit() {
	Y_DATA_DDR |= Y_DATA_ADDR;
	Y_CLK_DATA_DDR |= Y_CLK_DATA_ADDR;
	Y_RESET_DDR |= Y_RESET_ADDR;

	LATCH_DDR |= LATCH_ADDR;
	Y_ON();

	for(uint8_t i = 0; i < 7; i++)
		SendRegisterY(OFF, false);
	SendRegisterY(OFF, true);

	X0_DATA_DDR |= X0_DATA_ADDR;
	X1_DATA_DDR |= X1_DATA_ADDR;
	X2_DATA_DDR |= X2_DATA_ADDR;
	X3_DATA_DDR |= X3_DATA_ADDR;
	X_CLK_DATA_DDR |= X_CLK_DATA_ADDR;
} // END void RegistersInit







