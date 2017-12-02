/*
 * @file register.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-12-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 */

#include "register.h"


/*! konfiguracja rejestrow IO do pracy rejestrow 74HC595*/
void RegistersInit() {
	REGISTERS_DDR |= X0_DATA_ADDR | X_CLK_ADDR | LATCH_ADDR | Y_DATA_ADDR | Y_CLK_ADDR;
	//REGISTERS_DDR |= X1_DATA_ADDR | X2_DATA_ADDR | X3_DATA_ADDR;
	Y_SW_DDR |= Y_SW_ADDR;
} // END void RegistersInit







