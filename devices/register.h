/*!
 * @file register.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-12-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 9<br>
 * Obsluga 5 rejestrow matrycy LED. <br> <span class="rowny">
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
 * Wybrane cechy:<br>
 * 		- osobne szeregowe wejscia danych dla kazdego rejestru<br>
 * 		- wspolny sygnal zegarowy dla wejscia szeregowego dla rejestrow X, osobny dla Y<br>
 * 		- wspolny sygnal zegarowy dla zatrzasku wyjscia rownoneglego dla rejestrow X i Y<br>
 * 		- rejestr Y posiada sterowanie stanem wyjscia rownoleglego, mozna przejsc na stan wysokiej
 * 	      impedancji, co jest wykorzystywane w wylaczeniu calej matrycy<br>
 *
 * @see DiodeMatrix
 */

#ifndef register_H_
#define register_H_

#include "../group_includes.h"

/*
 *
 *		Makroinstrukcje IO
 *
 */

/*
 *
 *  wykorzystywane sa wszystkie grupy rejestrow
//! rejestr kierunku wejscia danych dla grupy rejestrow X
#define REGISTERSB_DDR DDRB
//! rejestr kierunku wejscia danych dla grupy rejestrow X
#define REGISTERSC_DDR DDRC
//! rejestr stanu wejscia danych dla grupy rejestrow X
#define REGISTERSB_PORT PORTB
//! rejestr stanu wejscia danych dla grupy rejestrow X
#define REGISTERSC_PORT PORTC
*/

/*
 *		X0
 */
//! rejestr kierunku wejscia danych dla rejestru X0
#define X0_DATA_DDR DDRC
//! rejestr stanu wejscia danych dla rejestru X0
#define X0_DATA_PORT PORTC
//! adres wejscia danych dla rejestru X0
#define X0_DATA_ADDR (1 << PC0)
//! ustawia wartosc wejscia danych dla rejestru X0 na HIGH
#define X0_DATA_HIGH() X0_DATA_PORT |= X0_DATA_ADDR
//! ustawia wartosc wejscia danych dla rejestru X0 na LOW
#define X0_DATA_LOW() X0_DATA_PORT &= ~X0_DATA_ADDR


/*
 *		X1
 */
//! rejestr kierunku wejscia danych dla rejestru X1
#define X1_DATA_DDR DDRB
//! rejestr stanu wejscia danych dla rejestru X1
#define X1_DATA_PORT PORTB
//! adres wejscia danych dla arejestru X1
#define X1_DATA_ADDR (1 << PB5)
//! ustawia wartosc wejscia danych dla rejestru X1 na HIGH
#define X1_DATA_HIGH() X1_DATA_PORT |= X1_DATA_ADDR
//! ustawia wartosc wejscia danych dla rejestru X1 na LOW
#define X1_DATA_LOW() X1_DATA_PORT &= ~X1_DATA_ADDR


/*
 * 		X2
 */
//! rejestr kierunku wejscia danych dla rejestru X2
#define X2_DATA_DDR DDRB
//! rejestr stanu wejscia danych dla rejestru X2
#define X2_DATA_PORT PORTB
//! adres wejscia danych dla arejestru X2
#define X2_DATA_ADDR (1 << PB4)
//! ustawia warosc wejscia danych dla rejestru X2 na HIGH
#define X2_DATA_HIGH() X2_DATA_PORT |= X2_DATA_ADDR
//! ustawia wartosc wejscia danych dla rejestru X2 na LOW
#define X2_DATA_LOW() X2_DATA_PORT &= ~X2_DATA_ADDR

/*
 *		X3
 */
//! rejestr kierunku wejscia danych dla rejestru X2
#define X3_DATA_DDR DDRB
//! rejestr stanu wejscia danych dla rejestru X2
#define X3_DATA_PORT PORTB
//! adres wejscia danych dla rejestru X3
#define X3_DATA_ADDR (1 << PB1)
//! ustawia wartosc wejscia danych dla rejestru X3 na HIGH
#define X3_DATA_HIGH() X3_DATA_PORT |= X3_DATA_ADDR
//! ustawia wartosc wejscia danych dla rejestru X3 na LOW
#define X3_DATA_LOW() X3_DATA_PORT &= ~X3_DATA_ADDR

/*
 *		X_CLK_DATA
 */
//! rejestr kierunku zegara wejscia szeregowego dla rejestrow X
#define X_CLK_DATA_DDR DDRB
//! rejestr stanu zegara wejscia szeregowego dla rejestrow X
#define X_CLK_DATA_PORT PORTB
//! adres zegara wejscia szeregowego dla rejestrow X
#define X_CLK_DATA_ADDR (1 << PB3)

/*
 *		Y
 */
