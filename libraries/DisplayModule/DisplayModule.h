#ifndef DisplayModule_h
#define DisplayModule_h

#include <inttypes.h>
#include "Arduino.h"
#include "PinsExtentionModule.h"

class DisplayModule{
  public:
    DisplayModule(byte *digitsPins, size_t len1, byte *segmentsPins, size_t len2, uint16_t *displaybuffer);
	void SetupPinsExtentionModule(PinsExtentionModule *pinsExtModule);
	void LoopHandler();
	void WriteDigitAscii(uint8_t n, uint8_t a);
	uint8_t WriteDigit(uint8_t a);
	void WriteDigit2(uint8_t a);
  private:
    PinsExtentionModule *pinsExtentionModule;
	byte *digitsPinsNumbers;
	size_t digitsPinsNumbersLen;
	byte *segmentsPinsNumbers;
	size_t segmentsPinsNumbersLen;
	uint16_t *buffer;
	int encounter = 0;
	void ActivateNumber(byte number);
};

#endif
