#ifndef ColorPanelAnimations_h
#define ColorPanelAnimations_h

#include <inttypes.h>

#include "FastLED.h"

class ColorTransaction{
  public:
	ColorTransaction();
	void ToColor(CRGB startColor, CRGB toColor, byte steps);
	void ToColor(CRGB startColor, CRGB toColor, byte steps, int skipSteps);
	void NextFrame();
	bool InAnimation();
	
	CRGB CurrentColor;
	CRGB TargetColor;
	byte StepsLeft = -1;
	int SkipSteps = -1;
	
	void Lock();
	void UnLock();
	//double currentR;
	//double currentG;
	//double currentB;
	
  private:
  
	//double rDelta;
	//double gDelta;
	//double bDelta;
	

  	bool isLocked;

    
};

#endif


