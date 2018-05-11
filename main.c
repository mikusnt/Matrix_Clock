/*!
 * @file main.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-05-11 $
 * $Created: 2017-11-04 $
 * @version 0.94
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
volatile bool bNewTime = false;
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

/*
 *
 *		Overflows
 *
 */

// ISR(TIMER0_COMPA_vect)
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

	PowerReduction();
	RegistersInit();
	DiodeMatrixInit(&matrix);
	RelayInit(&relay);
	Timer0Init();
	Timer2Init();
	PCINTInit();

	ADCInit(&adc);
	USART_Init(__UBRR);
	I2C_Init();

	DS3231_Init();
	SetSeqParams(&matrix, &actTime, &relay);
	RelayStartClicking(&relay, 0, RelayDataNumber);

	wdt_enable(WDTO_120MS);
	DS3231_GetTime(&RTCTime.uiHour, &RTCTime.uiMinute, &RTCTime.uiSecond);
	DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
	LoadToSingleTime(&RTCTime);

	sei();

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
				SetSeqParams(&matrix, &actTime, &relay);
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
							sprintf(ctTextBuffer, "%02d-%02d-2%03d %02d:%02d:%02d\n", RTCTime.uiDay,
								RTCTime.uiMonth, RTCTime.uiYear, RTCTime.uiHour,
								RTCTime.uiMinute, RTCTime.uiSecond);
							uart_puts(ctTextBuffer);
						}
						// display dane every 5 minutes on 10 second
						if ((RTCTime.uiSecond == 10) && ((RTCTime.uiMinute % 5) == 1)) {
							sprintf(ctTextBuffer, "%02d-%02d-2%03d", RTCTime.uiDay, RTCTime.uiMonth, RTCTime.uiYear);
							eActualSeq = SeqText;
							RunSlowClearedPos(&matrix);
						}

						// activate relay and read date every hour
						if (RTCTime.uiSecond == 0) {
							ResetProgress(&actTime);
							if (RTCTime.uiMinute == 0) {
								RelayStartClicking(&relay, RTCTime.uiHour, RelayDataHours);
								DS3231_GetDate(&RTCTime.uiDay, &RTCTime.uiMonth, &RTCTime.uiYear);
							}
							else
								if ((RTCTime.uiMinute % 15) == 0)
									RelayStartClicking(&relay, RTCTime.uiMinute, RelayDataMinutes);
						}
						bNewTime = false;
					}

					// loading time and devices state to matrix
					if (bNewRoundRefresh){
						LoadTimeToMatrix(&matrix, &actTime, &RTCTime);
						SetStatePoint(&matrix, 6, relay.eState);
						SetStatePoint(&matrix, 0, BLUETOOTH_IS_ON());
						bNewRoundRefresh = false;
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

//! CTC timer0 overflow, refreshing matrix, required hard optimisation
ISR(TIMER0_COMPA_vect) {
	// obsluga zmiany jasnosci
	uivModifyY = IncrementBrightness(&matrix);
	if (uivModifyY) {
		SendRegisterY(ReturnYValue(&matrix), true);
		if (matrix.uiBrightness) {
			RefreshBufferFlag(&matrix);
			SendRegistersX(matrix.uitBufferFlag, true);
		}
	} else if (matrix.uiBrightCount == matrix.uiBrightness)
		ClearRegistersX(true);
} // END ISR(TIMER0_COMPA_vect)

//! CTC timer2 overflow with 1ms period, internal time
ISR(TIMER2_COMPA_vect) {
	RelayTryClickMS(&relay);
	if (++ui16Ms >= 1000) {
		ui16Ms = 0;
		if (++uiSeconds >= 60)
			uiSeconds = 0;
	}
	// setting flag of refresh round buffer
	if ((ui16Ms % TIME_DECREMENT_MS) == 0)
		bNewRoundRefresh = true;
	// increasing buffer position and rename seq to SeqTimer when active is SeqText and iCountToTimer == 0
	if ((ui16Ms % INC_POS_MS) == 0)
		if (IncrementBufferStart(&matrix)) {
			if ((eActualSeq == SeqText) && (matrix.uiSlowClearedPos == 0) && ((--iCountToTimer) == 0) ) {
				eActualSeq = SeqTimer;
				SetSeqParams(&matrix, &actTime, &relay);
			}
		}
	if ((ui16Ms % 10) == 7)
		bEnableDecrement = true;
} // END ISR(TIMER2_COMPA_vect)

//! end of ADC measurement interrupt, reading ADC value
ISR(ADC_vect) {
	ReadADCToADCData(&adc, &matrix.uiBrightness);
}


//! SQW signal from DS3231 interrupt, set new time flag
ISR(PCINT1_vect) {
	ADCStart();
	if (SQW_IS_HIGH()) {
		bNewTime = true;
	}
} // END ISR(PCINT1_vect)

//! switch on bluetooth interrupt, init UART after turning on module
ISR(PCINT2_vect) {
	if (BLUETOOTH_IS_ON()) {
		USART_Init(__UBRR);
	}
} // ISR(PCINT2_vect)


