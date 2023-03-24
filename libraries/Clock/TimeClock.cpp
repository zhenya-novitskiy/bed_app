#include "TimeClock.h"
#include <Wire.h>
#include <DS3231.h>

TimeClock::TimeClock(byte nightStartHour, byte nightEndHour){
	this->nightStartHour=nightStartHour;
	this->nightEndHour=nightEndHour;
}

void TimeClock::nightStarted(OnClick_b clickHandler)
{
	isNightFunction = clickHandler;
}
void TimeClock::morningStarted(OnClick_b clickHandler)
{
	isMorningFunction = clickHandler;
}

void TimeClock::loop()
{
	if (timer.ItsTime())
	{
		
		int hour = clock2.now().hour();
		
		if (hour >= nightStartHour && hour <= nightEndHour)
		{
			if (!isNight)
			{
				isNight = true;
				if (isNightFunction != NULL) 
				{
					(*isNightFunction)();
					
				}
			}
		}else
		{
			if (isNight)
			{
				isNight = false;
				if (isMorningFunction != NULL) 
				{
					(*isMorningFunction)();
					
				}
			}
		}
	}
}

bool TimeClock::isNightNow()
{
	return isNight;
}
