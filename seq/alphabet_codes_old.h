/*!
 * @file alphabet_codes.h
 * @author		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 *
 * $Created: 2018-05-18 $
 * Header file containing byte pixels table (Alphabet) of chars and chars byte length in PROGMEM.
 */
/*#ifndef SEQ_ALPHABET_CODES_OLD_H_
#define SEQ_ALPHABET_CODES_OLD_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

//! number of chars in alphabet table
#define ALPHABET_SIZE 91

//! all alphabet chars and special chars
extern const uint8_t uiAlphabet[ALPHABET_SIZE][5] PROGMEM;
//! vertical length of uiAlphabet chars
extern const uint8_t uiAlLength[ALPHABET_SIZE] PROGMEM;

#endif /* SEQ_ALPHABET_CODES_OLD_H_ */
