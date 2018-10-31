/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl

 * $Modified: 2018-10-31 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Project main file
 * @see readme.md
 */

#include "group_includes.h"
#include "devices/relay.h"
#include "devices/ds3231.h"
#include "devices/register.h"
#include "devices/bluetooth/uart_processing.h"
#include "data_types/date_time.h"
#include "data_types/diode_matrix.h"
#include "devices/some_devices.h"
#include "devices/text_eeprom.h"
#include "seq/matrix_seq.h"

#include "tests.h"



/*
 *
 *		Variables
 *
 */

//! internal time in milliseconds, [0 - 1000]
volatile uint16_t ui16Ms = 0;
//! internal time in seconds, cooperate with ui16Ms
volatile uint8_t uiSeconds;
//! LED matrix struct
volatile DiodeMatrix matrix;
//! Relay struct
volatile Relay relay;
//! flag of new second, setting by DS3231 SQW overflow
//! @see ISR(PCINT1_vect)
volatile bool bNewTime = false;
//! flag of new half part of second, setting by DS3231 SQW overflow
//! @see ISR(PCINT1_vect)
volatile bool bHalfTime = false;
//! flag of new quater part ofsecond, setting by internal time from Timer2
//! @see ISR(TIMER2_COMPA_vect)
volatile bool bQuaterTime = false;
//! flag of refresh round buffer position
volatile bool bNewRoundRefresh = false;
//! ADC data about brightness
volatile ADCVoltageData adc;
//! time from RTC
TimeDate RTCTime;
//! actual time displaying on matrix
//! @see 	LoadTimeToMatrix
TimeDate actTime;
//! flag of new Y level of matrix
volatile uint8_t uivModifyY;
//! number of repetition of text on matrix
volatile int8_t iCountToTimer;
//! flag of decrement cleaning position on clearing mode
volatile bool bEnableDecrement;
//! flag of critical section on Timer0 register operations
volatile bool bCriticalY = false;

/*
 *
 *		Overflows
 *
 */

// ISR(TIMER0_COMPA_vect)
// ISR(TIMER1_COMPA_vect)
// ISR(TIMER2_COMPA_vect)
// ISR(ADC_vect)
// ISR(PCINT1_vect)

/*
 *
 *		Main function
 *
 */

