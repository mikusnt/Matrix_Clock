/*!
 * @file relay.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see relay.h*/
#include "relay.h"

uint8_t eEEMRelayState EEMEM;

/*
 *
 *		Internal functions
 *
 */

static void RelayReset(volatile Relay *r) {
	r->ui16ActTimeMS = 0;
	r->uiByteInfo = 0;
	r->uiByteLength = 0;
	r->uiStartTimeMS = 0;
	r->uiStartLength = 0;
}
/*! change hardware relay state
 * @param 		eB logic state of relay*/
static inline void RelaySW(BinarySwitch eB) {
	if (eB) RELAY_PORT |= RELAY_ADDR;
	else RELAY_PORT &= ~RELAY_ADDR;
}


/*! zamienia kolejnosc bitow w bajcie na przeciwna
 * rounding bit order to opposite
 * @param 		byte to round
 * @param 		uiByteLength_s length of rounded byte
 * @return 		byte after rounded*/
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
 *		External functions
 *
 */
/*! configure relay addess to out*/
void RelayInit(volatile Relay *r) {
	RELAY_DDR |= RELAY_ADDR;
	// read state from eeprom
	r->eState = (eeprom_read_byte(&eEEMRelayState) > 0) ? ON : OFF;
}

/*!@param 		relay pointer of relay structure
 * @param 		uiByteInfo data byte to click
 * @param 		dataType type of byte*/
void RelayStartClicking(volatile Relay *relay, uint8_t uiByteInfo, RelayDataType dataType) {
	if (relay->eState == ON) {
		relay->dataType = dataType;
		// load data to structure
		switch(dataType) {
			case RelayDataHours: {
				while (uiByteInfo > 12) uiByteInfo -= 12;
				if (uiByteInfo == 12) uiByteInfo = 0;
				// when hour < 2 not do rounding
				if (uiByteInfo < 2) {
					relay->uiByteLength = (uiByteInfo == 0) ? 6 : 1;
					relay->uiByteInfo = uiByteInfo;
				} else {
					relay->uiByteInfo = RoundByte(uiByteInfo, &relay->uiByteLength);
				}
				relay->uiStartLength = RELAY_HIGH_START_H_COUNT;
				relay->uiStartTimeMS = RELAY_HIGH_START_MS;
			} break;
			case RelayDataMinutes: {
				while (uiByteInfo >= 60) uiByteInfo -= 60;
				if (uiByteInfo > 0)
					relay->uiByteInfo = RoundByte(uiByteInfo / 15, &relay->uiByteLength);
				relay->uiStartLength = RELAY_HIGH_START_M_COUNT;
				relay->uiStartTimeMS = RELAY_HIGH_START_MS;
			} break;
			case RelayDataNumber: {
				relay->uiByteInfo = RoundByte(uiByteInfo, &relay->uiByteLength);
				relay->uiStartLength = RELAY_HIGH_START_N_COUNT;
				relay->uiStartTimeMS = RELAY_HIGH_START_MS_NUMBER;
			}
		}

		relay->ui16ActTimeMS = 1;
		RelayTryClickMS(relay);
	} else {
		RelaySW(OFF);
	}
} // END void RelayStartClicking

/*! realize clicking relay from uiByteInfo when time was decrement to 0 while uiByteLength is not empty
 * @param 		relay pointer of relay structure
 * @see 		RelayStartClicking()*/
void RelayTryClickMS(volatile Relay *relay) {
	// works where length of information not equals 0
	if (relay->uiByteLength){
		if (!(--relay->ui16ActTimeMS)) {
			// when start sequence
			if (relay->uiStartLength) {
					RELAY_CH();
					relay->ui16ActTimeMS = relay->uiStartTimeMS;
				// rename to pause
				if (--relay->uiStartLength == 0) {
					RelaySW(LOW);
					relay->ui16ActTimeMS = RELAY_LOW_START_MS;
				}
			// when dat asequence
			} else {
				// when high state init low (next bit)
				if (RELAY_IS_ON()) {
					if(relay->uiByteInfo % 2) {
						relay->ui16ActTimeMS = RELAY_LONG_LOW_TIME;
					} else {
						relay->ui16ActTimeMS = RELAY_SHORT_LOW_TIME;
					}
					relay->uiByteInfo >>= 1;
					relay->uiByteLength--;
				} else {
					// when low state init high
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

/*! change relay state and save state to eeprom
 *  @param			relay pointer of relay structure
 *  @param 			eState new relay state */
void SetRelayState(volatile Relay *relay, BinarySwitch eState) {
	if (relay->eState != eState) {
		relay->eState = eState;
		// save to eeprom
		eeprom_write_byte(&eEEMRelayState, eState);
		if (eState == OFF) {
			RelayReset(relay);
		}
	}
}
