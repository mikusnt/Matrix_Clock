/*!
 * @file text_eeprom.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-10-20 $
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

#define TEXT_EEPROM_SIZE 6

extern char EEMEM eeTextView[TEXT_EEPROM_SIZE][TEXT_BUFFER_SIZE];
extern char ctTextView[TEXT_EEPROM_SIZE][TEXT_BUFFER_SIZE];

// starts and period in minutes
extern uint8_t EEMEM eeTextParams[TEXT_EEPROM_SIZE][2];
extern uint8_t uitTextViewParams[TEXT_EEPROM_SIZE][2];

extern void WriteTextToEEProm(uint8_t adr);
extern void TextEEPromInit();

#endif /* DEVICES_TEXT_EEPROM_H_ */
