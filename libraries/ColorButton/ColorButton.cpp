#include "Arduino.h"
#include "ColorButton.h"
#include "ParalelTimer.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "ColorModel.h"
#include <Adafruit_NeoPixel.h>

ColorButton::ColorButton(int ledPosition, Adafruit_NeoPixel* strip, ColorModel* defaultColor, uint8_t buttonPin, uint8_t buttonMode)
{
	this->onUnClickColor = defaultColor;
	this->Strip = strip;
	this->ledPosition = ledPosition;
	this->pin=buttonPin;
    pinMode(pin,INPUT);
	buttonMode==PULLDOWN ? pulldown() : pullup();
    state = 0;
    bitWrite(state,CURRENT,!mode);
	currentColor = ColorModel(255,0,0);
}

void ColorButton::ToColor(int steps, ColorModel toColor)
{
	rDelta = (toColor.R - currentColor.R)/steps;
	gDelta = (toColor.G - currentColor.G)/steps;
	bDelta = (toColor.B - currentColor.B)/steps;
	currentAnimationStep = steps;
}

void ColorButton::NextFrame()
{
	double substructedValue = currentColor.R + rDelta;
	currentColor.R = substructedValue < 0 ? 0 : substructedValue;
	
	substructedValue = currentColor.G + gDelta;
	currentColor.G = substructedValue < 0 ? 0 : substructedValue;
	
	substructedValue = currentColor.B + bDelta;
	currentColor.B = substructedValue < 0 ? 0 : substructedValue;
}

uint32_t ColorButton::CurrentColor()
{
	return ColorModel(currentColor.R, currentColor.G, currentColor.B).Get();
}

void ColorButton::pullup(void){
	mode=PULLUP;
	digitalWrite(pin,HIGH);
}

void ColorButton::pulldown(void){
	mode=PULLDOWN;
	//digitalWrite(pin,LOW);
}

bool ColorButton::isPressed(void){
    bitWrite(state,PREVIOUS,bitRead(state,CURRENT));
    if (digitalRead(pin) == mode){
        bitWrite(state,CURRENT,false);
    } else {
        bitWrite(state,CURRENT,true);
    }
    if (bitRead(state,CURRENT) != bitRead(state,PREVIOUS)){
        bitWrite(state,CHANGED,true);
    }else{
        bitWrite(state,CHANGED,false);
    }
	return bitRead(state,CURRENT);
}

bool ColorButton::wasPressed(void){
    if (bitRead(state,CURRENT)){
        return true;
    } else {
        return false;
    }
}

bool ColorButton::stateChanged(void){
    return bitRead(state,CHANGED);
}

bool ColorButton::uniquePress(void){
    return (isPressed() && stateChanged());
}

void ColorButton::subscribeOnClick(OnClick clickHandler, ColorModel* color)
{
	onClickColor = color;
	onClickFunction = clickHandler;
}
void ColorButton::subscribeOnUnClick(OnClick clickHandler)
{
	onUnClickFunction = clickHandler;
}
void ColorButton::subscribeOnPressed(int interval, OnClick clickHandler, ColorModel* color)
{
	onPressedColor = color;
	onPressedFunction = clickHandler;
	buttonPressTimer.Miliseconds = interval;
}
void ColorButton::subscribeOnPressedMoreThan(int interval, OnClick clickHandler, ColorModel* color)
{
	onPressedAfterColor = color;
	onPressedAfterFunction = clickHandler;
	buttonPressMoreThanTimer.Miliseconds = interval;
}

void ColorButton::loop()
{
	if (currentAnimationStep != 0)
	{
		currentAnimationStep --;
		NextFrame();
		(*Strip).setPixelColor(ledPosition, CurrentColor());
		(*Strip).setPixelColor(ledPosition+1, CurrentColor());
		(*Strip).show();
	}
	
	if (buttonTimer.ItsTime())
	{
			isPressedState = isPressed();
			stateChangedState = stateChanged();
			
			if (stateChangedState)
			{
				if (isPressedState)
				{
					if (!timerRestarted)
					{
						buttonPressMoreThanTimer.RestartClock();	
						timerRestarted = true;
					}
					if (onClickFunction != NULL) 
					{
						(*onClickFunction)();
						ToColor(150, *onClickColor);
					}
				}else
				{
					if (timerRestarted)
					{
						timerRestarted = false;
					}
					
					if (pressedAfterOnce)
					{
						if (onUnClickFunction != NULL) 
						{
							(*onUnClickFunction)();
							ToColor(300, *onUnClickColor);
						}
					}else
					{
						pressedAfterOnce = true;
						ToColor(150, *onUnClickColor);
					}
				}
			}
	}
	
	if (buttonPressTimer.ItsTime() && isPressedState)
	{
		if (onPressedFunction != NULL && pressedAfterOnce) 
		{
			ToColor(150, *onPressedColor);
			(*onPressedFunction)();
		}
	}
	
	if (buttonPressMoreThanTimer.ItsTime() && isPressedState)
	{
		if (onPressedAfterFunction != NULL && pressedAfterOnce) 
		{
			(*onPressedAfterFunction)();
			ToColor(600, *onPressedAfterColor);
			pressedAfterOnce = false;
		}
	}
}

