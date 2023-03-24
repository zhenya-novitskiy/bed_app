#include "Arduino.h"
#include "ParalelTimer.h"
//
//#include <stdio.h>
//#include <string.h>
//#include <inttypes.h>

ParalelTimer::ParalelTimer(int milisec)
{
	current = millis();
	prev = millis();
	Miliseconds = milisec;
}

ParalelTimer::ParalelTimer()
{
	current = millis();
	prev = millis();
}

bool ParalelTimer::ItsTime()
{
	current = millis();

	if ((current - prev) > Miliseconds)
	{
		prev = current;
		return true;
	}else
	{
		return false;
	}
}

void ParalelTimer::RestartClock()
{
	current = millis();
	prev = current;
}
