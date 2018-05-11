/*!
 * @file uart_processing.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-09 $
 * $Created: 2017-11-04 $
 * @version 0.1
 *
 * Plik naglowkowy zawierajacy obsluge przetwarzania danych zgromadzonych w buforze UART
 */
#ifndef DEVICES_BLUETOOTH_UART_PROCESSING_H_
#define DEVICES_BLUETOOTH_UART_PROCESSING_H_

#include "../../group_includes.h"
#include "mkuart.h"
#include "../../seq/matrix_seq.h"
#include "../ds3231.h"

// kody specjalne bajtow
//! kod naglowka ramki zmiany sekwencji
#define MODIFY_SEQ_CODE 's'
//! kod naglowka ramki zmiany trybu pracy
#define DEVICE_TASK_CODE 't'
//! kod naglowka ramki godziny/daty
#define LOAD_DATE_CODE 'd'
//! kod naglowka ramki analizatora widma
#define SPECTRUM_CODE 0xF3
//! kod naglowka ramki utworu
#define MUSIC_CODE 0xF4
//! kod naglowka ramki poprawnej odpowiedzi
#define QUESTION_CODE 'q'
//! kod naglowka ramki wersji programu
#define VERSION_CODE 'v'
//! kod naglowka ramki resetu urzadzenia
#define RESET_CODE 'r'
//! kod poprawnego rozpoznania ramki
#define GOOD_COMMAND 0
//! kod bledu polecenia
#define ERROR_COMMAND 1
//! kod bledu paramentrow polecenia
#define ERROR_PARAMS 2

//! proba zaladowania i obsluzenia pojedynczej komendy wraz z danymi
extern void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, TimeDate *time);
#endif /* DEVICES_BLUETOOTH_UART_PROCESSING_H_ */
