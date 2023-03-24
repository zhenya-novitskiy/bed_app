/*
||
|| @file Button.h
|| @version 1.6
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making buttons
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "ParalelTimer.h"

#define PULLUP HIGH
#define PULLDOWN LOW

#define CURRENT 0
#define PREVIOUS 1
#define CHANGED 2

typedef void (*OnClick_b)();  // alarm callback function typedef

class Button{
  public:
    Button();
    Button(uint8_t buttonPin, uint8_t buttonMode=PULLDOWN);
    void pullup();
    void pulldown();
    bool isPressed();
    bool wasPressed();
    bool stateChanged();
	bool uniquePress();
	void init();
	
	void loop();
	void subscribeOnClick (OnClick_b clickHandler);
	void subscribeOnUnClick (OnClick_b clickHandler);
	void subscribeOnDoubleClick (OnClick_b clickHandler);
	void subscribeOnPressed (int interval, OnClick_b clickHandler);
	void subscribeOnPressedMoreThan (int interval, OnClick_b clickHandler);
	
	bool wasClickBefore = false;
	bool wasUnClickBefore = false;
	bool isPressedState = false;
	bool stateIsChanged = false;
	bool timerRestarted = false;
	bool pressedAfterOnce = true;
	
  private: 
    uint8_t pin;
    uint8_t mode;
    uint8_t state;
	OnClick_b onClickFunction;
	OnClick_b onUnClickFunction;
	OnClick_b onDoubleClickFunction;
	OnClick_b onPressedFunction;
	OnClick_b onPressedAfterFunction;
	ParalelTimer buttonTimer = ParalelTimer(50);
	ParalelTimer buttonPressTimer = ParalelTimer(10000);
	ParalelTimer buttonPressMoreThanTimer = ParalelTimer(10000);
	
	
	ParalelTimer buttonDoubleClickTimer = ParalelTimer(300);
	
	
};

#endif
