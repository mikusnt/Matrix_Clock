/*!
 * @file matrix_seq.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2017-03-29 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy zawierajacy metody wypelniania matrycy LED danymi.
 * @see DiodeMatrix
 * @see alphabet.h
 */


#ifndef MATRIX_SEQ_H_
#define MATRIX_SEQ_H_

#include "../group_includes.h"
#include "../data_types/diode_matrix.h"
#include "../data_types/date_time.h"
#include "../devices/other_devices.h"
#include "../devices/bluetooth/uart_processing.h"
#include "../devices/relay.h"
#include "alphabet.h"

//! ilosc sekwencji
#define SEQ_NUMBER 4

//! rozmiar bufora znakowego, 32 znaki i \0
#define TEXT_BUFFER_SIZE 33

//! pozycja dwukropka w buforze matrycy
#define SEC_SIGN_POS 12

//! ostania pozycja bloku binarnego sekundkina w buforze matrycy
#define SEC_END_POS 31

#define NUMBER_TO_ASCII 48

//! pozycje godzin i minut bloku binarnego sekundkina w buforze matrycy
extern uint8_t uitHMPos[4];

extern char ctTextBuffer[TEXT_BUFFER_SIZE];

typedef enum {
	SeqTimer = 1,
	SeqADC,
	SeqText,
	SeqRelayNumber,
	SeqManualPix
} ActualSeq;

typedef enum {
	ModeSetPix = 1,
	ModeRelay
} DeviceMode;

extern ActualSeq eActualSeq;

/*
 *
 * 		Funkcje wewnetrzne
 *
 */
// laduje do bufora matrycy sekundy w binarnym kodzie BCD
//static inline void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds, uint8_t brightness)
// laduje do bufora matrycy wybrany znak ASCII
//static uint8_t InsertCharToMatrix(volatile DiodeMatrix *m, char sign, uint8_t position, uint8_t brightness)
// laduje do bufora obrotowego matrycy wskazany znak na odpowiedniej pozycji
//static void InsertCharToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos, uint8_t brightness)
// laduje do bufora matrycy informacje o wartosci ADC w postaci kropek na pozycji 7
//void LoadADCToMatrix(volatile DiodeMatrix *m, uint16_t adcValue, uint8_t brightness)

/*
 *
 *		Funkcje zewnetrzne
 *
 */
//! wczytuje dany tekst do buf0ra matrycy w oparciu o alfabet
extern void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE], uint8_t brightness);
//! wczytuje godzine do bufora matrycy w oparciu o alfabet, efekt powolnej zmiany cyfr (RoundBuffer)
extern void LoadTimeToMatrix(volatile DiodeMatrix *m, TimeDate *from, TimeDate *to, uint8_t brightness);
//! wczytuje date w formacie dd/mm/rrrr do bufora matrycy
extern void LoadDateToMatrix(volatile DiodeMatrix *m, TimeDate *time, uint8_t brightness);
//! wczytuje liczbe calkowita w formacie 5 cyfr do bufora matrycy
extern void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number, uint8_t brightness);
//! ustawia potrzebne parametry w przypadku zmiany sekwencji
extern void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, volatile Relay *relay, uint8_t brightness);

#endif /* MATRIX_SEQUENCES_H_ */