//! rejestr kierunku zegara wejscia szeregowego dla rejestru Y
#define Y_DATA_DDR DDRD
//! rejestr stanu zegara wejscia szeregowego dla rejestru Y
#define Y_DATA_PORT PORTD
//! adres wejscia danych dla rejestru Y
#define Y_DATA_ADDR (1 << PD4)
//! ustawia wartosc wejscia danych dla rejestru Y na HIGH
#define Y_DATA_HIGH() Y_DATA_PORT |= Y_DATA_ADDR
//! ustawia wartosc wejscia danych dla rejestru Y na LOW
#define Y_DATA_LOW() Y_DATA_PORT &= ~Y_DATA_ADDR

/*
 *		Y_CLK_DATA
 */
//! rejestr kierunku zegara wejscia danych dla rejestru Y
#define Y_CLK_DATA_DDR DDRD
//! rejestr stanu zegara wejscia danych dla rejestru Y
#define Y_CLK_DATA_PORT PORTD
//! adres zegara wejscia danych dla rejestru Y
#define Y_CLK_DATA_ADDR (1 << PD2)

/*
 *		LATCH
 */
//! rejestr kierunku zegara wejscia szeregowego dla rejestru Y
#define LATCH_DDR DDRB
//! rejestr stanu zegara wejscia szeregowego dla rejestru Y
#define LATCH_PORT PORTB
//! adres zatrzasku wyjscia rowloleglego dla wszystkich rejestrow
#define LATCH_ADDR (1 << PB2)
//! ustawia wartosc zatrzasku wyjscia rownoleglego dla wszystkich rejestrow na HIGH
#define LATCH_HIGH() LATCH_PORT |= LATCH_ADDR
//! ustawia wartosc zatrzasku wyjscia rownloleglego dla wszystkich rejestrow na LOW
#define LATCH_LOW() LATCH_PORT &= ~LATCH_ADDR


/*
 *
 *		Deklaracje funkcji
 *
 */

//! inicjalizacja rejestrow
extern void RegistersInit();
//! wysyla pojedynczy bit do rejestru Y
inline void SendRegisterY(BinarySwitch eB, bool bWithLoad);
//! wysyla tablice bitow do rejestow X0 - X3
inline void SendRegistersX(uint8_t uitBuffer[32], bool bWithLoad);
//! wysyla tablice bitow do rejestru X0
inline void SendRegisterX(uint8_t uitBuffer[8], bool bWithLoad);

/*
 *
 * 		Definicje funkcji inline
 *
 */

inline void Y_CLK_01() {
	Y_CLK_DATA_PORT &= ~Y_CLK_DATA_ADDR;
	Y_CLK_DATA_PORT |= Y_CLK_DATA_ADDR;
}

inline void X_CLK_01() {
	X_CLK_DATA_PORT &= ~X_CLK_DATA_ADDR;
	X_CLK_DATA_PORT |= X_CLK_DATA_ADDR;

}

inline void LATCH_01() {
	LATCH_HIGH();
	LATCH_LOW();
}

/*
 @param 		eB wartosc logiczna stanu
inline void YSw(BinarySwitch eB) {
	if (eB) Y_SW_PORT |= Y_SW_ADDR;
	else Y_SW_PORT &= ~Y_SW_ADDR;
}*/

/*! @param		eB wartosc logiczna przesylana do rejestru
 *  @param		bWithLoad decyzja o zaladowaniu zawartosci rejestru na jego wyjscie*/
inline void SendRegisterY(BinarySwitch eB, bool bWithLoad) {
	if (eB) Y_DATA_HIGH();
	else Y_DATA_LOW();
	Y_CLK_01();

	if (bWithLoad) LATCH_01();
} // END inline void SendRegisterY

/*! @param		uitBuffer tablica bitow danych szeregowych dla rejestrow X0 - X3
 *  @param		bWithLoad decyzja o zaladowaniu zawartosci rejestru na jego wyjscie*/
inline void SendRegistersX(uint8_t uitBuffer[32], bool bWithLoad) {
	int8_t i;
	// 8 bitow do 4 rejestrow, od tylu ze wzgledu na przesuwnosc rejestru, pierwsze bity beda osatnimi
	for (i = 7; i >= 0; i--) {
		if (uitBuffer[i]) X0_DATA_HIGH();
			else X0_DATA_LOW();
		if (uitBuffer[i + 8] ) X1_DATA_HIGH();
			else X1_DATA_LOW();
		if (uitBuffer[i + 16]) X2_DATA_HIGH();
			else X2_DATA_LOW();
		if (uitBuffer[i + 24]) X3_DATA_HIGH();
			else X3_DATA_LOW();
		X_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegistersX

/*!  @param		uitBuffer tablica bitow danych szeregowych dla rejestru X0
 *   @param		bWithLoad decyzja o zaladowaniu zawartosci rejestru na jego wyjscie*/
inline void SendRegisterX(uint8_t uitBuffer[8], bool bWithLoad) {
	int8_t i;
	for (i = 7; i >= 0; i--) {
		if (uitBuffer[i]) X0_DATA_HIGH();
			else X0_DATA_LOW();
		X_CLK_01();
	}
	if (bWithLoad) LATCH_01();
} // END inline void SendRegisterX

#endif /* 54HC595_H_ */
