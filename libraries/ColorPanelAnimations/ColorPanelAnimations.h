#ifndef ColorPanelAnimations_h
#define ColorPanelAnimations_h

#include <inttypes.h>
#include "Arduino.h"
#include "ParalelTimer.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "LigthButton.h"

class ColorPanelAnimations{
  public:
    ColorPanelAnimations(int _pin);
	void LoopHandler();
    void ProgressAnimation(bool start);
	void Test();
	LigthButton Buttons[5];

  private:
	ParalelTimer timer_50 = ParalelTimer(50);
	Adafruit_NeoPixel strip  = Adafruit_NeoPixel(17, 7, NEO_GRB + NEO_KHZ800);
	int i = 0;
	bool isProgressAnimationStarted = false;
	void progress();
	int getPos(int shift, int original);
	int calculateValue(int c, int i, int interationValue, int channel);
	uint32_t wheel(byte WheelPos);

	//byte colors[12][3]={
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	///* {0,0,0},
	// {0,0,0},
	// {0,0,0},
	// {0,0,0},
	//*/
	// {0,200,0},
	// {0,100,0},
	// {0,99,0},
	// {0,0,0},
	//};
	//
	int n = 3;

	byte diffusedColors[12*3][3];

	byte colors[13][3]={
	 {0,0,0},
	 {0,0,0},
	 {0,0,0},
	 {255,0,0},
	 {255,30,0},
	 {255,60,0},
	 {255,90,0},
	 {255,120,0},
	 {255,150,0},
	 {255,180,0},
	 {255,210,0},
	 {255,255,0},
	 {0,0,0},
	};
	int aaa = 100;
	byte opacity[12]={0,0,0,2,5,7,15,30,50,60,80,100};
	//byte opacity[12]={aaa,aaa,aaa,aaa,aaa,aaa,aaa,aaa,aaa,aaa,aaa,aaa};
};

#endif


