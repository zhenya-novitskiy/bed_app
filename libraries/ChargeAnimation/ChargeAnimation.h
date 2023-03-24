#ifndef ChargeAnimation_h
#define ChargeAnimation_h

#include <inttypes.h>
#include "Arduino.h"
#include "ParalelTimer.h"
#include "ColorTransaction.h"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "ColorModel.h"

class ChargeAnimation{
  public:
	ChargeAnimation(int intervalMiliseconds, int steps, double brightness, Adafruit_NeoPixel strip);
	void SetColors(byte **colors);
	void SetMode(int mode);
	ColorTransaction Transactions[10];
	int Size;
	void Loop();
	int CurrentStep = -1;

  private:
	int GetNextPosition(int currentPosition);
    int GetPosition(int offset);
	int mode = 0;
    int position = 0;
	Adafruit_NeoPixel Strip;
    double Brightness;
	int TransactionSteps;
	ParalelTimer timer = ParalelTimer();
	int animationIndex = 0;
	
	 byte Colors1[11][3] = {
     {0, 0, 0},
     {250, 0, 0},
     {250, 5, 0},
     {220, 30, 0},
     {210, 30, 0},
     {200, 50, 0},
     {150, 50, 0},
     {100, 25, 0},
     {70, 25, 0},
     {50, 25, 0},
     {0, 0, 0},
     };
  

	byte Colors2[11][3] = {
    {80, 80, 80},
    {110, 110, 110},
    {150, 150, 150},
    {180, 180, 180},
    {220, 220, 220},
    {250, 250, 250},
    {220, 220, 220},
    {180, 180, 180},
    {150, 150, 150},
    {110, 110, 110},
    {80, 80, 80},
  };
  
  	 byte Colors3[11][3] = {
     {50, 50, 50},
     {25, 125, 25},
     {0, 250, 0},
     {0, 250, 0},
     {0, 200, 0},
     {0, 150, 0},
     {25, 100, 25},
     {25, 50, 25},
     {50, 50, 50},
     {50, 50, 50},
     {50, 50, 50},
     };
};

#endif


