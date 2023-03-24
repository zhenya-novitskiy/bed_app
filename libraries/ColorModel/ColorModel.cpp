#include "ColorModel.h"
#include "Adafruit_NeoPixel.h"

ColorModel::ColorModel(uint8_t r, uint8_t g, uint8_t b)
{
	R = r;
	G = g;
	B = b;
}

ColorModel::ColorModel()
{
}

uint32_t ColorModel:: Get()
{
	int ri = (int)R;
	int gi = (int)G;
	int bi = (int)B;
	return ((uint32_t)ri << 16) | ((uint32_t)gi <<  8) | (uint32_t)bi;
}
