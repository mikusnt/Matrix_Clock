/*!
 * @file matrix_seq.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-02 $
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
#include "../devices/relay.h"
#include "../devices/bluetooth/mkuart.h"
#include "alphabet.h"


//! rozmiar bufora znakowego, 32 znaki i \0
#define TEXT_BUFFER_SIZE 33

//! pozycja dwukropka w buforze matrycy
#define SEC_SIGN_POS 12
//! ostania pozycja bloku binarnego sekundkina w buforze matrycy
#define SEC_END_POS 31
//! przejscie z liczby na liczbe z wg ASCII
#define DIGIT_ASCII 48


//! pozycje godzin i minut bloku binarnego sekundkina w buforze matrycy
extern uint8_t uitHMPos[4];
//! uniwersalny bufor danych tekstowych
//! @see uart_processing.h
extern char ctTextBuffer[TEXT_BUFFER_SIZE];

//! mozliwe sekwencje pracy urzadzenia
typedef enum {
	SeqTimer = 't',
	SeqADC = 'a',
	SeqText = 'e',
	SeqRelayNumber = 'n',
	SeqEmpty = 'c'
} ActualSeq;

//! dodatkowe zadania, ktore obsluguje urzadzenie
typedef enum {
	TaskSetPix = 'p',
	TaskRelay = 'r'
} DeviceTask;

//! ektualnie aktywna sekwencja
//! @see SetSeqParams
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
extern void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE]);
//! wczytuje godzine do bufora matrycy w oparciu o alfabet, efekt powolnej zmiany cyfr (RoundBuffer)
extern void LoadTimeToMatrix(volatile DiodeMatrix *m, TimeDate *from, TimeDate *to);
//! wczytuje date w formacie dd/mm/rrrr do bufora matrycy
extern void LoadDateToMatrix(volatile DiodeMatrix *m, TimeDate *time);
//! wczytuje liczbe calkowita w formacie 5 cyfr do bufora matrycy
extern void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number);
//! ustawia potrzebne parametry w przypadku zmiany sekwencji
extern void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, volatile Relay *relay);

#endif /* MATRIX_SEQUENCES_H_ */
