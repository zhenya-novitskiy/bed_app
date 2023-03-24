#ifndef ColorModel_h
#define ColorModel_h


#include "Adafruit_NeoPixel.h"


class ColorModel{
  public:
	ColorModel(uint8_t r, uint8_t g, uint8_t b);
	ColorModel();
	uint32_t Get();
	double R, G, B;

  private:
};

#endif


