/*!
 * @file uart_processing.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-05-11 $
 * $Created: 2017-11-04 $
 * @version 0.1
 *
 * Header file containing processing data from UART buffer.
 */
#ifndef DEVICES_BLUETOOTH_UART_PROCESSING_H_
#define DEVICES_BLUETOOTH_UART_PROCESSING_H_

#include "../../group_includes.h"
#include "mkuart.h"
#include "../../seq/matrix_seq.h"
#include "../ds3231.h"

// special byte codes
//! frame header code of rename sequence
#define MODIFY_SEQ_CODE 's'
//! frame header code of rename task
#define DEVICE_TASK_CODE 't'
//! frame header code of hour, date
#define LOAD_DATE_CODE 'd'
//! frame header code of spectrum analyzer
#define SPECTRUM_CODE 0xF3
//! frame header code of music track
#define MUSIC_CODE 0xF4
//! frame header code of sending good data
#define QUESTION_CODE 'q'
//! frame header code of version of app
#define VERSION_CODE 'v'
//! output text of version of app
#define VERSION_TEXT PSTR("Matrix Clock PixBit v0.95 by MiSt ")
//! frame header code of reset device
#define RESET_CODE 'r'
//! code of good recognize the frame
#define GOOD_COMMAND 0
//! code of bad frame header code
#define ERROR_COMMAND 1
//! code of bad frame parameters
#define ERROR_PARAMS 2



//! trying load and perform command which parameters
extern void TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, TimeDate *time);
#endif /* DEVICES_BLUETOOTH_UART_PROCESSING_H_ */
