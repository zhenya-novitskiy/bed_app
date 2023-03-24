#ifndef ColorButton_h
#define ColorButton_h

#include <inttypes.h>
#include "Arduino.h"
#include "ParalelTimer.h"
#include "ColorModel.h"
#include <Adafruit_NeoPixel.h>

#define PULLUP HIGH
#define PULLDOWN LOW

#define CURRENT 0
#define PREVIOUS 1
#define CHANGED 2

typedef void (*OnClick)();

class ColorButton{
  public:
	ColorButton(int ledPosition, Adafruit_NeoPixel* strip, ColorModel* defaultColor, uint8_t buttonPin, uint8_t buttonMode=PULLDOWN);
	void loop();
	void subscribeOnClick (OnClick clickHandler, ColorModel* color);
	void subscribeOnUnClick (OnClick clickHandler);
	void subscribeOnPressed (int interval, OnClick clickHandler, ColorModel* color);
	void subscribeOnPressedMoreThan (int interval, OnClick clickHandler, ColorModel* color);
	
  private:
	uint32_t CurrentColor();
	void NextFrame();
	int TransactionSteps;
	ColorModel currentColor;
    void ToColor(int steps, ColorModel toColor);
    Adafruit_NeoPixel* Strip;
	int ledPosition;
    int currentAnimationStep = 0;
    void pullup();
    void pulldown();
    bool isPressed();
    bool wasPressed();
    bool stateChanged();
	bool uniquePress();
    double rDelta;
	double gDelta;
	double bDelta;
	ColorModel* onClickColor;
	ColorModel* onUnClickColor;
	ColorModel* onPressedColor;
	ColorModel* onPressedAfterColor;
    OnClick onClickFunction;
	OnClick onUnClickFunction;
	OnClick onPressedFunction;
	OnClick onPressedAfterFunction;
	uint8_t pin;
    uint8_t mode;
    uint8_t state;
	ParalelTimer buttonTimer = ParalelTimer(50);
	ParalelTimer buttonPressTimer = ParalelTimer(150);
	ParalelTimer buttonPressMoreThanTimer = ParalelTimer(150);
	bool isPressedState = false;
	bool stateChangedState = false;
	bool timerRestarted = false;
	bool pressedAfterOnce = true;
};

#endif


