#include "Arduino.h"
#include "ParalelTimer.h"
#include <Wire.h>
#include <DS3231.h>

typedef void (*OnClick_b)();  // alarm callback function typedef

class TimeClock{
  public:
    TimeClock();
    TimeClock(byte nightStartHour, byte nightEndHour);
	
	void loop();
	void nightStarted (OnClick_b clickHandler);
	void morningStarted (OnClick_b clickHandler);
	
	RTClib clock2;
	byte nightStartHour; 
	byte nightEndHour; 
	
	bool isNightNow();
	
  private: 
	OnClick_b isNightFunction;
	OnClick_b isMorningFunction;
	ParalelTimer timer = ParalelTimer(1000);
	bool isNight = false;
};


