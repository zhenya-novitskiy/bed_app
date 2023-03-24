
#ifndef DOORSENSOR_H
#define DOORSENSOR_H

#include "Arduino.h"
#include "ParalelTimer.h"

#define PULLUP HIGH
#define PULLDOWN LOW

#define CURRENT 0
#define PREVIOUS 1
#define CHANGED 2

typedef void (*OnClick_b)();  // alarm callback function typedef

class DoorSensor{
  public:
    DoorSensor();
    DoorSensor(uint8_t buttonPin, uint8_t buttonMode=PULLDOWN);
    void pullup();
    void pulldown();
    bool isPressed();
    bool wasPressed();
    bool stateChanged();
	bool uniquePress();
	void init();
	
	void loop();
	void doorOpen (OnClick_b clickHandler);
	void doorClose (OnClick_b clickHandler);
	
  private: 
    uint8_t pin;
    uint8_t mode;
    uint8_t state;
	OnClick_b doorOpenFunction;
	OnClick_b doorCloseFunction;
	ParalelTimer buttonTimer = ParalelTimer(50);
	bool isPressedState = false;
	bool stateChangedState = false;
	bool timerRestarted = false;
	bool pressedAfterOnce = true;
};

#endif

