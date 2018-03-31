/*!
 * @file uart_processing.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 23-12-2017 $
 * $Created: 23-12-2017 $
 * @version 0.1
 *
 * Plik naglowkowy zawierajacy obsluge przetwarzania danych zgromadzonych w buforze UART
 */
#ifndef DEVICES_BLUETOOTH_UART_PROCESSING_H_
#define DEVICES_BLUETOOTH_UART_PROCESSING_H_

#include "../../group_includes.h"
#include "mkuart.h"
#include "../../seq/matrix_seq.h"
#include "../relay.h"

// kody specjalne bajtow
//! kod naglowka ramki zmiany sekwencji
#define MODIFY_SEQ_CODE 0xF0
//! kod naglowka ramki zmiany trybu pracy
#define DEVICE_MODE_CODE 0xF1
//! kod naglowka ramki godziny/daty
#define LOAD_DATE_CODE 0xF2
//! kod naglowka ramki analizatora widma
#define SPECTRUM_CODE 0xF3
//! kod naglowka ramki utworu
#define MUSIC_CODE 0xF4
//! kod zakonczenia ramki
#define END_FRAME_CODE 0xFF

#define MAX_COMMAND_LENGTH 32

#define GOOD_COMMAND END_FRAME_CODE
#define ERROR_COMMAND 0xE0
#define ERROR_PARAMS 0xE1

//! proba zaladowania i obsluzenia pojedynczej komendy wraz z danymi
extern void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, uint8_t brightness);
#endif /* DEVICES_BLUETOOTH_UART_PROCESSING_H_ */
