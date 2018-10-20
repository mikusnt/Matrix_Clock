#include "uart_processing.h"

//! zamienia znaki ASCII trzech cyfr na liczbe i zapisuje do poczatku bufora jesli
//! miesci sie na jednym bajcie
//! @return 		dwubajtowa liczba bedaca wynikiem scalania ASCII
//! rename ASCII chars last three signs and save to beginning of buffer
//! @return 		two byte number from ASCII codes
static inline uint16_t RelayThreeToOne(char buffer[4]) {
	uint16_t temp  = (buffer[1] - DIGIT_ASCII) * 100;
	temp += (buffer[2] - DIGIT_ASCII) * 10;
	temp += (buffer[3] - DIGIT_ASCII);
	buffer[0] = temp;
	return temp;
}


extern void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, TimeDate *time) {
	// load command when are unreaded data
	if (UART_FirstEndFlag && IsUnreadData()) {
		register uint8_t i = 0;
		uint8_t uiEndCode = GOOD_COMMAND;
		uint8_t uiCode = ' ';

		while (uiCode == ' ')
			uiCode = uart_getc();

		// load command parameters to buffer
		while (IsUnreadData() && (i < TEXT_BUFFER_SIZE)) {
			if (((ctTextBuffer[i] = uart_getc()) == END_FRAME_CODE)
				|| (ctTextBuffer[i] == 0)) {
				ctTextBuffer[i] = 0;
				if (!IsUnreadData())
					UART_FirstEndFlag = false;
				break;
			}
			i++;
		}
		// when buffer is full empty read UART buffer
		if (i >= TEXT_BUFFER_SIZE) {
			while (uart_getc() != END_FRAME_CODE);
		}
		// DEBUG info
		/*uart_putc(uiCode);
		uart_puts(ctTextBuffer);
		uart_putc(' ');*/

		// realize the commands
		switch (uiCode) {
			case MODIFY_SEQ_CODE: {
				if ((ctTextBuffer[0] == SeqTimer) || (ctTextBuffer[0] == SeqADC)
						|| (ctTextBuffer[0] == SeqText) || (ctTextBuffer[0] == SeqRelayNumber)
						|| (ctTextBuffer[0] == SeqEmpty) || (ctTextBuffer[0] == SeqTextDebug)
						|| (ctTextBuffer[0] == SeqBomb) || (ctTextBuffer[0] == SeqDeCounter)) {
					if ((ctTextBuffer[0] == SeqText) && (ctTextBuffer[1] == 0)) {
						uiEndCode = ERROR_PARAMS;
						break;
					}
					eActualSeq = ctTextBuffer[0];
					if ((eActualSeq == SeqText) || (eActualSeq == SeqTextDebug)) {
						// przesuwanie napisu o 1 w lewo
						// shift buffer to left
						i = 0;
						while(ctTextBuffer[i] != 0) {
							ctTextBuffer[i] = ctTextBuffer[i + 1];
							if ((++i >= 5) && (eActualSeq == SeqTextDebug)) {
								ctTextBuffer[5] = 0;
								break;
							}
						}
					} else if (eActualSeq == SeqRelayNumber) {
						if ((i < 4) || (RelayThreeToOne(ctTextBuffer) > 255)) {
							uiEndCode = ERROR_PARAMS;
							break;
						}
					} else if (eActualSeq == SeqBomb) {
						if (i > 2) {
							uiEndCode = ERROR_PARAMS;
							break;
						}
						ctTextBuffer[1] = ctTextBuffer[1] - DIGIT_ASCII;

					} else if (eActualSeq == SeqDeCounter) {
						if (i != 7) {
							uiEndCode = ERROR_PARAMS;
							break;
						}
						// days
						ctTextBuffer[1] = ((ctTextBuffer[1] - DIGIT_ASCII) * 10) + ctTextBuffer[2] - DIGIT_ASCII;
						// hours
						ctTextBuffer[2] = ((ctTextBuffer[3] - DIGIT_ASCII) * 10) + ctTextBuffer[4] - DIGIT_ASCII;
						// minutes
						ctTextBuffer[3] = ((ctTextBuffer[5] - DIGIT_ASCII) * 10) + ctTextBuffer[6] - DIGIT_ASCII;
						if ((ctTextBuffer[1] > 41) || (ctTextBuffer[2] > 23) || (ctTextBuffer[3] > 59)) {
							uiEndCode = ERROR_PARAMS;
							break;
						}
					}
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
							if ((i >= 5) && (y_pos < MATRIX_Y_SIZE) && (x_pos < MATRIX_X_SIZE)) {
								m->uitBufferX[x_pos] &= ~(1 << y_pos);
								m->uitBufferX[x_pos] |= (brightness > 0) << y_pos;
							}
							else
								uiEndCode = ERROR_PARAMS;
						} break;
						case TaskRelayNumber: {
							if (relay->eState == RelayOFF)
								uart_puts_p(PSTR("Relay disabled "));
							if ((i == 4) && (RelayThreeToOne(ctTextBuffer) <= 255)) {
								RelayStartClicking(relay, ctTextBuffer[0], RelayDataNumber);
							} else if ((i == 5) && (ctTextBuffer[0] = ctTextBuffer[1]) && (RelayThreeToOne(ctTextBuffer+1) <= 255)){
								RelayClicking(relay, ctTextBuffer[0] - DIGIT_ASCII, ctTextBuffer[1]);
							} else {
								uiEndCode = ERROR_PARAMS;
							}
						} break;
						case TaskRelayMode: {
							uint8_t mode = ctTextBuffer[1] - DIGIT_ASCII;
							if ((i >= 2) && (mode < 3)) {
								SetRelayState(relay, mode);
							} else
								uiEndCode = ERROR_PARAMS;
						} break;
						case TaskWriteText: {
							uint8_t addr = ctTextBuffer[1] - DIGIT_ASCII;
							if ((i >= 2) && (addr < TEXT_EEPROM_SIZE)) {
								memcpy(&ctTextView[addr], ctTextBuffer+2, TEXT_BUFFER_SIZE-2);
								WriteTextToEEProm(addr);
							} else
								uiEndCode = ERROR_PARAMS;
						} break;
						case TaskSetText: {
							uint8_t addr = ctTextBuffer[1] - DIGIT_ASCII;
							uint8_t starts = ctTextBuffer[2] - DIGIT_ASCII;
							uint8_t period = ctTextBuffer[3] - DIGIT_ASCII;
							if ((i == 4) && (addr < TEXT_EEPROM_SIZE) && (starts < 10) && (period < 10)) {
								uitTextViewParams[addr][0] = starts;
								uitTextViewParams[addr][1] = period;
								WriteTextToEEProm(addr);
							} else {
								uiEndCode = ERROR_PARAMS;
							}
						} break;
						case TaskReadText: {
							uint8_t addr = ctTextBuffer[1] - DIGIT_ASCII;
							if ((i >= 2) && (addr < TEXT_EEPROM_SIZE)) {
								memcpy(ctTextBuffer, &ctTextView[addr], TEXT_BUFFER_SIZE);
								if (ctTextBuffer[0] == 0)
									uart_puts_p(PSTR("<empty>"));
								else {
									uart_puts_p(PSTR("\""));
									uart_puts(ctTextBuffer);
									uart_puts_p(PSTR("\""));
								}
								snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, " starts %d, period %d", uitTextViewParams[addr][0], uitTextViewParams[addr][1]);
								uart_puts(ctTextBuffer);
							} else
								uiEndCode = ERROR_PARAMS;
						} break;
						default: { uiEndCode = ERROR_COMMAND; } break;

					}
			} break;
			case LOAD_DATE_CODE: {
					uint8_t day = (ctTextBuffer[0] - DIGIT_ASCII) * 10 + (ctTextBuffer[1] - DIGIT_ASCII);
					// -
					uint8_t month = (ctTextBuffer[3] - DIGIT_ASCII) * 10 + (ctTextBuffer[4] - DIGIT_ASCII);
					// -
					uint8_t year = (ctTextBuffer[6] - DIGIT_ASCII) * 100
							+(ctTextBuffer[7] - DIGIT_ASCII) * 10 + (ctTextBuffer[8] - DIGIT_ASCII);
					// space
					uint8_t hour = (ctTextBuffer[10] - DIGIT_ASCII) * 10 + (ctTextBuffer[11] - DIGIT_ASCII);
					// :
					uint8_t minute = (ctTextBuffer[13] - DIGIT_ASCII) * 10 + (ctTextBuffer[14] - DIGIT_ASCII);
					// :
					uint8_t second = (ctTextBuffer[16] - DIGIT_ASCII) * 10 + (ctTextBuffer[17] - DIGIT_ASCII);

					if ((i >= 18) && (day <= 31) && (month <= 12) && (year < 200) && (hour < 24) && (minute < 60)
							&& (second < 60)) {
						DS3231_SetDate(day, month, year);
						DS3231_SetTime(hour, minute, second);
						DS3231_GetDate(&time->uiDay, &time->uiMonth, &time->uiYear);
						DS3231_GetTime(&time->uiHour, &time->uiMinute, &time->uiSecond);
						WriteDateTimeToEEProm(time);
					} else
						uiEndCode = ERROR_PARAMS;
			} break;
			case SPECTRUM_CODE: {
			} break;
			case MUSIC_CODE: {
			} break;
			default: { uiEndCode = ERROR_COMMAND; } break;
			case QUESTION_CODE: {
				uart_puts_p(PSTR("Hey! "));
			} break;
			case VERSION_CODE: {
				uart_puts_p(VERSION_TEXT);
			} break;
			case RESET_CODE: {
				uart_puts_p(PSTR("Reset ok\n"));
				m->uiBrightness = 0;
				D_MS(1);
				Reset_UC();
			} break;
			case MODIFY_DATE_CODE: {
				uart_puts_p(PSTR("Date saved to RTC: "));
				TimeDate sTime;
				ReadDateTimeFromEEProm(&sTime);
				DateToTextBuffer(&sTime, 0);
				uart_puts(ctTextBuffer);
			} break;

		}
		// sending response code
		switch(uiEndCode) {
			case GOOD_COMMAND: { uart_puts_p(PSTR("<ok>")); } break;
			case ERROR_COMMAND: { uart_puts_p(PSTR("<undefined command>"));} break;
			case ERROR_PARAMS: {uart_puts_p(PSTR("<incorrect params>")); } break;
		}
		uart_putc('\n');
	}
} // END extern void TryLoadCommand
