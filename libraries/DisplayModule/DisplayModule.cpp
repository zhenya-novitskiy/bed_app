#include "Arduino.h"
#include "DisplayModule.h"
#include "PinsExtentionModule.h"
//
//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>


static const uint16_t alphafonttable[] PROGMEM =  {

0b0000000000000001,
0b0000000000000010,
0b0000000000000100,
0b0000000000001000,
0b0000000000010000,
0b0000000000100000,
0b0000000001000000,
0b0000000010000000,
0b0000000100000000,
0b0000001000000000,
0b0000010000000000,
0b0000100000000000,
0b0001000000000000,
0b0010000000000000,
0b0100000000000000,
0b1000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0000000000000000,
0b0001001011001001,
0b0001010111000000,
0b0001001011111001,
0b0000000011100011,
0b0000010100110000,
0b0001001011001000,
0b0011101000000000,
0b0001011100000000,
0b0000000000000000, //  
0b0000000000000000, // !
0b0000000000000000, // "
0b0000000000000000, // #
0b0000000000000000, // $
0b0000000000000000, // %
0b0000000000000000, // &
0b0000000000000000, // '
0b0000000000000000, // (
0b0000000000000000, // )
0b0000000000000000, // *
0b0000000000000000, // +
0b0000000000000000, // ,
0b0000000000000000, // -
0b0000000001000000, // .
0b0000100000000010, // /
0b0110000110100001, // 0
0b0100000010000000, // 1
0b0111000000110001, // 2
0b0111000010110000, // 3
0b0101000110010000, // 4
0b0011000110110000, // 5
0b0011000110110001, // 6
0b0110000010000000, // 7
0b0111000110110001, // 8
0b0111000110110000, // 9
0b0000000000000000, // :
0b0000000000000000, // ;
0b0000000000000000, // <
0b0000000000000000, // =
0b0000000000000000, // >
0b0000000000000000, // ?
0b0000000000000000, // @
0b0111000110010001, // A
0b0110010010110100, // B
0b0010000100100001, // C
0b0110010010100100, // D
0b0011000100100001, // E
0b0011000100000001, // F
0b0010000110110001, // G
0b0101000110010001, // H
0b0000010000000100, // I
0b0100000010100001, // J
0b0001100100001001, // K
0b0000000100100001, // L
0b0100101110000001, // M
0b0100001110001001, // N
0b0110000110100001, // O
0b0111000100010001, // P
0b0110000110101001, // Q
0b0111000100011001, // R
0b0011000110110000, // S
0b0010010000000100, // T
0b0100000110100001, // U
0b0000100100000011, // V
0b0100000110001011, // W
0b0000101000001010, // X
0b0000101000000100, // Y
0b0010100000100010, // Z
0b0010000100100001, // [
0b0000000000000000, // 
0b0110000010100000, // ]
0b0000000000000000, // ^
0b0000000000001000, // _
0b0000000000000000, // `
0b0111000110010001, // a
0b0110010010110100, // b
0b0010000100100001, // c
0b0110010010100100, // d
0b0011000100100001, // e
0b0011000100000001, // f
0b0010000110110001, // g
0b0101000110010001, // h
0b0000010000000100, // i
0b0100000010100001, // j
0b0001100100001001, // k
0b0000000100100001, // l
0b0100101110000001, // m
0b0100001110001001, // n
0b0110000110100001, // o
0b0111000100010001, // p
0b0110000110101001, // q
0b0111000100011001, // r
0b0011000110110000, // s
0b0010010000000100, // t
0b0100000110100001, // u
0b0000100100000011, // v
0b0100000110001011, // w
0b0000101000001010, // x
0b0000101000000100, // y
0b0010100000100010, // z
0b0000000000000000, // {
0b0000000000000000, // |
0b0000000000000000, // }
0b0000000000000000, // ~
0b0011111111111111,

};

DisplayModule::DisplayModule(byte *digitsPins, size_t len1, byte *segmentsPins, size_t len2, uint16_t *displaybuffer)
{
	digitsPinsNumbers = digitsPins;
	segmentsPinsNumbers = segmentsPins;
	digitsPinsNumbersLen = len1;
	segmentsPinsNumbersLen = len2;
	buffer = displaybuffer;
}

void DisplayModule::SetupPinsExtentionModule(PinsExtentionModule *pinsExtModule)
{
    pinsExtentionModule = pinsExtModule;
}

uint8_t DisplayModule::WriteDigit(uint8_t a) {
	
	 uint16_t font = pgm_read_word(alphafonttable+a);
	 uint16_t result = buffer[1];
	buffer[1] = buffer[2];
	buffer[2] = buffer[3];
	buffer[3] = buffer[4];
	buffer[4] = font;
	
	return result;
}

void DisplayModule::WriteDigit2(uint8_t a) {

	buffer[1] = buffer[2];
	buffer[2] = buffer[3];
	buffer[3] = buffer[4];
	buffer[4] = a;
}

void DisplayModule::WriteDigitAscii(uint8_t n, uint8_t a) {
  uint16_t font = pgm_read_word(alphafonttable+a);
   for (int d = 0; d < segmentsPinsNumbersLen ; d++) {
			
			 Serial.print(bitRead(font, d));
	 }
   Serial.println("");
 
  buffer[n] = font;
  		pinsExtentionModule->AllToLow();
	  
	 
	 // pinsExtentionModule->Push();
		uint16_t data = buffer[encounter+1];

		 for (int n = 0; n < segmentsPinsNumbersLen ; n++) {
				pinsExtentionModule->Set(segmentsPinsNumbers[n], bitRead(data, n));
		 }
		 pinsExtentionModule->Push();
}

void DisplayModule::LoopHandler()
{
    ActivateNumber(encounter);
	pinsExtentionModule->Push();
	
	encounter++;
	if (encounter > digitsPinsNumbersLen-1)
	{
		encounter = 0;
	}
}


void DisplayModule::ActivateNumber(byte number)
{
	for (int n = 0; n < digitsPinsNumbersLen ; n++) {
			pinsExtentionModule->Set(digitsPinsNumbers[n], 1);
	 }
	pinsExtentionModule->Set(digitsPinsNumbers[number], 0);
}
