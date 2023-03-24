#ifndef ParalelTimer_h
#define ParalelTimer_h

#include <inttypes.h>
#include "Arduino.h"

class ParalelTimer{
  public:
    ParalelTimer(int milisec);
	ParalelTimer();
    bool ItsTime();
    void RestartClock();
	int Miliseconds;

	private:
    int current;
    int prev;
};

#endif
