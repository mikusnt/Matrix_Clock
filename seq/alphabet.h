/*!
 * @file alphabet.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-10 $
 * $Created: 2017-11-04 $
 * @version 2.0
 *
 * Uzyte piny procesora: 0<br>
 * Plik naglowkowy przechowujacy dane na temat potrzebnych liter alfabetu, tekstow z wykorzystaniem
 * pamieci EEPROM. Niekompatybilny z wersja 1.0
 * @see DiodeMatrix
 */

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include "../group_includes.h"

/*
 *
 *		Makroinstrukcje
 *
 */

//! ilosc znakow tablicy znakow alfabetu
#define ALPHABET_SIZE 89
//! ilosc znakow tablicy znakow animacji
#define ANIMATIONS_SIZE 7
//! maksymalny rozmiar tekstu dla znakow char ladowanych do bufora
#define MAX_TEXT_SIZE 30
//! ilosc tekstow w pamieci programu
#define TEXT_NUMBER 5

// odczytanie tablic z pamieci programu
//! odczytanie bajtow danych poszczegolnych liter
/*! @param 			y wspolrzedna y odpowiedzialna za nr znaku
 *  @param 			x wspolrzedna x odpowiedzialna za nr pionowej pozycji znaku
 *  @return wybrany pionowy bajt danych okreslonej litery*/
#define ALPHABET_YX(y, x) (pgm_read_byte(&(uiAlphabet[y][x])))
//! odczytanie szerokosci znaku
/*! @param 			y numer znaku
 *  @return 			szerokosc znaku*/
#define AL_LENGTH(y) (pgm_read_byte(&(uiAlLength[y])))
//! odczytanie bajtow danych poszczegolnych klatek animacji
/*! @param 			y wspolrzedna y odpowiedzialna za nr klatki
 *  @param 			x wspolrzedna x odpowiedzialna za nr pionowej pozycji klatki
 *  @return wybrany pionowy bajt danych okreslonej klatki animacji*/
#define ANIMATIONS_YX(y, x) (pgm_read_byte(&(uiAnimations[y][x])))

//! oznaczenia pozycji tablicy znakowej klatek animacji
typedef enum {
	MarioS0 = 0,
	MarioS1,
	MarioS2,
	MarioD0,
	MarioD1,
	Pacman0,
	Pacman1
} AnimationsSign;

/*
 *
 * 		Dane pamiêci FLASH
 *
 */
//! poszczegolne znaki alfabetu i znaki specjalne
extern const uint8_t uiAlphabet[ALPHABET_SIZE][5] PROGMEM;
//! ilosc bajtow pionowych dla znakow powyzszej tablicy
extern const uint8_t uiAlLength[ALPHABET_SIZE] PROGMEM;
//! poszczegolne znaki animacji Mario i Pacman
extern const uint8_t uiAnimations[ANIMATIONS_SIZE][7] PROGMEM;

/*
 *
 *		Funkcje
 *
 */

//! zwraca pionowa zawartosc okreslonego znaku z tablicy Alphabet
extern uint8_t LoadIntSignByte(uint8_t uiIntSign, uint8_t uiByteNr);
//! zwraca pionowa dlugosc okreslonego znaku z tablicy Alphabet
extern uint8_t LoadIntSignLength(uint8_t uiIntSign);
//! laduje do zmiennych informacje o znaku
extern void LoadSign(char cSign, uint8_t *uiStart_s, uint8_t *uiStop_s, uint8_t *uiIntSign_s);
//! zwraca pionowa zawartosc okreslonej sekwencji
extern uint8_t LoadAnimationsByte(AnimationsSign eSign, uint8_t uiByteNr);

//! laduje okreslony tekst z pamieci programu do bufora
extern void LoadText(char buffer[], uint8_t uiTextNr );

//extern const char string_1[MAX_TEXT_SIZE] PROGMEM;
//extern const char string_2[MAX_TEXT_SIZE] PROGMEM;
//extern const char string_3[MAX_TEXT_SIZE] PROGMEM;
//extern const char string_4[MAX_TEXT_SIZE] PROGMEM;
//extern const char string_5[MAX_TEXT_SIZE] PROGMEM;
//extern PGM_P const string_table[TEXT_NUMBER] PROGMEM;
extern PGM_P const string_table[] PROGMEM;

//! podobnie jak LoadText, wieksza optymalizacja, NIEPRZETESTOWANA
extern void LoadText2(char buffer[], uint8_t uiTextNr );
#endif /* ALPHABET_H_ */
