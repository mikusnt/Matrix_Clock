/*!
 * @file relay.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see relay.h*/
#include "relay.h"


/*
 *
 *		Funkcje statyczne
 *
 */

/*! steruje wyjsciem cyfrowym przekaznika
 * @param 		eB wartosc logiczna dzialania przekaznika*/
static inline void RelaySW(BinarySwitch eB) {
	if (eB) RELAY_PORT |= RELAY_ADDR;
	else RELAY_PORT &= ~RELAY_ADDR;
}


/*! zamienia kolejnosc bitow w bajcie na przeciwna
 * @param 		byte bajt, ktorego bity zostaja zamienione
 * @param 		uiByteLength_s adres wygenerowanej dlugosci pierwszego argumentu
 * @return 		bajt, ktory ma zamieniona kolejnosc bitow*/
static inline uint8_t RoundByte(uint8_t byte, volatile uint8_t *uiByteLength_s) {
	uint8_t temp = 0;
	if (byte) {
		*uiByteLength_s = 8;
		temp |= ((byte & 0x80) >> 7) | ((byte & 0x01) << 7);
		temp |= ((byte & 0x40) >> 5) | ((byte & 0x02) << 5);
		temp |= ((byte & 0x20) >> 3) | ((byte & 0x04) << 3);
		temp |= ((byte & 0x10) >> 1) | ((byte & 0x08) << 1);

		while (!(temp % 2)) {
			temp >>= 1;
			(*uiByteLength_s)--;
		}
		return temp;
	} else {
		*uiByteLength_s = 1;
		return 0;
	}
} // END static inline uint8_t RoundByte

/*
 *
 *		Funkcje wlasciwe
 *
 */
/*! konfiguruje adres przekaznika na wyjscie*/
void RelayInit() {
	RELAY_DDR |= RELAY_ADDR;
}

/*!@param 		relay adres struktury przekaznika
 * @param 		uiByteInfo bajt, ktory ma byc przekazany za pomoca impulsow przekaznika
 * @param 		dataType typ danych wejsciowych*/
void RelayStartClicking(volatile Relay *relay, uint8_t uiByteInfo, RelayDataType dataType) {
	relay->dataType = dataType;
	// zaladowanie danychdo struktury
	switch(dataType) {
		case RelayDataHours: {
			while (uiByteInfo > 12) uiByteInfo -= 12;
			if (uiByteInfo == 12) uiByteInfo = 0;
			// gdy godzina mniejsza od 2 nie odwracaj
			if (uiByteInfo < 2) {
				relay->uiByteLength = (uiByteInfo == 0) ? 6 : 1;
				relay->uiByteInfo = uiByteInfo;
			} else {
				relay->uiByteInfo = RoundByte(uiByteInfo, &relay->uiByteLength);
			}
			relay->uiStartLength = RELAY_HIGH_START_H_COUNT;
			relay->uiHighStartTimeMS = RELAY_HIGH_START_MS;
		} break;
		case RelayDataMinutes: {
			while (uiByteInfo >= 60) uiByteInfo -= 60;
			if (uiByteInfo > 0)
				relay->uiByteInfo = RoundByte(uiByteInfo / 15, &relay->uiByteLength);
			relay->uiStartLength = RELAY_HIGH_START_M_COUNT;
			relay->uiHighStartTimeMS = RELAY_HIGH_START_MS;
		} break;
		case RelayDataNumber: {
			relay->uiByteInfo = RoundByte(uiByteInfo, &relay->uiByteLength);
			relay->uiStartLength = RELAY_HIGH_START_N_COUNT;
			relay->uiHighStartTimeMS = RELAY_HIGH_START_MS_NUMBER;
		}
	}

	// zerowanie zmiennych przed pierwszym kliknieciem
	relay->ui16ActTimeMS = 1;
	RelayTryClickMS(relay);
} // END void RelayStartClicking

/*! obsluguje przekazywanie informacji w uiByteInfo, sprawdzajac czy uplynal wymagany czas
 * stanu przekaznika, przechodzac do obslugi nastepnego tak dlugo, az nie skoncza sie dane
 * @param 		relay adres struktury przekaznika
 * @see 		RelayStartClicking()*/
void RelayTryClickMS(volatile Relay *relay) {
	// dziala tylko gdy niezerowa dlugosc informacji
	if (relay->uiByteLength) {
		if (!(--relay->ui16ActTimeMS)) {
			// gdy aktywna sekwencja startowa
			if (relay->uiStartLength) {
				// zmiana stanu przekaznika gdy tryb drgania (relay->uiStartLength > 0)
					RELAY_CH();
					relay->ui16ActTimeMS = relay->uiHighStartTimeMS;
				// zmiana z drybu drgania na przerwe
				if (--relay->uiStartLength == 0) {
					RelaySW(LOW);
					relay->ui16ActTimeMS = RELAY_LOW_START_MS;
				}
			// gdy aktywna wlasciwa sekwencja danych
			} else {
				// gdy aktualnie wysoki to zainicjuj niski (nastepny bit)
				if (RELAY_IS_ON()) {
					if(relay->uiByteInfo % 2) {
						relay->ui16ActTimeMS = RELAY_LONG_LOW_TIME;
					} else {
						relay->ui16ActTimeMS = RELAY_SHORT_LOW_TIME;
					}
					relay->uiByteInfo >>= 1;
					relay->uiByteLength--;
				// gdy aktualny niski to zainicjuj wysoki
				} else {
					// gdy aktualnie niski to zainicjuj wysoki
					if(relay->uiByteInfo % 2) {
						relay->ui16ActTimeMS = RELAY_LONG_HIGH_TIME;
					} else {
						relay->ui16ActTimeMS = RELAY_SHORT_HIGH_TIME;
					}
				}

				RELAY_CH();
			}

		}
	} else RelaySW(OFF);
} // END void RelayTryClickMS
