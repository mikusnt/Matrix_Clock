/*!
 * @file diode_matrix.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-11-10 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * NIETESTOWANY!!!<br>
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy obslugujacy logiczna strukture marzierzy LED sluzacej do wyswietlania danych.
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
//! maksymalna liczbowa jasnosc
#define MAX_MATRIX_BRIGHTNESS 20


/*
 *
 *		Glowny typ danych
 *
 */

//! glowna struktura macierzy LED
typedef struct {
	//! bufor jasnosci poszczegolnych diod matrycy LED
	uint8_t uitBufferYX[MATRIX_Y_SIZE][MATRIX_X_SIZE];
	//! aktualna wartosc jasnosci
	uint8_t uiActBrightness;
	//! aktualna pozycja Y
	uint8_t uiActY;
	//! bufor aktualnego dzialania LED
	BinarySwitch etBufferFlag[MATRIX_Y_SIZE][MATRIX_X_SIZE];
	//! flaga modyfikacji macierzy uitBufferFlag
	bool bModifyFlag;
} DiodeMatrix;

/*
 *
 *		Deklaracje funkcji
 *
 */

//! zwieksza o 1 aktualna jasnosc matrycy
inline uint8_t IncrementBrightness(DiodeMatrix *m);
//! zwieksza o 1 aktualna wspolrzedna Y
inline void IncrementY(DiodeMatrix *m);
//! zwraca zawartosc rejestru Y na podstawie pozycji Y
inline uint8_t ReturnYValue(DiodeMatrix *m);
//! zwraca tablice dzialania LED dla danej pozycji Y
inline BinarySwitch* ReturnOneYBufferFlag(DiodeMatrix *m);
//! inicjalizacja struktury
inline void DiodeMatrixInit(DiodeMatrix *m);
//! odswieza tablice dzialania LED dla danej pozycji Y
inline void RefreshBufferFlag(DiodeMatrix *m);

/*
 *
 *		Definicje funkcji inline
 *
 */

// zwraca 1 jeœli nast¹pi³ reset jasnoœci i inkrementacja X
// czysci flagê modyfikacji
/*! @param			m adres struktury macierzy LED
 *  @return 		1 jesli doszlo do zwiekszenia wspolrzednej Y, 0 w przeciwnym wypadku*/
inline uint8_t IncrementBrightness(DiodeMatrix *m){
	m->bModifyFlag = false;
	if (++m->uiActBrightness >= MAX_MATRIX_BRIGHTNESS) {
		m->uiActBrightness = 0;
		IncrementY(m);
		return 1;
	}
	return 0;
}
/*! @param			m adres struktury macierzy LED*/
inline void IncrementY(DiodeMatrix *m){
	if (++m->uiActY >= MATRIX_Y_SIZE) m->uiActY = 0;
	m->bModifyFlag = true;
}

/*! @param			m adres struktury macierzy LED
 *  @return 		nowa wartosc szeregowa dla rejestru X */
inline uint8_t ReturnYValue(DiodeMatrix *m) {
	return (m->uiActY) ? 0 : 1;
}

/*! @param			m adres struktury macierzy LED
 *  @return 		tablica dzialania poziomej linijki LED*/
inline BinarySwitch* ReturnOneYBufferFlag(DiodeMatrix *m) {
	return m->etBufferFlag[m->uiActY];
}

/*! @param			m adres struktury macierzy LED*/
inline void DiodeMatrixInit(DiodeMatrix *m) {
	uint8_t i, j;
	for (i = 0; i < MATRIX_Y_SIZE; i++) {
		for (j = 0; j < MATRIX_X_SIZE; j++) {
			m->uitBufferYX[i][j] = 0;
			m->etBufferFlag[i][j] = 0;
		}
	}
	m->uiActBrightness = 0;
	m->uiActY = 0;
	m->bModifyFlag = 0;
} // END inline void DiodeMatrixInit

/*! @param			m adres struktury macierzy LED*/
inline void RefreshBufferFlag(DiodeMatrix *m) {
	uint8_t i;
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		if (m->uitBufferYX[m->uiActY][i] > m->uiActBrightness) {
			if (!m->etBufferFlag[m->uiActY][i]) m->bModifyFlag = true;
			m->etBufferFlag[m->uiActY][i] = ON;
		} else {
			if (m->etBufferFlag[m->uiActY][i]) m->bModifyFlag = true;
			m->etBufferFlag[m->uiActY][i] = OFF;
		}
	}
} // END inline void RefreshBufferFlag

#endif /* DIODE_MATRIX_H_ */
