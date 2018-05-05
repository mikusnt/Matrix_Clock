/*!
 * @file diode_matrix.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy obslugujacy logiczna strukture macierzy LED sluzacej do wyswietlania danych.
 * @see		register.h
 */

#ifndef DIODE_MATRIX_H_
#define DIODE_MATRIX_H_

#include "../group_includes.h"

/*
 *
 *		Makroinstrukcje
 *
 */

//! rozmiar Y (pionowy)
#define MATRIX_Y_SIZE 8
//! rozmiar X (poziomy)
#define MATRIX_X_SIZE 32
//! rozmiar X (poziomy) bufora po ktorym algorytm porusza sie aby wyswietlic fragment
#define BUFFER_X_SIZE 32 * 8
//! maksymalna liczbowa jasnosc
#define MAX_MATRIX_BRIGHTNESS 19
//! maksymalna jesnosc wedlug korekcji gamma z tablicy gamma_o
#define MAX_GAMMA_BRIGHTNESS 4
//! czas w ms po ktorym nastepuje przesuniecie poczatkowego adresu bufora
#define INC_POS_MS 100

extern const uint8_t gamma_o[MAX_GAMMA_BRIGHTNESS+1];

/*
 *
 *		Glowny typ danych
 *
 */

//! glowna struktura macierzy LED
typedef struct {
	//! bufor jasnosci poszczegolnych diod matrycy LED, zmieniac manualnie
	uint8_t uitBufferX[BUFFER_X_SIZE];
	//! aktualna wartosc jasnosci, zmieniana przez funkcje
	//! @see IncrementBrightness
	uint8_t uiBrightCount;
	//! aktualna pozycja Y, zmieniana przez funkcje
	//! @see IncrementY
	uint8_t uiActY;
	//! bufor aktualnego dzialania LED
	uint8_t uitBufferFlag[MATRIX_X_SIZE];
	//! aktualna pozycja wyswietlania danych z bufora, zmieniana przez funkcje
	//! @see IncrementBufferPosition
	int16_t i16BufferPosition;
	//! indeks konca pozycji wyswietlania danych z bufora, zmieniac manualnie
	uint8_t uiEndBufferPosition;
	//! tryb wyswietlania, zmieniany manualnie
	//! true - wyswietlenie pustej matrycy, pojawienie sie zawartosci bufora i wyjscie
	//! calego mufora z matrycy
	//! false - wyswietlenie zawartosci bufora i przejscie do konca
	bool bIsMoving;
	uint8_t uitRoundBufferYX[2][5];
	//! pozycja oczyszczania bufora, ustawiana przez
	//! @see 	RunSlowClearedPos
	//! @see 	DecrementTo0SlowClear
	uint8_t uiSlowClearedPos;
	//! jasnosc calej matrycy
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
