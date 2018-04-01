#include "uart_processing.h"
static inline void RelayThreeToOne(char *buffer) {
	buffer[0] = (buffer[1] - DIGIT_ASCII) * 100;
	buffer[0] += (buffer[2] - DIGIT_ASCII) * 10;
	buffer[0] += (buffer[3] - DIGIT_ASCII);
}


extern void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, TimeDate *time, uint8_t brightness) {
	// odczytaj polecenie jesli sa nieodczytane dane
	if (UART_FirstEndFlag && IsUnreadData()) {
		register uint8_t i = 0;
		uint8_t uiEndCode = GOOD_COMMAND;
		uint8_t uiCode;

		uiCode = uart_getc();

		// zaladuj dane polecenia do bufora
		while (IsUnreadData() && (i < TEXT_BUFFER_SIZE)) {
			if ((ctTextBuffer[i] = uart_getc()) == END_FRAME_CODE) {
				ctTextBuffer[i] = 0;
				if (!IsUnreadData())
					UART_FirstEndFlag = false;
				break;
			}
			i++;
		}
		// wyslanie zwrotne polecenia
		uart_putc(uiCode);
		uart_puts(ctTextBuffer);
		uart_putc(' ');
		// obsluga poszczegolnych komend
		switch (uiCode) {
			case MODIFY_SEQ_CODE: {
				if ((ctTextBuffer[0] == SeqTimer) || (ctTextBuffer[0] == SeqADC)
						|| (ctTextBuffer[0] == SeqText) || (ctTextBuffer[0] == SeqRelayNumber)
						|| (ctTextBuffer[0] == SeqEmpty)) {
					if ((ctTextBuffer[0] == SeqText) && (ctTextBuffer[1] == 0)) {
						uiEndCode = ERROR_PARAMS;
						break;
					}
					eActualSeq = ctTextBuffer[0];
					if (eActualSeq == SeqText)
						ctTextBuffer[0] = ' ';
					if (eActualSeq == SeqRelayNumber)
						RelayThreeToOne(ctTextBuffer);
					RunSlowClearedPos(m);
				} else
					uiEndCode = ERROR_COMMAND;
			} break;
			case DEVICE_TASK_CODE: {
				switch (ctTextBuffer[0]) {
					case TaskSetPix: {
						uint8_t y_pos = ctTextBuffer[1] - DIGIT_ASCII;
						uint8_t x_pos = (ctTextBuffer[2] - DIGIT_ASCII) * 10 + (ctTextBuffer[3] - DIGIT_ASCII);
						uint8_t brightness = (ctTextBuffer[4] - DIGIT_ASCII);
						if ((y_pos < MATRIX_Y_SIZE) && (x_pos < MATRIX_X_SIZE)
								&& (brightness <= MAX_GAMMA_BRIGHTNESS))
							m->uitBufferYX[y_pos][x_pos] = gamma_o[brightness];
						else {
							uiEndCode = ERROR_PARAMS;
							break;
						}
					} break;
					case TaskRelay: {
						RelayThreeToOne(ctTextBuffer);
						RelayStartClicking(relay, ctTextBuffer[0], RelayDataNumber);
					} break;
					default: { uiEndCode = ERROR_COMMAND; } break;
				}
			} break;
			case LOAD_DATE_CODE: {
				uint8_t day = (ctTextBuffer[0] - DIGIT_ASCII) * 10 + (ctTextBuffer[1] - DIGIT_ASCII);
				// myslnik
				uint8_t month = (ctTextBuffer[3] - DIGIT_ASCII) * 10 + (ctTextBuffer[4] - DIGIT_ASCII);
				// myslnik
				uint8_t year = (ctTextBuffer[6] - DIGIT_ASCII) * 100
						+(ctTextBuffer[7] - DIGIT_ASCII) * 10 + (ctTextBuffer[8] - DIGIT_ASCII);
				// spacja
				uint8_t hour = (ctTextBuffer[10] - DIGIT_ASCII) * 10 + (ctTextBuffer[11] - DIGIT_ASCII);
				// dwukropek
				uint8_t minute = (ctTextBuffer[13] - DIGIT_ASCII) * 10 + (ctTextBuffer[14] - DIGIT_ASCII);
				// dwukropek
				uint8_t second = (ctTextBuffer[16] - DIGIT_ASCII) * 10 + (ctTextBuffer[17] - DIGIT_ASCII);

				if ((day <= 31) && (month <= 12) && (year < 200) && (hour < 24) && (minute < 60)
						&& (second < 60)) {
					DS3231_SetDate(day, month, year);
					DS3231_SetTime(hour, minute, second);
					DS3231_GetDate(&time->uiDays, &time->uiMonths, &time->uiYears);
				} else
					uiEndCode = ERROR_PARAMS;
			} break;
			case SPECTRUM_CODE: {
			} break;
			case MUSIC_CODE: {
			} break;
			default: { uiEndCode = ERROR_COMMAND; } break;

		}
		// wyslanie zwrotne kodu wykonania
		switch(uiEndCode) {
			case GOOD_COMMAND: { uart_puts_p(PSTR("ok")); } break;
			case ERROR_COMMAND: { uart_puts_p(PSTR("undefined command"));} break;
			case ERROR_PARAMS: {uart_puts_p(PSTR("incorrect params")); } break;
		}
		uart_putc('\n');

	}


}
