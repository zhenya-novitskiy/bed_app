#include "Arduino.h"
#include "ACS712.h"
#include "ProjectorPower.h"



ProjectorPower::ProjectorPower(){
	
}

void ProjectorPower::projectorOn(OnClick_b clickHandler)
{
	projectorOnFunction = clickHandler;
}
void ProjectorPower::projectorOff(OnClick_b clickHandler)
{
	projectorOffFunction = clickHandler;
}

void ProjectorPower::loop()
{
	current = acs.mA_AC();
	
	if (current > limit)
	{
		if (!isPrevMore)
		{
			stabilizationTimer.RestartClock();	
		}
		
		isPrevMore = true;
	}else
	{
		if (isPrevMore)
		{
			stabilizationTimer.RestartClock();	
		}
		
		isPrevMore = false;
	}
	
	if (stabilizationTimer.ItsTime())
	{
		if (current > limit)
		{
			if (!isProjectorOn || isFirstStart)
			{
				if (projectorOnFunction != NULL) 
				{
					(*projectorOnFunction)();
				}
				isProjectorOn = true;	
				isFirstStart = false;
			}
		}else
		{
			if (isProjectorOn || isFirstStart)
			{
				if (projectorOffFunction != NULL) 
				{
					(*projectorOffFunction)();
				}
				isProjectorOn = false;	
				isFirstStart = false;
			}
		}
	}
}

void ProjectorPower::init(void){
	acs.autoMidPoint();
	acs.setNoisemV(100) ;   
}
