/*!
 * @file register.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-20 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Used uC pins: 9<br>
 * Header file containing 5 LED matrix shift registers. <br> <span class="rowny">
 * Model of register: serial in (1 bit), parallel out(8 bit) 74HC595
 * ___|X0_____________________|X1____________________|X2_____________________|X3____________________		<br>
 * ___|0__1__2__3__4__5__6__7_|0__1__2__3__4__5__6__7|0__1__2__3__4__5__6__7_|0__1__2__3__4__5__6__7		<br>
 * ____0__1__2__3__4__5__6__7__8__9_10_11_12_13_14_15_16_17_18_19_20_21_22_23_24_25_26_27_28_29_30_31		<br>
 * Y_0|																										<br>
 * __1|																										<br>
 * __2|																										<br>
 * __3|																										<br>
 * __4|																										<br>
 * __5|																										<br>
 * __6|																										<br>
 * __7|																										<br>
 * </span>																											<br>
 * Features:<br>
 * 		- different serial data input to all register <br>
 * 		- same serial clock signal ro X register, different to Y <br>
 * 		- wspolny sygnal zegarowy dla zatrzasku wyjscia rownoneglego dla rejestrow X i Y<br>
 * 		- same parallel data
 * 		- rejestr Y posiada sterowanie stanem wyjscia rownoleglego, mozna przejsc na stan wysokiej
 * 	      impedancji, co jest wykorzystywane w wylaczeniu calej matrycy, niestety efekt duchow<br>
 * 	      nalezalo obejsc poprzez zaladowanie zer dla X0-X4, dopiero pozniej mozna przelaczyc Y
 *
 * @see DiodeMatrix
 */

#ifndef register_H_
#define register_H_

#include "../group_includes.h"

/*
 *
 *		IO macros
 *
 */


/*
 *		X0
 */
//! direction register of serial data input X0
#define X0_DATA_DDR DDRC
//! state register of serial data input X0
#define X0_DATA_PORT PORTC
//! address of serial data input X0
#define X0_DATA_ADDR (1 << PC0)
//! set high state to serial data input X0
#define X0_DATA_HIGH() X0_DATA_PORT |= X0_DATA_ADDR
//! set low state to serial data input X0
#define X0_DATA_LOW() X0_DATA_PORT &= ~X0_DATA_ADDR


/*
 *		X1
 */
//! direction register of serial data input X1
#define X1_DATA_DDR DDRB
//! state register of serial data input X1
#define X1_DATA_PORT PORTB
//! address of serial data input X1
#define X1_DATA_ADDR (1 << PB5)
//! set high state to serial data input X1
#define X1_DATA_HIGH() X1_DATA_PORT |= X1_DATA_ADDR
//! set low state to serial data input X1
#define X1_DATA_LOW() X1_DATA_PORT &= ~X1_DATA_ADDR


/*
 * 		X2
 */
//! direction register of serial data input X2
#define X2_DATA_DDR DDRB
//! state register of serial data input X2
#define X2_DATA_PORT PORTB
//! address of serial data input X2
#define X2_DATA_ADDR (1 << PB4)
//! set high state to serial data input X2
#define X2_DATA_HIGH() X2_DATA_PORT |= X2_DATA_ADDR
//! set low state to serial data input X2
#define X2_DATA_LOW() X2_DATA_PORT &= ~X2_DATA_ADDR

/*
 *		X3
 */
//! direction register of serial data input X3
#define X3_DATA_DDR DDRB
//! state register of serial data input X3
#define X3_DATA_PORT PORTB
//! address of serial data input X3
#define X3_DATA_ADDR (1 << PB1)
//! set high state to serial data input X3
#define X3_DATA_HIGH() X3_DATA_PORT |= X3_DATA_ADDR
//! set low state to serial data input X3
#define X3_DATA_LOW() X3_DATA_PORT &= ~X3_DATA_ADDR

/*
 *		Y
 */
//! direction register of serial data input Y
#define Y_DATA_DDR DDRD
//! state register of serial data input Y
#define Y_DATA_PORT PORTD
//! address of serial data input Y
#define Y_DATA_ADDR (1 << PD4)
//! set high state to serial data input Y
#define Y_DATA_HIGH() Y_DATA_PORT |= Y_DATA_ADDR
//! set low state to serial data input Y
#define Y_DATA_LOW() Y_DATA_PORT &= ~Y_DATA_ADDR

/*
 *		X_CLK_DATA
 */
//! direction register of clk of X group serial data inputs
#define X_CLK_DATA_DDR DDRB
//! state register of clk of X group serial data inputs
#define X_CLK_DATA_PORT PORTB
//! address of clk of X group serial data inputs
#define X_CLK_DATA_ADDR (1 << PB3)


