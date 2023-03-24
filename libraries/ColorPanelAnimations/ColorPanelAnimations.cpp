#include "Arduino.h"
#include "ColorPanelAnimations.h"
#include "ParalelTimer.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
//

ColorPanelAnimations::ColorPanelAnimations(int _pin)
{
	//strip = Adafruit_NeoPixel(17, _pin, NEO_GRB + NEO_KHZ800);
	strip.begin();

	
}

void ColorPanelAnimations::ProgressAnimation(bool start)
{
	isProgressAnimationStarted = start;
}

void ColorPanelAnimations::Test()
{
	int c = 0;
	int iterationValue1 = 0;
	int iterationValue2 = 0;
	int iterationValue3 = 0;

	for (int i=0; i<sizeof(diffusedColors)/sizeof(*diffusedColors); i++)
	{
		/*Serial.println("---------------------------------------");
		Serial.println(i);*/
		
		if (i%n == 0)
		{
			diffusedColors[i][0] = iterationValue1 = colors[i/n][0];
			diffusedColors[i][1] = iterationValue2 = colors[i/n][1];
			diffusedColors[i][2] = iterationValue3 = colors[i/n][2];
			c+=n;
		}else
		{
			diffusedColors[i][0] = calculateValue(c,i, iterationValue1, 0);
			diffusedColors[i][1] = calculateValue(c,i, iterationValue2, 1);
			diffusedColors[i][2] = calculateValue(c,i, iterationValue3, 2);
		}
	
		/*Serial.print(diffusedColors[i][0]);
		Serial.print(",");
		Serial.print(diffusedColors[i][1]);
		Serial.print(",");
		Serial.print(diffusedColors[i][2]);
		Serial.println();
		Serial.println("---------------------------------------");*/
	}
}

int ColorPanelAnimations::calculateValue(int c,int i, int interationValue, int channel) {
	return interationValue + (i%n * ((colors[c/n][channel] - colors[(c-n)/n][channel])/n));
}

void ColorPanelAnimations::LoopHandler()
{
   if (timer_50.ItsTime())
   {
	   if (isProgressAnimationStarted)
	   {
	      progress();
	   }

	    for(int j=0; j<5; j++) {
			if (Buttons[j].State)
			{
				strip.setPixelColor(12 + j, Buttons[j].Color[0], Buttons[j].Color[1], Buttons[j].Color[2]);
			}
		}

   }
   strip.setPixelColor(1, 255,255,0);
   
   strip.show();
}



int abcd = 1;
void ColorPanelAnimations::progress()
{


	/*	Serial.println("||||||||||||||||||||||||||||||||||||||||");
    for(int j=0; j<12; j++) {
        int pixelNumber = getPos(i,j);
        strip.setPixelColor(pixelNumber, wheel(abcd * j));
    }
		Serial.println("||||||||||||||||||||||||||||||||||||||||");
	if (abcd > n+1)
	{
	abcd = 1;
	i++;
		if (i>11) i=0;

	}else
	{
	 abcd++;
	}*/

   
}
//void ColorPanelAnimations::progress()
//{
//    for(int j=0; j<12; j++) {
//        int pixelNumber = getPos(i,j);
//        strip.setPixelColor(pixelNumber, wheel(j));
//    }
//  
//   i++;
//   if (i>11) i=0;
//
//}

	

int ColorPanelAnimations::getPos(int shift, int original) {
    int sum = shift+original;
    
    if (sum>11)
    {
      return sum-12;
    }else
    {
      return sum;
    }
}

// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t ColorPanelAnimations::wheel(byte WheelPos) {
//  
//  return strip.Color(colors[WheelPos][0]/100.0*opacity[WheelPos], colors[WheelPos][1]/100.0*opacity[WheelPos], colors[WheelPos][2]/100.0*opacity[WheelPos]);
//}

uint32_t ColorPanelAnimations::wheel(byte WheelPos) {
	Serial.println("---------------------------------------");
	Serial.println(WheelPos);
	Serial.print(",");
  		Serial.print(diffusedColors[WheelPos][0]);
		Serial.print(",");
		Serial.print(diffusedColors[WheelPos][1]);
		Serial.print(",");
		Serial.print(diffusedColors[WheelPos][2]);
		Serial.println();
		Serial.println("---------------------------------------");
		delay(2000);
  return strip.Color(diffusedColors[WheelPos][0], diffusedColors[WheelPos][1], diffusedColors[WheelPos][2]);
}