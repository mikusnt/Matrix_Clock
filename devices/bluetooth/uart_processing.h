/*!
 * @file uart_processing.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2019-01-07 $
 * $Created: 2017-11-04 $
 * @version 1.1
 *
 * Header file containing processing data from UART buffer.
 */
#ifndef DEVICES_BLUETOOTH_UART_PROCESSING_H_
#define DEVICES_BLUETOOTH_UART_PROCESSING_H_

#include "../../group_includes.h"
#include "mkuart.h"
#include "../../seq/matrix_seq.h"
#include "../ds3231.h"
#include "../text_eeprom.h"

/*
 *
 *		Special byte codes
 *
 */

//! frame header code of rename sequence
#define MODIFY_SEQ_CODE 's'
//! frame header code of rename task
#define DEVICE_TASK_CODE 't'

//! frame header code of spectrum analyzer
#define SPECTRUM_CODE 0xF3
//! frame header code of music track
#define MUSIC_CODE 0xF4

// Extras
//! frame header code of hour, date
#define LOAD_DATE_CODE 'd'
//! frame header code of sending good data
#define QUESTION_CODE 'q'
//! frame header code of version of app
#define VERSION_CODE 'v'
//! output text of version of app, to manual update before compile
#define VERSION_TEXT PSTR("Matrix Clock PixBit v1.12 by MiSt, compiled 2019-01-07")
//! frame header code of reset device
#define RESET_CODE 'r'
//! frame header code of last time and date modification
#define MODIFY_DATE_CODE 'm'

// internal commands
//! code of good frame and parameters recognize
#define GOOD_COMMAND 0
//! code of bad frame header code
#define ERROR_COMMAND 1
//! code of bad frame parameters
#define ERROR_PARAMS 2



//! trying load and perform command which parameters
extern bool TryLoadCommand(volatile DiodeMatrix *m, volatile Relay *relay, TimeDate *time);
#endif /* DEVICES_BLUETOOTH_UART_PROCESSING_H_ */
