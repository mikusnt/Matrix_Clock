/*!
 * @file uart_read.h
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

// kody specjalne bajtow
//! kod naglowka ramki zmiany sekwencji
#define MODIFY_SEQ_CODE 0xF0
//! kod naglowka ramki zmiany trybu pracy
#define DEVICE_MODE_CODE 0xF1
//! kod naglowka ramki godziny/daty
#define LOAD_DATE_CODE 0xF2
//! kod naglowka ramki tekstu
#define LOAD_TEXT_CODE 0xF3
//! kod naglowka ramki analizatora widma
#define SPECTRUM_CODE 0xF4
//! kod naglowka ramki utworu
#define MUSIC_CODE 0xF5
//! kod zakonczenia ramki
#define END_FRAME_CODE 0xFF

#define MAX_COMMAND_LENGTH 32

//! proba zaladowania i obsluzenia pojedynczej komendy wraz z danymi
extern void TryLoadCommand();
#endif /* DEVICES_BLUETOOTH_UART_PROCESSING_H_ */
