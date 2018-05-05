/*!
 * @file diode_matrix.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Used uC pins: 0<br>
 * Header file containing logic structure of LED Matrix to display data.
 * @see		register.h
 */

#ifndef DIODE_MATRIX_H_
#define DIODE_MATRIX_H_

#include "../group_includes.h"

/*
 *
 *		Macros
 *
 */

//! Y size (vertical)
#define MATRIX_Y_SIZE 8
//! X size (horizontal)
#define MATRIX_X_SIZE 32
//! X size of hardware matrix buffer
#define BUFFER_X_SIZE 32 * 8
//! max number of brightness
#define MAX_MATRIX_BRIGHTNESS 19
//! max brightness in gamma correction table
//! @see gamma_o
#define MAX_GAMMA_BRIGHTNESS 4
//! time period to increment moving adress of buffer
#define INC_POS_MS 100

//! orange gamma correction
extern const uint8_t gamma_o[MAX_GAMMA_BRIGHTNESS+1];

/*
 *
 *		Main data types
 *
 */

//! main structure of LED Matrix
typedef struct {
	//! buffer of LED state, one byte for all Y positions (vertical)
	uint8_t uitBufferX[BUFFER_X_SIZE];
	//! actual bright level of matrix, modify by function
	//! @see IncrementBrightness
	uint8_t uiBrightCount;
	//! actual Y position (vertical), modify by function
	//! @see IncrementY
	uint8_t uiActY;
	//! buffer of LED state in actual Y position
	uint8_t uitBufferFlag[MATRIX_X_SIZE];
	//! actual matrix buffer start adress, modify by function
	//! @see IncrementBufferPosition
	int16_t i16BufferPosition;
	//! matrix buffer end address, modify manually
	uint8_t uiEndBufferPosition;
	//! mode of displaying, modify manually
	//! true - display clear matrix, increment start address to end buffer position
	//! false - display first part of buffer and increment address to see end part of buffer
	bool bIsMoving;
	uint8_t uitRoundBufferYX[2][5];
	//! position of right to left clearing line, set by
	//! @see 	RunSlowClearedPos
	//! @see 	DecrementTo0SlowClear
	uint8_t uiSlowClearedPos;
	//! brightness of matrix, modify manually
	uint8_t uiBrightness;
} DiodeMatrix;

/*
 *
 *		Deklaracje funkcji
 *
 */

//! zwieksza o 1 aktualna jasnosc matrycy
inline volatile bool IncrementBrightness(volatile DiodeMatrix *m);
//! zwieksza o 1 aktualna wspolrzedna Y
inline void IncrementY(volatile DiodeMatrix *m);
//! zwraca zawartosc rejestru Y na podstawie pozycji Y
inline volatile BinarySwitch ReturnYValue(volatile DiodeMatrix *m);
//! inicjalizacja struktury
inline void DiodeMatrixInit(volatile DiodeMatrix *m);
//! odswieza tablice dzialania LED dla danej pozycji Y
inline void RefreshBufferFlag(volatile DiodeMatrix *m);
//! zwieksza aktualna pozycje wyswietlania danych z bufora o 1, gdy wartosc osiagnie koncowa to reset do 0
inline bool IncrementBufferPosition(volatile DiodeMatrix *m);
//! ustawia tryb wyswietlania bufora, resetuje pozycje wyswietlania danych z bufora
extern void SetMoving(volatile DiodeMatrix *m, bool isMoving);
//! laduje do calej linijki bufora o danej wysokosci dana wartosc jasnosci
extern void SetYBuffer(volatile DiodeMatrix *m, uint8_t y_pos, uint8_t value);
//! zeruje caly bufor
extern void ClearBuffer(volatile DiodeMatrix *m);
//! kopiuje czesc zawartosci bufora obrotowego do bufora matrycy
extern void CopyFromRoundToBuffer(volatile DiodeMatrix *m, uint8_t y_round, uint8_t x_buffer);
//! zmniejsza pozycje oczyszczania bufora o 1
inline bool DecrementTo0SlowClear(volatile DiodeMatrix *m);
//! ustawia pozycje oczyszczania bufora na koniec zapisanego bufora
inline void RunSlowClearedPos(volatile DiodeMatrix *m);
//! ustawia wartosc binarna dla danego piksela
inline void SetPixelBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t y_pos, BinarySwitch *state);
/*
 *
 *		Definicje funkcji inline
 *
 */