int main (void) {
	/*
	 *
	 *		Init block
	 *
	 */
	USART_Init(__UBRR);
	uart_puts_p(PSTR("<Initialization>\n"));
	PowerReduction();
	RegistersInit();
	DiodeMatrixInit(&matrix);
	TextEEPromInit();
	RelayInit(&relay);
	Timer0Init();
	Timer2Init();
	PCINTInit();

	ADCInit(&adc);
	matrix.uiBrightness = 1;
	I2C_Init();

	DS3231_Init();
	//eActualSeq = SeqDeCounter;
	SetSeqParams(&matrix, &actTime, &RTCTime, &relay);
	RelayStartClicking(&relay, 15 * relay.eState, RelayDataMinutes);

	wdt_enable(WDTO_120MS);
	DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
	DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
	LoadToSingleTime(&RTCTime);

	sei();
	uart_puts_p(PSTR("<Running main loop>\n"));
	/*
	 *
	 *		Main loop
	 *
	 */

	while(1) {
		wdt_reset();
		// trying read and execute full UART command
		TryLoadCommand(&matrix, &relay, &RTCTime);
		// clearing buffer, change seq to eActualSeq after decrement to 0
		if (bEnableDecrement) {
			if (DecrementTo0SlowClear(&matrix)) {
				SetSeqParams(&matrix, &actTime, &RTCTime, &relay);
				iCountToTimer = 2;

			}
			bEnableDecrement = false;
		}

		// when clearing buffer mode deactiwated
		if (matrix.uiSlowClearedPos == 0) {
			switch(eActualSeq) {
				// Timer mode
				case SeqTimer: {
					if (bNewTime) {
						DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
						LoadToSingleTime(&RTCTime);

						if ((RTCTime.uiSecond % 10) == 0) {
							RTCToTextBuffer(&RTCTime, '\n');
							uart_puts(ctTextBuffer);
						}

						// date and texts from eeprom buffer, first good cancels others in the same minute
						for (uint8_t i = 0; i < TEXT_EEPROM_SIZE; i++) {
							if ((RTCTime.uiSecond == 10) && (uitTextViewParams[i][1] > 0) && ((RTCTime.uiMinute % uitTextViewParams[i][1]) == uitTextViewParams[i][0])) {
								if (i == 0) {
									DateToTextBuffer(&RTCTime, 0);
								} else {
									memcpy(ctTextBuffer, &ctTextView[i], TEXT_BUFFER_SIZE);
								}
								eActualSeq = SeqText;
								RunSlowClearedPos(&matrix);
								break;
							}
						}
						// display date every 5 minutes on 10 second
						if ((RTCTime.uiSecond == 10) && ((RTCTime.uiMinute % 5) == 1)) {
							DateToTextBuffer(&RTCTime, 0);
							eActualSeq = SeqText;
							RunSlowClearedPos(&matrix);
						}

						// activate relay and read date every hour, synchronize ui16Ms with RTC
						if (RTCTime.uiSecond == 0) {
							ui16Ms = 0;
							ResetProgress(&actTime);
							if (RTCTime.uiMinute == 0) {
								if (relay.eState == RelayBinaryFull)
									RelayStartClicking(&relay, RTCTime.uiHour, RelayDataHours);
								else if (relay.eState == RelaySilent)
									RelayClicking(&relay, RelayClickSlow, 1);
								DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
							}
							else
								if ((RTCTime.uiMinute % 15) == 0) {
									if (relay.eState == RelayBinaryFull)
										RelayStartClicking(&relay, RTCTime.uiMinute, RelayDataMinutes);

								}
						}
						bNewTime = false;
					}

					// loading time and devices state to matrix
					if (bNewRoundRefresh){
						LoadTimeToMatrix(&matrix, &actTime, &RTCTime);
						SetStatePoint(&matrix, 6, relay.eState % 2);
						SetStatePoint(&matrix, 5, relay.eState / 2);
						SetStatePoint(&matrix, 0, BLUETOOTH_IS_ON());
						bNewRoundRefresh = false;
					}
				} break;
				case SeqBomb: {
					if (bNewTime) {
						TryDecrementTime(&actTime);
						snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, "%02d:%02d\n", actTime.uiMinute, actTime.uiSecond);
						uart_puts(ctTextBuffer);
						ctTextBuffer[5] = 0;
						if (actTime.uiSecond % 2) ctTextBuffer[2] = ' ';
						else ctTextBuffer[2] = 0x80;
						LoadTextToMatrix(&matrix, ctTextBuffer);

						// clicking in specific period
						if (((actTime.uiSecond % 10) == 0) && (actTime.uiMinute > 0)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}
						if (((actTime.uiSecond % 2) == 0) && (actTime.uiMinute == 0) && (actTime.uiSecond > 30)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}
						if ((actTime.uiMinute == 0) && (actTime.uiSecond <= 30) && (actTime.uiSecond > 10)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}
						if ((actTime.uiMinute == 0) && (actTime.uiSecond <= 10)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}

						if ((actTime.uiSecond ==0) && (actTime.uiMinute == 0)) {
							snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, "Boom!!!");
							eActualSeq = SeqText;
							RelayClicking(&relay, RelayClickStartFast, 182);
							RunSlowClearedPos(&matrix);
							break;
						}
						bNewTime = false;
					// half of second
					} else if (bHalfTime) {
						if ((actTime.uiMinute == 0) && (actTime.uiSecond <= 10)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}
						bHalfTime = false;
					// quater of second
					} else if (bQuaterTime) {
						if ((actTime.uiMinute == 0) && (actTime.uiSecond <= 5)) {
							RelayClicking(&relay, RelayClickStartFast, 1);
						}
						bQuaterTime = false;
					}

				} break;
				case SeqDeCounter: {
					if (bNewTime) {
						if (actTime.uiSecond == 0) {
							snprintf(ctTextBuffer, TEXT_BUFFER_SIZE, "%3d:%02d:%02d to end\n", actTime.uiDay * 24 + actTime.uiHour,
									actTime.uiMinute, actTime.uiSecond);
							uart_puts(ctTextBuffer);
						}
						TryDecrementTime(&actTime);
						LoadDeCounterToMatrix(&matrix, &actTime);


						bNewTime = false;
					}
				} break;
				case SeqADC: {
					// write ADC number to matrix and UART
					if (bNewTime) {
						DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
						LoadNumberToMatrix(&matrix, adc.ui16PhotoAvg);
						if ((RTCTime.uiSecond % 10) == 0) {
							uart_puts_p(PSTR("Brightness: "));
							ctTextBuffer[4] = '\n';
							uart_puts(ctTextBuffer);
						}
						bNewTime = false;
					}
				} break;
				case SeqText: {} break;
				case SeqTextDebug: {} break;
				case SeqRelayNumber: {} break;
				case SeqEmpty: {} break;
			}
		}
	}
} // END int main (void)

