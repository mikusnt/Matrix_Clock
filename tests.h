/*!
 * @file tests.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-015 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Simple tests of some basic functions.
 * Test should be run in main function (infinite loop).
 */
#ifndef TESTS_H_
#define TESTS_H_

#include "group_includes.h"
#include "devices/other_devices.h"
#include "devices/relay.h"
#include "devices/ds3231.h"
#include "data_types/date_time.h"
#include "data_types/diode_matrix.h"
#include "devices/register.h"

//! tests the relay's all hours operations
extern void Test_Relay_Hours(volatile Relay *r);
//! tests the ralay's all minutes operations
extern void Test_Relay_Minutes(volatile Relay *r);
//! tests Y shift register operations
//! @see SendRegisterY
extern void Test_Y(uint8_t *i);
//! tests LED matrix, try to show 4 brightness levels
extern void Test_MatrixBuffer(volatile DiodeMatrix *m);

#endif /* TESTS_H_ */
