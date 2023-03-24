
#ifndef ProjectorPower_H
#define ProjectorPower_H

#include "Arduino.h"
#include "ParalelTimer.h"

#include "ACS712.h"


typedef void (*OnClick_b)();  // alarm callback function typedef

class ProjectorPower{
  public:
    ProjectorPower();
    void init();

	void loop();
	void projectorOn (OnClick_b clickHandler);
	void projectorOff (OnClick_b clickHandler);
	
	
	int current;
  private: 
	int limit = 2200;
	int prevCurrent; 
	bool isPrevMore = false;
	bool isProjectorOn = false;
	bool isFirstStart = true;
	
    OnClick_b projectorOnFunction;
	OnClick_b projectorOffFunction;
	
	ACS712 acs = ACS712(A0);
	ParalelTimer buttonTimer = ParalelTimer(50);
	ParalelTimer stabilizationTimer = ParalelTimer(500);
};

#endif

