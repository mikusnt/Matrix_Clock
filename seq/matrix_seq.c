/*!
 * @file matrix_seq.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see matrix_seq.h*/

#include "matrix_seq.h"


uint8_t uitHMPos[4] = {0, 6, 14, 20};

char ctTextBuffer[TEXT_BUFFER_SIZE] = {0};

ActualSeq eActualSeq = SeqTimer;

/*
 *
 * 		Funkcje wewnetrzne
 *
 */

/*! laduje do bufora matrycy sekundy w binarnym kodzie BCD
 *  @param		m adres struktury macierzy LED
 *  @param		seconds sekundy ktore maja byc wieswietlone*/
static inline void SecondsBinary(volatile DiodeMatrix *m, volatile uint8_t seconds) {
	uint8_t tens = seconds / 10;
	uint8_t ones = seconds % 10;
	for(uint8_t i = 0; i < 4; i++) {
		m->uitBufferX[SEC_END_POS - i] = 0;
		if (tens % 2)
			m->uitBufferX[SEC_END_POS - i] |= 1 << 2;
		tens >>= 1;
		if (ones % 2)
			m->uitBufferX[SEC_END_POS - i] |= 1 << 5;
		ones >>= 1;
	}
} // END static inline void SecondsBinary

static inline void SetRelayModeToMatrix(volatile DiodeMatrix *m, volatile Relay *r) {
	if (r->eState == ON) {
		m->uitBufferX[2] &= 0x7F;
	} else {
		m->uitBufferX[2] |= 0x80;
	}
} // END static inline void RelayMode

/*! laduje do bufora matrycy wybrany znak ASCII
 *  @param		m adres struktury macierzy LED
 *  @param		sign znak jaki ma byc wstawiony
 *  @param		x_pos pozycja X w buforze
 *  @return		pozycja konca wczytywanego znaku*/
static uint8_t InsertCharToMatrix(volatile DiodeMatrix *m, char sign, uint16_t x_pos) {
	uint8_t start, stop, signCode;
	LoadSign(sign, &start, &stop, &signCode);
	for (uint8_t j = start; j <= stop; j++) {
		m->uitBufferX[x_pos++] = LoadIntSignByte(signCode, j);
		if (x_pos >= BUFFER_X_SIZE)
			return BUFFER_X_SIZE - 1;
	}
	return x_pos;
} // END static uint8_t InsertCharToMatrix

/*! laduje do bufora obrotowego matrycy wskazany znak na odpowiedniej pozycji
 *  @param		m adres struktury macierzy LED
 *  @param		sign znak jaki ma byc wstawiony
 *  @param		y_pos pozycja Y w buforze obrotowym*/
static void InsertCharToRoundBuffer(volatile DiodeMatrix *m, char sign, uint8_t y_pos) {
	if (y_pos < 2) {
		uint8_t start, stop, signCode;
		LoadSign(sign, &start, &stop, &signCode);
		for (uint8_t i = 0; i < 5; i++)
			m->uitRoundBufferYX[y_pos][i] = 0;
		for (uint8_t j = start; j <= stop; j++) {
			m->uitRoundBufferYX[y_pos][j] = LoadIntSignByte(signCode, j);
		}
	}
} // END static void InsertCharToRoundBuffer

/*! laduje do bufora matrycy informacje o wartosci ADC w postaci kropek na pozycji 7
 *  @param		m adres struktury macierzy LED
 *  @param		adcValue wartosc ADC*/
void LoadADCToMatrix(volatile DiodeMatrix *m, uint16_t adcValue) {
	adcValue /= 32;
	for (int8_t i = 31; i >= 0; i--){
		m->uitBufferX[i] &= ~(1 << 7);
		if (i >= MATRIX_X_SIZE - adcValue)
			m->uitBufferX[i] |= 1 << 7;
	}
} // END void LoadADCToMatrix

/*
 *
 *		Funkcje wlasciwe
 *
 */

/*! laduje parametry kazdego ze znakow z alfabetu, laduje zawartosc znaku do kolejnej pozycji bufora
 *  @param		m adres struktury macierzy LED
 *  @param		text tekst ktory zostanie zaladowany do bufora macierzy*/
void LoadTextToMatrix(volatile DiodeMatrix *m, char text[TEXT_BUFFER_SIZE]) {
	uint16_t i = 0;
	uint16_t addr = 0;
	while(text[i] && (addr < BUFFER_X_SIZE)) {
		addr = InsertCharToMatrix(m, text[i], addr);
		i++;
		if (addr < BUFFER_X_SIZE - 1)
			m->uitBufferX[addr++] = 0;
	}
	m->uiEndBufferPosition = addr;
} // END void LoadTextToMatrix

