/*!
 * @file text_eeprom.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see text_eeprom.h*/

#include "text_eeprom.h"

char EEMEM eeText[TEXT_EEPROM_SIZE][TEXT_BUFFER_SIZE] = {"Matrix Clock PixBit v0.952 by MiSt", "", "", "", "", ""};

void WriteTextToEEProm(char *buffer, uint8_t bufferSize, uint8_t adr) {
	if (adr < TEXT_EEPROM_SIZE) {
		eeprom_update_block(buffer, &eeText[adr], bufferSize);
		eeprom_busy_wait();
	}
} // END void WriteTextToEEProm

void ReadTextFromEEProm(char *buffer, uint8_t bufferSize, uint8_t adr) {
	if (adr < TEXT_EEPROM_SIZE) {
		eeprom_read_block(buffer, &eeText[adr], bufferSize);
		eeprom_busy_wait();
	} else {
		buffer[0] = 0;
	}
}
