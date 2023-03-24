#include "Arduino.h"
#include "LigthButton.h"
//
//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>

LigthButton::LigthButton()
{
}

void LigthButton::SetColor(byte r, byte g, byte b)
{
	Color[0]  = r;
	Color[1]  = g;
	Color[2]  = b;
}
