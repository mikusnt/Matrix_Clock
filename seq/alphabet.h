/*!
 * @file alphabet.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-29 $
 * $Created: 2017-11-04 $
 * @version 2.0
 *
 * Used uC pins: 0<br>
 * Header file containing byte alphabet of chars in PROGMEM. Not comapible with v1.0.
 * @see DiodeMatrix
 */

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include "../group_includes.h"
#include "alphabet_codes.h"

/*
 *
 *		Macros
 *
 */

//! number of signs in animation table
#define ANIMATIONS_SIZE 7
//! max text length in text buffer
#define MAX_TEXT_SIZE 30
//! number of texts in PROGMEM
#define TEXT_NUMBER 5

// load tables from PROGMEM
//! odczytanie bajtow danych poszczegolnych liter
//! load one data byte of char from alphabet
/*! @param 			y coordinate, number of char
 *  @param 			x coordinate, number of vertical byte of char
 *  @return one byte of char from alphabet*/
#define ALPHABET_YX(y, x) (pgm_read_byte(&(uiAlphabet[y][x])))
//! load char vertical length
/*! @param 			y number of char
 *  @return 		vertical length*/
#define AL_LENGTH(y) (pgm_read_byte(&(uiAlLength[y])))
//! load one data byte of animation from animation table
/*! @param 			y coordinate, code of animation
 *  @param 			x coordinate, number of vertical byte of animation
 *  @return one byte of animation from animation table*/
#define ANIMATIONS_YX(y, x) (pgm_read_byte(&(uiAnimations[y][x])))

//! name of animation in animation table
typedef enum {
	MarioS0 = 0,//!< MarioS0
	MarioS1,    //!< MarioS1
	MarioS2,    //!< MarioS2
	MarioD0,    //!< MarioD0
	MarioD1,    //!< MarioD1
	Pacman0,    //!< Pacman0
	Pacman1     //!< Pacman1
} AnimationsCode;

/*
 *
 * 		Tables in PROGMEM
 *
 */

//! animation table of Mario i Pacman
extern const uint8_t uiAnimations[ANIMATIONS_SIZE][7] PROGMEM;

/*
 *
 *		Funkcje
 *
 */

//! return one data byte of char from alphabet
extern uint8_t LoadIntSignByte(uint8_t uiIntSign, uint8_t uiByteNr);
//! load char vertical length
extern uint8_t LoadIntSignLength(uint8_t uiIntSign);
//! load all info about char
extern void LoadSign(char cSign, uint8_t *uiStart_s, uint8_t *uiStop_s, uint8_t *uiIntSign_s);
//! load one data byte of animation from animation table
extern uint8_t LoadAnimationsByte(AnimationsCode eSign, uint8_t uiByteNr);


extern const char string_1[MAX_TEXT_SIZE] PROGMEM;
extern const char string_2[MAX_TEXT_SIZE] PROGMEM;
extern const char string_3[MAX_TEXT_SIZE] PROGMEM;
extern const char string_4[MAX_TEXT_SIZE] PROGMEM;
extern const char string_5[MAX_TEXT_SIZE] PROGMEM;
extern PGM_P const string_table[TEXT_NUMBER] PROGMEM;

//! load text from PROGMEM to buffer
extern void LoadText(char buffer[], uint8_t uiTextNr );
#endif /* ALPHABET_H_ */
