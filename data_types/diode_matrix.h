/*!
 * @file diode_matrix.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-03-28 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy obslugujacy logiczna strukture macierzy LED sluzacej do wyswietlania danych.
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
#define BUFFER_X_SIZE 32 * 4
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
	uint8_t uitBufferYX[MATRIX_Y_SIZE][BUFFER_X_SIZE];
	//! aktualna wartosc jasnosci
	uint8_t uiActBrightness;
	//! aktualna pozycja Y
	uint8_t uiActY;
	//! bufor aktualnego dzialania LED
	BinarySwitch etBufferFlag[MATRIX_X_SIZE];
	//! flaga modyfikacji macierzy uitBufferFlag
	bool bModifyFlag;
	//! aktualna pozycja wyswietlania danych z bufora
	int16_t i16BufferPosition;
	//! indeks konca pozycji wyswietlania danych z bufora, zmieniac manualnie
	uint8_t uiEndBufferPosition;
	//! tryb wyswietlania
	//! true - wyswietlenie pustej matrycy, pojawienie sie zawartosci bufora i wyjscie
	//! calego mufora z matrycy
	//! false - wyswietlenie zawartosci bufora i przejscie do konca
	bool bIsMoving;
	uint8_t uitRoundBufferYX[MATRIX_Y_SIZE * 2][5];
} DiodeMatrix;

/*
 *
 *		Deklaracje funkcji
 *
 */

//! zwieksza o 1 aktualna jasnosc matrycy
inline volatile uint8_t IncrementBrightness(volatile DiodeMatrix *m);
//! zwieksza o 1 aktualna wspolrzedna Y
inline void IncrementY(volatile DiodeMatrix *m);
//! zwraca zawartosc rejestru Y na podstawie pozycji Y
inline volatile uint8_t ReturnYValue(volatile DiodeMatrix *m);
//! inicjalizacja struktury
inline void DiodeMatrixInit(volatile DiodeMatrix *m);
//! odswieza tablice dzialania LED dla danej pozycji Y
inline void RefreshBufferFlag(volatile DiodeMatrix *m);
//! zwieksza aktualna pozycje wyswietlania danych z bufora o 1, gdy wartosc osiagnie koncowa to reset do 0
inline void IncrementBufferPosition(volatile DiodeMatrix *m);
//! ustawia tryb wyswietlania bufora, resetuje pozycje wyzeietlania danych z bufora
extern void SetMoving(volatile DiodeMatrix *m, bool isMoving);
//! laduje do calej linijki bufora o danej wysokosci dana wartosc jasnosci
extern void SetYBuffer(volatile DiodeMatrix *m, uint8_t y_pos, uint8_t value);
//! laduje do calej linijki pionowej bufora o danej wspolrzednej x wartosc jasnosci
extern void SetXBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t signMask, uint8_t value);
//! laduje do calej linijki pionowej bufora obrotowego danej wspolrzednej x wartosc jasnosci
extern void SetXRoundBuffer(volatile DiodeMatrix *m, uint8_t x_pos, uint8_t y_pos, uint8_t signMask, uint8_t value);
//! zeruje caly bufor
extern void ClearBuffer(volatile DiodeMatrix *m);
extern void CopyFromRoundToBuffer(volatile DiodeMatrix *m, uint8_t y_round, uint8_t x_buffer);
/*
 *
 *		Definicje funkcji inline
 *
 */

// zwraca 1 jeœli nast¹pi³ reset jasnoœci i inkrementacja X
// czysci flagê modyfikacji
/*! @param			m adres struktury macierzy LED
 *  @return 		1 jesli doszlo do zwiekszenia wspolrzednej Y, 0 w przeciwnym wypadku*/
inline volatile uint8_t IncrementBrightness(volatile DiodeMatrix *m){
	m->bModifyFlag = false;
	if (++m->uiActBrightness > MAX_MATRIX_BRIGHTNESS) {
		m->uiActBrightness = 0;
		IncrementY(m);
		return 1;
	}
	return 0;
}
/*! @param			m adres struktury macierzy LED*/
inline void IncrementY(volatile DiodeMatrix *m){
	if (++m->uiActY >= MATRIX_Y_SIZE) m->uiActY = 0;
	m->bModifyFlag = true;
}

/*! @param			m adres struktury macierzy LED
 *  @return 		nowa wartosc szeregowa dla rejestru Y */
inline volatile uint8_t ReturnYValue(volatile DiodeMatrix *m) {
	return (m->uiActY) ? 0 : 1;
}

/*! @param			m adres struktury macierzy LED
 *  @return 		tablica dzialania poziomej linijki LED*/
inline volatile BinarySwitch* ReturnYBufferFlag(volatile DiodeMatrix *m) {
	return m->etBufferFlag;
}

/*! @param			m adres struktury macierzy LED*/
inline void DiodeMatrixInit(volatile DiodeMatrix *m) {
	uint8_t i, j;
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		for (j = 0; j < MATRIX_Y_SIZE; j++) {
			m->uitBufferYX[j][i] = 0;

		}
		m->etBufferFlag[i] = OFF;
	}
	m->uiActBrightness = 0;
	m->uiActY = 0;
	m->bModifyFlag = 0;
	m->i16BufferPosition = 0;
	m->uiEndBufferPosition = MATRIX_X_SIZE;

} // END inline void DiodeMatrixInit

/*! @param			m adres struktury macierzy LED*/
inline void RefreshBufferFlag(volatile DiodeMatrix *m) {
	uint8_t i;
	for (i = 0; i < MATRIX_X_SIZE; i++) {
		int16_t adr = i+m->i16BufferPosition;
		if ((adr >= 0) && (adr < BUFFER_X_SIZE) && (m->uitBufferYX[m->uiActY][adr] > m->uiActBrightness)) {
			if (!m->etBufferFlag[i]) {
				m->bModifyFlag = true;
				m->etBufferFlag[i] = ON;
			}
		} else {
			if (m->etBufferFlag[i]) {
				m->bModifyFlag = true;
				m->etBufferFlag[i] = OFF;
			}
		}
	}
} // END inline void RefreshBufferFlag

/*! @param			m adres struktury macierzy LED*/
inline void IncrementBufferPosition(volatile DiodeMatrix *m) {
	if (m->bIsMoving) {
		if (++m->i16BufferPosition > m->uiEndBufferPosition)
			m->i16BufferPosition = -MATRIX_X_SIZE;
	} else {
		if (++m->i16BufferPosition > (m->uiEndBufferPosition - MATRIX_X_SIZE))
			m->i16BufferPosition = 0;
	}
} // END inline void IncrementBufferPosition

#endif /* DIODE_MATRIX_H_ */
