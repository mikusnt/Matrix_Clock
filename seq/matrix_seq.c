/*!
 * @file matrix_seq.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see matrix_seq.h*/

#include "matrix_seq.h"


/*
 *
 * 		Funkcje wewnetrzne
 *
 */

uint8_t uitHMPos[4] = {0, 6, 14, 20};

char ctTextBuffer[TEXT_BUFFER_SIZE] = {0};

ActualSeq eActualSeq = SeqTimer;

/*! laduje do bufora matrycy sekundy w binarnym kodzie BCD
 *  @param		m adres struktury macierzy LED
 *  @param		seconds sekundy ktore maja byc wieswietlone
 *  @param 		brightness jasnosc*/
static inline void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds, uint8_t brightness) {
	uint8_t tens = seconds / 10;
	uint8_t ones = seconds % 10;
	for(uint8_t i = 0; i < 4; i++) {
		if (tens % 2) {
			m->uitBufferYX[2][SEC_END_POS - i] = brightness;
		}else{
			m->uitBufferYX[2][SEC_END_POS - i] = 0;
		}
		tens >>= 1;
		if (ones % 2){
			m->uitBufferYX[5][SEC_END_POS - i] = brightness;
		}else{
			m->uitBufferYX[5][SEC_END_POS - i] = 0;
		}
		ones >>= 1;
	}
}

/*! laduje do bufora matrycy wybrany znak ASCII
 *  @param		m adres struktury macierzy LED
 *  @param		sign znak jaki ma byc wstawiony
 *  @param		x_pos pozycja X w buforze
 *  @param 		brightness jasnosc
 *  @return		pozycja konca wczytywanego znaku*/
static uint8_t InsertCharToMatrix(volatile DiodeMatrix *m, char sign, uint8_t x_pos, uint8_t brightness) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t j = start; j <= stop; j++) {
		SetXBuffer(m, x_pos++, LoadIntSignByte(signCode, j), brightness);
		if (x_pos >= BUFFER_X_SIZE)
			return BUFFER_X_SIZE - 1;
	}
	return x_pos;
} // END static uint8_t InsertCharToMatrix

/*! laduje do bufora obrotowego matrycy wskazany znak na odpowiedniej pozycji
 *  @param		m adres struktury macierzy LED
 *  @param		sign znak jaki ma byc wstawiony
 *  @param		y_pos pozycja Y w buforze obrotowym
 *  @param 		brightness jasnosc*/
static void InsertCharToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos, uint8_t brightness) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t i = 0; i < 5; i++)
		SetXRoundBuffer(m, i, y_pos, 0, 0);
	for (uint8_t j = start; j <= stop; j++) {
		SetXRoundBuffer(m, j, y_pos, LoadIntSignByte(signCode, j), brightness);
	}
} // END static void InsertCharToRoundBuffer

/*! laduje do bufora matrycy informacje o wartosci ADC w postaci kropek na pozycji 7
 *  @param		m adres struktury macierzy LED
 *  @param		adcValue wartosc ADC
 *  @param 		brightness jasnosc*/
void LoadADCToMatrix(volatile DiodeMatrix *m, uint16_t adcValue, uint8_t brightness) {
	adcValue /= 32;
	for (int8_t i = 31; i >= 0; i--){
		if (i >= MATRIX_X_SIZE - adcValue)
			m->uitBufferYX[MATRIX_Y_SIZE - 1][i] = brightness;
		else
			m->uitBufferYX[MATRIX_Y_SIZE - 1][i] = 0;
	}
} // END void LoadADCToMatrix

/*
 *
 *		Funkcje zewnetrzne
 *
 */

/*! laduje parametry kazdego ze znakow z alfabetu, laduje zawartosc znaku do kolejnej pozycji bufora
 *  @param		m adres struktury macierzy LED
 *  @param		text tekst ktory zostanie zaladowany do bufora macierzy
 *	@param		brightness jasnosc poszczegolnych pikseli */
void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE], uint8_t brightness) {
	uint8_t i = 0;
	uint8_t addr = 0;
	while(text[i] && (addr < BUFFER_X_SIZE)) {
		addr = InsertCharToMatrix(m, text[i], addr, brightness);
		i++;
		if (addr < BUFFER_X_SIZE - 1)
			SetXBuffer(m, addr++, 0, brightness);
	}
	m->uiEndBufferPosition = addr;
} // END void LoadTextToMatrix