/*! W przypadku roznic pomiedzy dwiema struktury czasowymi laduje ta starsza do bufora macierzy
 *  Wykorzystuje bufor obrotowy do plynnego przejscia pomiedzy dwiema cyframi
 *  @param		m adres struktury macierzy LED
 *  @param		from czas wyswietlany na matrycy, dazy do czasu to
 *  @param		to czas rzeczywisty*/
void LoadTimeToMatrix(volatile DiodeMatrix *m, volatile Relay *r, TimeDate *from, TimeDate *to) {
	m->uitBufferX[SEC_SIGN_POS] = (to->uiSeconds % 2) == 0 ? 0x24 : 0x00;
	for (int8_t i = 3; i >= 0; i--) {
		// gdy cyfry takie same
		if ((from->uitSingleTime[i] == to->uitSingleTime[i]) && (from->uiSingleProgress[i] < MAX_PROGRESS))
			from->uiSingleProgress[i] = MAX_PROGRESS;

		// stopniowe ladowanie z bufora obrotowego
		if (from->uiSingleProgress[i] < MAX_PROGRESS) {
			if (from->uiSingleProgress[i] == 0) {
				InsertCharToRoundBuffer(m, from->uitSingleTime[i] + DIGIT_ASCII, 0);
				InsertCharToRoundBuffer(m, to->uitSingleTime[i] + DIGIT_ASCII, 1);
			}
			CopyFromRoundToBuffer(m, from->uiSingleProgress[i]++, uitHMPos[i]);
			break;
		// zaladowanie wlasciwej cyfry
		} else if (from->uiSingleProgress[i] == MAX_PROGRESS){
			from->uitSingleTime[i] = to->uitSingleTime[i];
			LoadToGroupTime(from);
			InsertCharToMatrix(m, from->uitSingleTime[i] + DIGIT_ASCII, uitHMPos[i]);
			// blokaga na ponowne wstawienie gdy wstawiono
			from->uiSingleProgress[i] = MAX_PROGRESS + 1;
		}
	}

	SecondsBinary(m, to->uiSeconds);
	SetRelayModeToMatrix(m, r);
	//LoadADCToMatrix(m, adcValue, brightness);

	m->uiEndBufferPosition = SEC_END_POS + 1;
} // END void LoadTimeToMatrix

/*! @param		m adres struktury macierzy LED
 *  @param		number wstawiana liczba*/
void LoadNumberToMatrix(volatile DiodeMatrix *m, uint16_t number) {
	sprintf(ctTextBuffer, "%04d%c", number, 0);
	LoadTextToMatrix(m, ctTextBuffer);
} // END void LoadNumberToMatrix

/*! @param		m adres struktury macierzy LED
 *  @param		actTime adres struktury aktualnego czasu
 *  @param		relay adres struktury przekaznika*/
void SetSeqParams(volatile DiodeMatrix *m, TimeDate *actTime, volatile Relay *relay) {
	ClearBuffer(m);
	uart_puts_p(PSTR("Seq: "));

	switch(eActualSeq) {
		case SeqTimer: {
			LoadTextToMatrix(m, "00 00");
			TimeInit(actTime);
			SetMoving(m, false);
			uart_puts_p(PSTR("Timer"));
		} break;
		case SeqADC: {
			SetMoving(m, false);
			uart_puts_p(PSTR("ADC brightness"));
		} break;
		case SeqText: {
			LoadTextToMatrix(m, ctTextBuffer);
			SetMoving(m, true);
			uart_puts_p(PSTR("Text \""));
			uart_puts(ctTextBuffer);
			uart_putc('\"');
		} break;
		case SeqTextDebug: {
			LoadTextToMatrix(m, ctTextBuffer);
			uart_puts_p(PSTR("Text debug \""));
			uart_puts(ctTextBuffer);
			uart_putc('\"');
		} break;
		case SeqRelayNumber: {
			uart_puts_p(PSTR("Relay with number "));
			uart_putint(ctTextBuffer[0], 10);
			RelayStartClicking(relay, ctTextBuffer[0], RelayDataNumber);
			LoadNumberToMatrix(m, ctTextBuffer[0]);
			SetMoving(m, false);

		} break;
		case SeqEmpty: {
			SetMoving(m, false);
			uart_puts_p(PSTR("Clear matrix"));
		}
	}
	uart_putc('\n');
} // END void SetSeqParams

