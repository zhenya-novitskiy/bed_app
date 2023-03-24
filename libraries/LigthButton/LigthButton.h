#ifndef LigthButton_h
#define LigthButton_h

#include <inttypes.h>
#include "Arduino.h"

class LigthButton{
  public:
    LigthButton();
    bool State = false;
	byte Color[3] = {0,0,0};
	void SetColor(byte r, byte g, byte b);
  private:
    int current;
    int prev;
	int miliseconds;
};

#endif
