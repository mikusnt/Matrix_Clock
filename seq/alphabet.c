/*!
 * @file alphabet.c
 * @author 		Mikolaj Stankowiak <br>
 * 				mik-stan@go2.pl
 * @see alphabet.h
 */

#include "alphabet.h"

/*
 *
 * 		Tables in PROGMEM
 *
 */

/*! first coorinate in number of cha [0..ALPHABET_SIZE],
 *  second is char vertical length [0..4],
 *  all chars format is 5x8*/


const uint8_t uiAnimations[ANIMATIONS_SIZE][7] PROGMEM = {
//	{ // MarioS0, y = 0
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 1, 1, 0},
//				{1, 0, 1, 0, 1},
//				{0, 0, 1, 0, 0},
//				{0, 1, 0, 1, 0},
//				{0, 1, 0, 1, 0},
//				{1, 1, 1, 1, 1}
//		},
	{ 0x88, 0xE5, 0x9F, 0xE5, 0x88, 0x00, 0x00 },
//	{ // MarioS1, y = 1
//				{0, 1, 1, 1, 0},
//				{1, 0, 1, 0, 1},
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 0, 1, 0},
//				{0, 1, 0, 1, 0},
//				{1, 1, 1, 1, 1}
//		},
	{ 0x82, 0xE5, 0x9F, 0xE5, 0x82, 0x00, 0x00 },
//	{ // MarioS2, y = 2
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 0, 1, 0},
//				{0, 1, 0, 1, 0},
//				{1, 1, 1, 1, 1}
//		},
	{ 0x80, 0xE9, 0x9F, 0xE9, 0x80, 0x00, 0x00 },

//	{ // MarioD0, y = 3
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 1, 1, 0},
//				{1, 0, 1, 0, 1},
//				{0, 0, 1, 0, 0},
//				{0, 1, 0, 1, 0},
//				{1, 0, 0, 0, 1},
//				{1, 1, 1, 1, 1}
//		},
	{ 0xC8, 0xA5, 0x9F, 0xA5, 0xC8, 0x00, 0x00 },
//	{ // MarioD1, y = 4
//				{0, 1, 1, 1, 0},
//				{1, 0, 1, 0, 1},
//				{0, 1, 1, 1, 0},
//				{0, 0, 1, 0, 0},
//				{0, 0, 1, 0, 0},
//				{0, 1, 0, 1, 0},
//				{1, 0, 0, 0, 1},
//				{1, 1, 1, 1, 1}
//		},
	{ 0xC2, 0xA5, 0x9F, 0xA5, 0xC2, 0x00, 0x00 },
//	{ // Pacman0, y = 5
//				{0, 0, 1, 1, 1, 0, 0},
//				{0, 1, 1, 1, 1, 1, 0},
//				{1, 1, 1, 1, 1, 1, 1},
//				{0, 0, 1, 1, 1, 1, 1},
//				{1, 1, 1, 1, 1, 1, 1},
//				{0, 1, 1, 1, 1, 1, 0},
//				{0, 0, 1, 1, 1, 0, 0},
//				{1, 1, 1, 1, 1, 1, 1}
//		},
	{ 0x94, 0xB6, 0xFF, 0xFF, 0xFF, 0xBE, 0x9C },
//	{ // Pacman1, y = 6
//				{0, 0, 1, 1, 1, 0, 0},
//				{0, 1, 1, 1, 1, 1, 0},
//				{0, 0, 0, 1, 1, 1, 1},
//				{0, 0, 0, 0, 1, 1, 1},
//				{0, 0, 0, 1, 1, 1, 1},
//				{0, 1, 1, 1, 1, 1, 0},
//				{0, 0, 1, 1, 1, 0, 0},
//				{1, 1, 1, 1, 1, 1, 1}
//		},
	{ 0x80, 0xA2, 0xE3, 0xF7, 0xFF, 0xBE, 0x9C },
};

/*
 *
 *		Funkcje
 *
 */

//! translate ASCII char code to internal uiAlphabet code
/*! @param 		cSign ASCII code of char
 *  @return		position in uiAlphabet and uiAlLength table*/
static inline uint8_t TranslateToAlphabet(char cSign) {
	// small chars
	if ((cSign >= 97) && (cSign <= 122)) return cSign - 61;
	// big chars
	else if ((cSign >= 65) && (cSign <= 90)) return cSign - 65;
	// digits
	if ((cSign >= 48) && (cSign <= 57)) return cSign - 22;
	// space
	else if (cSign == ' ') return 67;
	// .
	else if (cSign == '.') return 63;
	// ,
	else if (cSign == ',') return 64;
	// !
	else if (cSign == '!') return 65;
	// ?
	else if (cSign == '?') return 66;
	// : z odstepem
	else if (cSign == ':') return 68;
	// : bez odstepu
	else if (cSign == 0x80) return 69;
	// smile
	else if (cSign == 0x7F) return 70;
	// -
	else if (cSign == '-') return 71;
	// #
	else if (cSign == '#') return 82;
	// $
	else if (cSign == '$') return 83;
	// %
	else if (cSign == '%') return 84;
	// &
	else if (cSign == '&') return 85;
	// (
	else if (cSign == '(') return 86;
	// )
	else if (cSign == ')') return 87;
	// +
	else if (cSign == '+') return 89;
	// *
	else if (cSign == '*') return 90;
	/*
	 *
	 *		polish chars
	 *
	 */
	// ¹
	else if (cSign == 0xA5) return 72;
	// æ
	else if (cSign == 0x86) return 73;
	// ê
	else if (cSign == 0xA9) return 74;
	// ³
	else if (cSign == 0x88) return 75;
	// ñ
	else if (cSign == 0xE4) return 76;
	// ó
	else if (cSign == 0xA2) return 77;
	// œ
	else if (cSign == 0x98) return 78;
	// ¿
	else if (cSign == 0xBE) return 79;
	// Ÿ
	else if (cSign == 0xAB) return 80;
	// empty
	else if (cSign == 7) return 81;
	// error
	else return 88;
}

