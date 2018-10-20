/**
 * @file alphabet_codes.h
 * @author		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * 				
 * $Created: 2018-10-20 $
 * Header file containing byte pixels table (Alphabet) of chars and chars byte length in PROGMEM. <br>
 * Created by Alphabet_Generator.
 */
#ifndef SEQ_ALPHABET_CODES_H_
#define SEQ_ALPHABET_CODES_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

//! number of chars in alphabet table
#define ALPHABET_SIZE 256

//! all alphabet chars and special chars
extern const uint8_t PROGMEM uiAlphabet[ALPHABET_SIZE][5];
//! vertical length of uiAlphabet chars
extern const uint8_t PROGMEM uiAlLength[ALPHABET_SIZE];

#endif /* SEQ_ALPHABET_CODES_H_ */
