/*!
 * @file tests.h
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * $Modified: 2018-04-02 $
 * $Created: 2017-11-04 $
 * @version 1.0
 *
 * Testy poszczegolnych funkcji urzadzenia.
 * Ka¿dy test powinien byc uruchomiony w petli glownej po inicjalizacji skladowych testowych.
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

//! testuje dzialanie przekaznika dla wszystkich godzin oraz wypisuje godzine na rejestrze X0
extern void Test_Relay_Hours(volatile Relay *r);
//! testuje dzialanie przekaznika dla wszystkich minut oraz wypisuje minuty na rejestrze X0
extern void Test_Relay_Minutes(volatile Relay *r);
//! testuje dzialanie rejestru Y
extern void Test_Y(uint8_t *i);
//! testuje dzialanie matrycy LED poprzez wyswietlanie 4 poziomow jasnosci
extern void Test_MatrixBuffer(volatile DiodeMatrix *m);

#endif /* TESTS_H_ */
