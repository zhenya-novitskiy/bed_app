#include "Arduino.h"
#include "ChargeAnimation.h"
#include "ColorTransaction.h"
#include "ParalelTimer.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "ColorModel.h"
//

ChargeAnimation::ChargeAnimation(int intervalMiliseconds, int steps, double brightness, Adafruit_NeoPixel strip)
{
	 strip.setBrightness(65);
	 Brightness = brightness;
	 timer.Miliseconds = intervalMiliseconds;
	 TransactionSteps = steps;
	 Strip = strip;
	 Size = sizeof(Transactions) / sizeof(ColorTransaction);
}

void ChargeAnimation :: SetColors(byte **colors)
{
	//Colors = colors;
}

void ChargeAnimation :: SetMode(int mode)
{
	if (this->mode != mode && mode == 1)
	{
		timer.Miliseconds = 5;
	    TransactionSteps = 20;
		for (int i = 0; i < Size; i++) {
		   Transactions[i].TransactionSteps = TransactionSteps;
		}
		CurrentStep = -1;
	}
	
	if (this->mode != mode && mode == 2)
	{
		timer.Miliseconds = 5;
	    TransactionSteps = 20;
		for (int i = 0; i < Size; i++) {
		   Transactions[i].TransactionSteps = TransactionSteps;
		}
		CurrentStep = -1;
	}
	
	if (this->mode != mode && mode == 3)
	{
		timer.Miliseconds = 5;
	    TransactionSteps = 200;
		animationIndex = 0;
		
		Transactions[1].TransactionSteps = TransactionSteps;
		Transactions[2].TransactionSteps = TransactionSteps;
		
		CurrentStep = -1;
	}
	this->mode = mode;
}

void ChargeAnimation :: Loop()
 {
	
	
	if (mode == 1)
	{
		if (CurrentStep == -1)
		{
			for (int i = 0; i < Size; i++) {
				Transactions[i].ToColor(ColorModel(Colors1[i][0], Colors1[i][1], Colors1[i][2]), ColorModel(Colors1[i+1][0], Colors1[i+1][1], Colors1[i+1][2]));
			}
			CurrentStep = TransactionSteps;
			position = GetNextPosition(position);
		}
		
		if (timer.ItsTime())
		{
			for (int i = 0; i < Size; i++) 
			{
				Transactions[i].NextFrame();
				Strip.setPixelColor(GetPosition(1-i), Transactions[i].CurrentColor());
				Strip.setPixelColor(GetPosition(18-i), Transactions[i].CurrentColor());
			}
			
			Strip.show();
			CurrentStep--;	
		}
	}
	if (mode == 2)
	{
		if (CurrentStep == -1)
		{
			for (int i = 0; i < Size; i++) {
			 Transactions[i].ToColor(ColorModel(Colors2[i][0], Colors2[i][1], Colors2[i][2]), ColorModel(Colors2[i+1][0], Colors2[i+1][1], Colors2[i+1][2]));
			}
			
			CurrentStep = TransactionSteps;
			position = GetNextPosition(position);
		}
		
		if (timer.ItsTime())
		{
			for (int i = 0; i < Size; i++) 
			{
				Transactions[i].NextFrame();
				Strip.setPixelColor(GetPosition(1-i), Transactions[i].CurrentColor());
				Strip.setPixelColor(GetPosition(18-i), Transactions[i].CurrentColor());
			}
			
			Strip.show();
			CurrentStep--;	
		}
	}
	
	if (mode == 3)
	{
		if (CurrentStep == -1)
		{
			animationIndex++;
			if (animationIndex >= 10)
			{
				animationIndex = 0;
			}
			
            Transactions[1].ToColor(ColorModel(Colors3[animationIndex][0], Colors3[animationIndex][1], Colors3[animationIndex][2]), ColorModel(Colors3[animationIndex+1][0], Colors3[animationIndex+1][1], Colors3[animationIndex+1][2]));
			
			CurrentStep = TransactionSteps;
		}
		
		Transactions[1].NextFrame();
		
		for (int i = 0; i < 35; i++) 
		{
			Strip.setPixelColor(i, Transactions[1].CurrentColor());
		}
		
		Strip.show();
		CurrentStep--;	
	
		
	}
 }
 
 
int ChargeAnimation :: GetNextPosition(int currentPosition)
{
  currentPosition++;
  if (currentPosition > 34)
  {
    currentPosition = 0;
  }
  return currentPosition;
}

int ChargeAnimation :: GetPosition(int offset)
{
  if (position + offset > 34)
  {
    return position - 35 + offset;
  }
  else if (position + offset < 0)
  {
    return position + 35 + offset;
  }
  else
  {
    return position + offset;
  }
}