/*! W przypadku roznic pomiedzy dwiema struktury czasowymi laduje ta starsza do bufora macierzy
 *  Wykorzystuje bufor obrotowy do plynnego przejscia pomiedzy dwiema cyframi
 *  @param		m adres struktury macierzy LED
 *  @param		from czas wyswietlany na matrycy, dazy do czasu to
 *  @param		to czas rzeczywisty
 *	@param		brightness jasnosc poszczegolnych pikseli */
void LoadTimeToMatrix(volatile DiodeMatrix *m, TimeDate *from, TimeDate *to, uint8_t brightness) {
	SetXBuffer(m, SEC_SIGN_POS, (to->uiSeconds % 2) == 0 ? 0x44 : 0x00, brightness);
	for (int8_t i = 3; i >= 0; i--) {
		// gdy cyfry takie same
		if ((from->uitSingleTime[i] == to->uitSingleTime[i]) && (from->uiSingleProgress[i] < MAX_PROGRESS))
			from->uiSingleProgress[i] = MAX_PROGRESS;

		// stopniowe ladowanie z bufora obrotowego
		if (from->uiSingleProgress[i] < MAX_PROGRESS) {
			if (from->uiSingleProgress[i] == 0) {
				InsertCharToRoundBuffer(m, from->uitSingleTime[i] + NUMBER_TO_ASCII, 0, brightness);
				InsertCharToRoundBuffer(m, to->uitSingleTime[i] + NUMBER_TO_ASCII, 8, brightness);
			}
			CopyFromRoundToBuffer(m, from->uiSingleProgress[i]++, uitHMPos[i]);
			break;
		// zaladowanie wlasciwej cyfry
		} else if (from->uiSingleProgress[i] == MAX_PROGRESS){
			from->uitSingleTime[i] = to->uitSingleTime[i];
			LoadToGroupTime(from);
			InsertCharToMatrix(m, from->uitSingleTime[i] + NUMBER_TO_ASCII, uitHMPos[i], brightness);
			// blokaga na ponowne wstawienie gdy wstawiono
			from->uiSingleProgress[i] = MAX_PROGRESS + 1;
		}
	}

	SecondsBinary(m, to->uiSeconds, brightness);
	//LoadADCToMatrix(m, adcValue, brightness);

	m->uiEndBufferPosition = SEC_END_POS + 1;
} // END void LoadTimeToMatrix

void LoadDateToMatrix(volatile DiodeMatrix *m, TimeDate *time, uint8_t brightness) {
	sprintf(ctTextBuffer, "%02d-%02d-%02d", time->uiDays, time->uiMonths, time->uiYears);
	LoadTextToMatrix(m, ctTextBuffer, brightness);
}


/*! @param		m adres struktury macierzy LED
 *  @param		number wstawiana liczba
 *	@param		brightness jasnosc*/
void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number, uint8_t brightness) {
	sprintf(ctTextBuffer, "%04d%c", number, 0);
	LoadTextToMatrix(m, ctTextBuffer, brightness);
} // END void LoadNumberToMatrix

/*! @param		seq ustawiona wczesniej sekwencja
 *  @param		m adres struktury macierzy LED
 *  @param		actTime adres struktury aktualnego czasu
 *  @param		relay adres struktury przekaznika
 *  @param		brightness jasnosc*/
void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, volatile Relay *relay, uint8_t brightness) {
	ClearBuffer(m);
	switch(eActualSeq) {
		case SeqTimer: {
			LoadTextToMatrix(m, "00 00", brightness);
			TimeInit(actTime);
			SetMoving(m, false);
		} break;
		case SeqADC: {
			SetMoving(m, false);
		} break;
		case SeqText: {
			LoadTextToMatrix(m, ctTextBuffer, brightness);
			SetMoving(m, true);
		} break;
		case SeqRelayNumber: {
			RelayStartClicking(relay, ctTextBuffer[1] - 128, RelayDataNumber);
			LoadNumberToMatrix(m, ctTextBuffer[1] - 128, brightness);
			SetMoving(m, false);
		} break;
		case SeqManualPix: {
			SetMoving(m, false);
		}
	}
} // END void SetSeqParams

