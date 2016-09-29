#include "stdafx.h"
#include "WindowConfig.h"

int CWindowConfig::hight = 500;
int CWindowConfig::width = 1000;
int CWindowConfig::hightClock = 100;
int CWindowConfig::widthClock = 100;
double CWindowConfig::scale = 1;
POINT CWindowConfig::movingPoint;

CWindowConfig::CWindowConfig()
{
}

bool CWindowConfig::IsScalingPossible(int mouseDirection)
{
	bool result = true;
	if ((mouseDirection < 0) && ((movingPoint.x + movingPoint.y) != 0) && (CWindowConfig::scale <= 1))
		result = false;
	return result;
}



bool CWindowConfig::IsMovingPossible()
{
	bool result = true;
	if (scale < 1)
		result = false;
	return result;
}


CWindowConfig::~CWindowConfig()
{
}
