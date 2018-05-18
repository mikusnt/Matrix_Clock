/*
 * @file alphabet_codes.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-05-18 $
 * $Created: 2018-05-18 $
 * @version 0.1
 */
#ifndef SEQ_ALPHABET_CODES_H_
#define SEQ_ALPHABET_CODES_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

//! number of chars in alphabet table
#define ALPHABET_SIZE 91

//! all alphabet chars and special chars
extern const uint8_t uiAlphabet[ALPHABET_SIZE][5] PROGMEM;
//! vertical length of uiAlphabet chars
extern const uint8_t uiAlLength[ALPHABET_SIZE] PROGMEM;

#endif /* SEQ_ALPHABET_CODES_H_ */
