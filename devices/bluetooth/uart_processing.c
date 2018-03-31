#include "uart_processing.h"

void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, uint8_t brightness) {
	uint8_t uiTemp, uiCode;
	uint8_t i = 0;
	uint8_t uiEndCode = GOOD_COMMAND;
	// odczytaj polecenie jesli sa nieodczytane dane
	if (IsUnreadData() && UART_FirstEndFlag) {
		uiCode = uart_getc();

		// zaladuj dane polecenia do bufora
		while (IsUnreadData() && (i < MAX_COMMAND_LENGTH)) {
			uiTemp = uart_getc();
			if (uiTemp == END_FRAME_CODE) {
				ctTextBuffer[i] = 0;
				if (!IsUnreadData())
					UART_FirstEndFlag = false;
				break;
			}
			else ctTextBuffer[i++] = uiTemp;

		}
		// wyslanie zwrotne polecenia
		uart_putc(uiCode);
		uart_puts(ctTextBuffer);

		// obsluga poszczegolnych komend
		switch (uiCode) {
			case MODIFY_SEQ_CODE: {
				if (ctTextBuffer[0] <= SeqManualPix) {
					if ((ctTextBuffer[0] == SeqText) && (ctTextBuffer[1] == 0)) {
						uiEndCode = ERROR_PARAMS;
						break;
					}
					eActualSeq = ctTextBuffer[0];
					if (eActualSeq == SeqText)
						ctTextBuffer[0] = ' ';
					RunSlowClearedPos(m);
				} else
					uiEndCode = ERROR_COMMAND;
			} break;
			case DEVICE_MODE_CODE: {
				switch (ctTextBuffer[0]) {
					case ModeSetPix: {
						if (((ctTextBuffer[1] - 1) < MATRIX_Y_SIZE) && (ctTextBuffer[2] - 1) < MATRIX_X_SIZE
								&& ((ctTextBuffer[3] - 1) < MAX_GAMMA_BRIGHTNESS))
							m->uitBufferYX[ctTextBuffer[1] - 1][ctTextBuffer[2] - 1] = gamma_o[ctTextBuffer[3] - 1];
						else {
							uiEndCode = ERROR_PARAMS;
							break;
						}
					} break;
					case ModeRelay: {
						RelayStartClicking(relay, ctTextBuffer[1] - 128, RelayDataNumber);
					} break;
					default: { uiEndCode = ERROR_COMMAND; } break;
				}
			} break;
			case LOAD_DATE_CODE: {
			} break;
			case SPECTRUM_CODE: {
			} break;
			case MUSIC_CODE: {
			} break;
			default: { uiEndCode = ERROR_COMMAND; } break;

		}
		// wyslanie zwrotne kodu wykonania
		uart_putc(uiEndCode);
		uart_putc('\n');

	}


}