/*! zwraca 1 jeœli nast¹pi³ reset jasnoœci i inkrementacja Y, 0 w przeciwnym wypadku
 *  czysci flagê modyfikacji
 * @param			m adres struktury macierzy LED
 *  @return 		1 jesli doszlo do zwiekszenia wspolrzednej Y, 0 w przeciwnym wypadku*/
inline volatile bool IncrementBrightness(volatile DiodeMatrix *m){
	//m->bModifyFlag = false;
	if (++m->uiBrightCount > MAX_MATRIX_BRIGHTNESS) {
		m->uiBrightCount = 0;
		IncrementY(m);
		return 1;
	}
	return 0;
}

/*! @param			m adres struktury macierzy LED*/
inline void IncrementY(volatile DiodeMatrix *m){
	if (++m->uiActY >= MATRIX_Y_SIZE) m->uiActY = 0;
	//m->bModifyFlag = true;
}

/*! @param			m adres struktury macierzy LED
 *  @return 		nowa wartosc szeregowa dla rejestru Y */
inline volatile BinarySwitch ReturnYValue(volatile DiodeMatrix *m) {
	return (m->uiActY) ? 0 : m->uiBrightness;
}

/*! @param			m adres struktury macierzy LED*/
inline void DiodeMatrixInit(volatile DiodeMatrix *m) {
	uint16_t i;
	for (i = 0; i < BUFFER_X_SIZE; i++)
		m->uitBufferX[i] = 0;
	m->uiBrightCount = 0;
	m->uiActY = 0;
	m->uiBrightness = 0;
	m->i16BufferPosition = 0;
	m->uiEndBufferPosition = MATRIX_X_SIZE;
	m->uiBrightness = gamma_o[1];

} // END inline void DiodeMatrixInit

/*! @param			m adres struktury macierzy LED*/
inline void RefreshBufferFlag(volatile DiodeMatrix *m) {
	register uint8_t i;
	register const uint8_t y_mask = 1 << m->uiActY;
	register int16_t adr;
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		adr = i+m->i16BufferPosition;
		m->uitBufferFlag[i] = 0;
		if ((adr >= 0) && (adr < BUFFER_X_SIZE)) {
			m->uitBufferFlag[i] = m->uitBufferX[adr] & y_mask;
		} else
			m->uitBufferFlag[i] = 0;
	}
} // END inline void RefreshBufferFlag

/*! @param			m adres struktury macierzy LED
 *  @return			true jesli zresetowano do poczatku*/
inline bool IncrementBufferPosition(volatile DiodeMatrix *m) {
	if (m->bIsMoving) {
		if (++m->i16BufferPosition > m->uiEndBufferPosition){
			m->i16BufferPosition = -MATRIX_X_SIZE;
			return true;
		}
		return false;
	} else {
		if (++m->i16BufferPosition > (m->uiEndBufferPosition - MATRIX_X_SIZE)) {
			m->i16BufferPosition = 0;
			return true;
		}
		return false;
	}
} // END inline void IncrementBufferPosition

/*! @param			m adres struktury macierzy LED
 *  @return			true jesli wlasnie osiagnieto poczatek, false w przeciwnym wypadku*/
inline bool DecrementTo0SlowClear(volatile DiodeMatrix *m) {
	if (m->uiSlowClearedPos) {
		m->uitBufferX[m->uiSlowClearedPos--] = 0;
		if (m->uiSlowClearedPos == 0)
			return true;
		return false;
	}
	return false;
} // END inline bool DecrementTo0SlowClear

/*! @param			m adres struktury macierzy LED*/
inline void RunSlowClearedPos(volatile DiodeMatrix *m) {
	m->uiSlowClearedPos = m->uiEndBufferPosition;
}

inline void SetPixelBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t y_pos, BinarySwitch *state) {
	register uint8_t y_mask = (1 << y_pos);
	m->uitBufferX[x_pos] &= ~y_mask;
	m->uitBufferX[x_pos] |= y_mask;
}

#endif /* DIODE_MATRIX_H_ */
