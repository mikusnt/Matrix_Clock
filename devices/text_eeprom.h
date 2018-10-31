/*!
 * @file text_eeprom.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-31 $
 * $Created: 2018-10-16 $
 * @version 1.1
 *
 * Used uC pins: 0<br>
 * Header file containing read and write texts to display.
 */

#ifndef DEVICES_TEXT_EEPROM_H_
#define DEVICES_TEXT_EEPROM_H_

#include "../group_includes.h"
#include "../seq/matrix_seq.h"

/*
 *
 *		Macros
 *
 */

//! number of texts to read and write to EEPROM
#define TEXT_EEPROM_SIZE 6

/*
 *
 *		Data types
 *
 */

//! table of texts to read and write in EEPROM memory, one text length the same as RAM text buffer
extern char EEMEM eeTextView[TEXT_EEPROM_SIZE][TEXT_BUFFER_SIZE];
//! table of texts to read and write in RAM memory, one text length the same as RAM text buffer
extern char ctTextView[TEXT_EEPROM_SIZE][TEXT_BUFFER_SIZE];

//! table of config data of eeTextView in EEPROM memory, first is start minute, second is period in minutes
extern uint8_t EEMEM eeTextParams[TEXT_EEPROM_SIZE][2];
//! table of config data of eeTextView in RAM memory, first is start minute, second is period in minutes
extern uint8_t uitTextViewParams[TEXT_EEPROM_SIZE][2];

/*
 *
 *		Declaration of functions
 *
 */

//! write all data of one text from RAM to EEPROM
extern void WriteTextToEEProm(uint8_t adr);
//! read all text data from EEPROM to RAM
extern void TextEEPromInit();

#endif /* DEVICES_TEXT_EEPROM_H_ */