/*//! ladowanie tekstu z pamieci programu do bufora znakowego
! @param 		buffer bufor do ktorego zostana zaladowane dane
 *  @param 		data adres mapieci EEPROM
static void LoadTextFromPGM(char buffer[], const char *data) {
	uint8_t i = 0;
	while (pgm_read_byte(data))
		buffer[i++] = pgm_read_byte(data++);
	buffer[i] = '\0';
} // END static void LoadTextFromPGM*/

/*! @param 		uiIntSign internal char code in uiAlphabet
 *  @param 		uiByteNr number of vertical byte
 *  @return		vertical byte data of char */
uint8_t LoadIntSignByte(uint8_t uiIntSign, uint8_t uiByteNr) {
	if (uiByteNr > 4) return 0;
	else return ALPHABET_YX(uiIntSign, uiByteNr);
} // END uint8_t LoadSignByte

/*! @param 		uiIntSign internal char code in uiAlphabet
 *  @return		vertical length of uiAlphabet chars*/
uint8_t LoadIntSignLength(uint8_t uiIntSign) {
	if (uiIntSign >= ALPHABET_SIZE) return 0;
	else return AL_LENGTH(uiIntSign);
} // END uint8_t LoadSignByte

/*! Legend of char length
 * 5: 0..4 -- |p    |
 * 4: 0..3 <- |p    |
 * 3: 1..3 -> | p   |
 * 2: 1..2 <- | p   |
 * 1: 2..2 -> |  p  |
 *  @param		cSign ASCII code of char
 *  @param		uiStart_s start x coorinate
 *  @param		uiStop_s end x coorinate
 *  @param		uiIntSign_s internal char code in uiAlphabet*/
void LoadSign(char cSign, uint8_t *uiStart_s, uint8_t *uiStop_s, uint8_t *uiIntSign_s) {
	//*uiIntSign_s = TranslateToAlphabet(cSign); old
	*uiIntSign_s = cSign; // new
	uint8_t uiSize = AL_LENGTH(*uiIntSign_s);
	*uiStart_s = (5 - uiSize) / 2;
	*uiStop_s = *uiStart_s + uiSize - 1;
} // END void LoadSign

/*! @param 		eSign code of animation
 *  @param		uiByteNr number of vertical byte of animation
 *  @return		one byte of animation from animation table*/
uint8_t LoadAnimationsByte(AnimationsCode eSign, uint8_t uiByteNr) {
	if ((uiByteNr >= 6) && (eSign >= ANIMATIONS_SIZE)) return 0;
		else return ANIMATIONS_YX(eSign, uiByteNr);
} // END uint8_t LoadAnimationsByte

/* @param 		buffer bufor do ktorego zostana zaadowane dane
 *  @param		uiTextNr ikdens tekstu
void LoadText(char buffer[], uint8_t uiTextNr ) {
	switch (uiTextNr) {
		case 0: {
		  //LoadTextFromE(buffer, PSTR("ABCDEFGHIJKLMNOPQRST\0"));
		  //LoadTextFromE(buffer, PSTR("abcdefghijklmnopqrstYZX\0"));
			LoadTextFromPGM(buffer, PSTR("Witamy na Roboday 2017\0"));
			//LoadTextFromE(buffer, PSTR("s\x82\x83\x84\x85\x86\x87\x88\x89\x8A\0"));
		} break;
		case 1: {
			LoadTextFromPGM(buffer, PSTR("Wy\x88wietlacz Widmowy x7\0"));
		} break;
		case 2: {
			LoadTextFromPGM(buffer, PSTR("Prowadz\x82\acy drKlaus\0"));
		} break;
		case 3: {
			LoadTextFromPGM(buffer, PSTR("Zapraszamy do stoiska\x7F\0"));
		} break;
		default: {
			LoadTextFromPGM(buffer, PSTR("Error no text/\0"));
		}
	}
} // END void LoadText*/

const char string_1[MAX_TEXT_SIZE] PROGMEM = "Witamy na Roboday 2017\0";
const char string_2[MAX_TEXT_SIZE] PROGMEM = "Wy\x88wietlacz Widmowy x7\0";
const char string_3[MAX_TEXT_SIZE] PROGMEM = "Prowadz\x82\acy drKlaus\0";
const char string_4[MAX_TEXT_SIZE] PROGMEM = "Zapraszamy do stoiska\x7F\0";
const char string_5[MAX_TEXT_SIZE] PROGMEM = "Error no text/\0";

PGM_P const string_table[] PROGMEM =
{
   string_1,
   string_2,
   string_3,
   string_4,
   string_5
};

/*!
 * @param		buffer with length from MAX_TEXT_SIZE
 * @param		uiTextNr number of text from string_table {0..TEXT_NUMBER-1}*/
void LoadText(char buffer[], uint8_t uiTextNr ) {
	if (uiTextNr < TEXT_NUMBER)
		strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[uiTextNr])));
}
