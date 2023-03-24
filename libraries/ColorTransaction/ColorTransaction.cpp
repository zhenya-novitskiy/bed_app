#include "Arduino.h"
#include "ColorTransaction.h"
#include "FastLED.h"
//

ColorTransaction::ColorTransaction()
{
	isLocked = false;
}

void ColorTransaction::ToColor(CRGB startColor, CRGB toColor, byte steps)
{
	if (!isLocked)
	{
		
		CurrentColor = startColor;
		TargetColor = toColor;
		
		//currentR = startColor.r;
		//currentG = startColor.g;
		//currentB = startColor.b;

		//rDelta = (toColor.r - currentR)/TransactionSteps;
		//gDelta = (toColor.g - currentG)/TransactionSteps;
		//bDelta = (toColor.b - currentB)/TransactionSteps;

		StepsLeft = steps;
	}
}


void ColorTransaction::ToColor(CRGB startColor, CRGB toColor, byte steps, int skipSteps)
{
	if (!isLocked)
	{
		
		CurrentColor = startColor;
		TargetColor = toColor;

		StepsLeft = steps;
		SkipSteps = skipSteps;
	}
}

void ColorTransaction::NextFrame()
{
	if (SkipSteps > 0)
	{
		SkipSteps--;
		return;
		
	}

	
	if (StepsLeft > 0)
	{
		double rDelta = (TargetColor.r - CurrentColor.r)/StepsLeft;
		double gDelta = (TargetColor.g - CurrentColor.g)/StepsLeft;
		double bDelta = (TargetColor.b - CurrentColor.b)/StepsLeft;
		
		double substructedValue = CurrentColor.r + rDelta;
		
		CurrentColor.r = substructedValue < 0 ? 0 : substructedValue;
		
		substructedValue = CurrentColor.g + gDelta;
		CurrentColor.g = substructedValue < 0 ? 0 : substructedValue;
		
		substructedValue = CurrentColor.b + bDelta;
		CurrentColor.b = substructedValue < 0 ? 0 : substructedValue;

		StepsLeft--;	
	}
	
}


void ColorTransaction::Lock()
{
	isLocked = true;
}

void ColorTransaction::UnLock()
{
	isLocked = false;
}

bool ColorTransaction::InAnimation()
{
	return StepsLeft > 0;
}