//! CTC timer0 overflow, refreshing matrix, required hard optimisation,
//! always critical section, when rename Y, next overflow ignored
ISR(TIMER0_COMPA_vect) {
	uivModifyY = IncrementBrightness(&matrix);
	if (uivModifyY) {
		SendRegisterY(ReturnYValue(&matrix), true);
		if (matrix.uiBrightness) {
			TCNT0 = 0;
			RefreshBufferFlag(&matrix);
			TCNT0 = 0;
			SendRegistersX(matrix.uitBufferFlag, true);
			// brightness level at 1
			TCNT0 = 120;
		}
	} else {
		if (matrix.uiBrightCount == matrix.uiBrightness)
			ClearRegistersX(true); //old

	}
} // END ISR(TIMER0_COMPA_vect)

//! CTC timer2 overflow with 1ms period, internal time
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	// reset to 0 in PCINT1_vect
	if (++ui16Ms == 1) {
		if (++uiSeconds >= 60)
			uiSeconds = 0;
	}
	if ((ui16Ms == 250) || (ui16Ms == 750)) {
		bQuaterTime = true;
	}
	// setting flag of refresh round buffer
	if ((ui16Ms % TIME_DECREMENT_MS) == 0)
		bNewRoundRefresh = true;
	// increasing buffer position and rename seq to SeqTimer when active is SeqText and iCountToTimer == 0
	if ((ui16Ms % INC_POS_MS) == 0)
		if (IncrementBufferStart(&matrix)) {
			if ((eActualSeq == SeqText) && (matrix.uiSlowClearedPos == 0) && ((--iCountToTimer) == 0) ) {
				eActualSeq = SeqTimer;
				SetSeqParams(&matrix, &actTime, &RTCTime, &relay);
			}
		}
	if ((ui16Ms % 10) == 7)
		bEnableDecrement = true;
	if (ui16Ms >= 1000) ui16Ms = 0;
} // END ISR(TIMER2_COMPA_vect)

//! end of ADC measurement interrupt, reading ADC value
ISR(ADC_vect) {
	ReadADCToADCData(&adc, &matrix.uiBrightness, &matrix.uiPWMBrightness);
}


//! SQW signal from DS3231 interrupt, set new time flag
ISR(PCINT1_vect) {
	ADCStart();
	if (SQW_IS_HIGH()) {
		bNewTime = true;
		//ui16Ms = 0;
	} else
		bHalfTime = true;
} // END ISR(PCINT1_vect)

//! switch on bluetooth interrupt, init UART after turning on module
ISR(PCINT2_vect) {
	if (BLUETOOTH_IS_ON()) {
		USART_Init(__UBRR);
	}
} // ISR(PCINT2_vect)


