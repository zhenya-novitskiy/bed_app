//include the class definition
#include "DoorSensor.h"

/*
|| <<constructor>>
|| @parameter buttonPin sets the pin that this switch is connected to
|| @parameter buttonMode indicates PULLUP or PULLDOWN resistor
*/
DoorSensor::DoorSensor(uint8_t buttonPin, uint8_t buttonMode){
	this->pin=buttonPin;
    pinMode(pin,INPUT);
	buttonMode==PULLDOWN ? pulldown() : pullup();
    state = 0;
    bitWrite(state,CURRENT,!mode);
}

/*
|| Set pin HIGH as default
*/
void DoorSensor::pullup(void){
	mode=PULLUP;
	digitalWrite(pin,HIGH);
}

/*
|| Set pin LOW as default
*/
void DoorSensor::pulldown(void){
	mode=PULLDOWN;
	//digitalWrite(pin,LOW);
}

/*
|| Return the bitWrite(state,CURRENT, of the switch
*/
bool DoorSensor::isPressed(void){
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

/*
|| Return true if the button has been pressed
*/
bool DoorSensor::wasPressed(void){
    if (bitRead(state,CURRENT)){
        return true;
    } else {
        return false;
    }
}

/*
|| Return true if state has been changed
*/
bool DoorSensor::stateChanged(void){
    return bitRead(state,CHANGED);
}

/*
|| Return true if the button is pressed, and was not pressed before
*/
bool DoorSensor::uniquePress(void){
    return (isPressed() && stateChanged());
}

void DoorSensor::doorOpen(OnClick_b clickHandler)
{
	doorOpenFunction = clickHandler;
}
void DoorSensor::doorClose(OnClick_b clickHandler)
{
	doorCloseFunction = clickHandler;
}

void DoorSensor::loop()
{
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
						timerRestarted = true;
					}
					if (doorCloseFunction != NULL) 
					{
						(*doorCloseFunction)();
					}
				}else
				{
					if (timerRestarted)
					{
						timerRestarted = false;
					}
					
					if (pressedAfterOnce)
					{
						if (doorOpenFunction != NULL) 
						{
							(*doorOpenFunction)();
						}
					}else
					{
						pressedAfterOnce = true;
					}
				}
			}
	}
}

void DoorSensor::init(void){
    if (digitalRead(pin)==PULLUP)
	{
		if (doorOpenFunction != NULL) 
		{
			(*doorOpenFunction)();
		}
	}
}