/*
 *		Y_CLK_DATA
 */
//! direction register of clk of Y serial data input Y
#define Y_CLK_DATA_DDR DDRD
//! state register of clk of Y serial data input
#define Y_CLK_DATA_PORT PORTD
//! state register of clk of Y serial data input
#define Y_CLK_DATA_ADDR (1 << PD2)


/*
 *		Y_RESET, not used
 */
//! direction register of reset Y register
#define Y_RESET_DDR DDRD
//! state register of reset Y register
#define Y_RESET_PORT PORTD
//! address of reset Y register
#define Y_RESET_ADDR (1 << PD3)
//! set ON Y register
#define Y_ON() Y_RESET_PORT &= ~Y_RESET_ADDR
//! set OFF Y register
#define Y_OFF() Y_RESET_PORT |= Y_RESET_ADDR

/*
 *		LATCH
 */
//! direction register of parallel output enable of all registers
#define LATCH_DDR DDRB
//! state register of parallel output enable of all registers
#define LATCH_PORT PORTB
//! address of parallel output enable of all registers
#define LATCH_ADDR (1 << PB2)
//! set high state of parallel output enable of all registers
#define LATCH_HIGH() LATCH_PORT |= LATCH_ADDR
//! set low state of parallel output enable of all registers
#define LATCH_LOW() LATCH_PORT &= ~LATCH_ADDR


/*
 *
 *		Declaration of functions
 *
 */
//! initialize registers
extern void RegistersInit();
//! send one bit to Y register
inline void SendRegisterY(volatile BinarySwitch eB, bool bWithLoad);
//! send byte table to X0 - X3 registers
inline void SendRegistersX(volatile uint8_t uitLineBuffer[32], bool bWithLoad);
//! send zeros to X0 - X3 registers
inline void ClearRegistersX(bool bWithLoad);
//! send byte table only to X0 register
inline void SendRegisterX(volatile uint8_t uitBuffer[8], bool bWithLoad);

/*
 *
 * 		Definitions of inline functions
 *
 */

//! to load new byte to Y register
inline void Y_CLK_01() {
	Y_CLK_DATA_PORT &= ~Y_CLK_DATA_ADDR;
	Y_CLK_DATA_PORT |= Y_CLK_DATA_ADDR;
}

//! to load new bytes to X registers
inline void X_CLK_01() {
	X_CLK_DATA_PORT &= ~X_CLK_DATA_ADDR;
	X_CLK_DATA_PORT |= X_CLK_DATA_ADDR;

}

//! to reload parallel output in all registers
inline void LATCH_01() {
	LATCH_LOW();
	LATCH_HIGH();
}

/*! @param		eB new logic value
 *  @param		bWithLoad enable to reload parallel output*/
inline void SendRegisterY(volatile BinarySwitch eB, bool bWithLoad) {
	if (eB) Y_DATA_HIGH();
	else Y_DATA_LOW();
	Y_CLK_01();

	if (bWithLoad) LATCH_01();
} // END inline void SendRegisterY

/*! @param		uitLineBuffer logic bytes of registers state
 *  @param		bWithLoad enable to reload parallel output*/
inline void SendRegistersX(volatile uint8_t uitLineBuffer[32], bool bWithLoad) {
	register int8_t i; // required fast operations

	for (i = 0; i < 8; i++) {
		if (uitLineBuffer[0 + i]) X0_DATA_HIGH();
			else X0_DATA_LOW();
		if (uitLineBuffer[8 + i]) X1_DATA_HIGH();
			else X1_DATA_LOW();
		if (uitLineBuffer[16 + i]) X2_DATA_HIGH();
			else X2_DATA_LOW();
		if (uitLineBuffer[24 + i]) X3_DATA_HIGH();
			else X3_DATA_LOW();
		X_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersX

/*! @param		bWithLoad enable to reload parallel output*/
inline void ClearRegistersX(bool bWithLoad) {
	register uint8_t i;
	X0_DATA_LOW();
	X1_DATA_LOW();
	X2_DATA_LOW();
	X3_DATA_LOW();
	for (i = 0; i < 8; i++) {
		X_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersX

/*!  @param		uitBuffer logic bytes of X0 state
 *   @param		bWithLoad enable to reload parallel output*/
inline void SendRegisterX(volatile uint8_t uitBuffer[8], bool bWithLoad) {
	register int8_t i;
	for (i = 0; i < 8; i++) {
		if (uitBuffer[i]) X0_DATA_HIGH();
			else X0_DATA_LOW();
		X_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegisterX

#endif /* 54HC595_H_ */
