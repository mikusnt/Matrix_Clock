#include "uart_processing.h"

void TryLoadCommand() {
	uint8_t uiCode, uiTemp;
	uint8_t dataBuffer[MAX_COMMAND_LENGTH];
	uint8_t i = 0;

	// odczytaj polecenie jesli sa nieodczytane dane
	if (IsUnreadData()) {
		uiCode = uart_getc();

		// zaladuj dane polecenia do bufora
		while (IsUnreadData() && (i < MAX_COMMAND_LENGTH)) {
			uiTemp = uart_getc();
			if (uiTemp == END_FRAME_CODE) break;
			else dataBuffer[i++] = uiTemp;

		}
		// obsluga poszczegolnych komend
		switch (uiCode) {
			case MODIFY_SEQ_CODE: {
			} break;
			case DEVICE_MODE_CODE: {
			} break;
			case LOAD_DATE_CODE: {
			} break;
			case LOAD_TEXT_CODE: {
			} break;
			case SPECTRUM_CODE: {
			} break;
			case MUSIC_CODE: {
			} break;
		}
	}


}
