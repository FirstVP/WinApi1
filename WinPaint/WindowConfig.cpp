#include "stdafx.h"
#include "WindowConfig.h"

int CWindowStatus::hight = 500;
int CWindowStatus::width = 1000;
int CWindowStatus::hightClock = 200;
int CWindowStatus::widthClock = 200;
double CWindowStatus::scale = 1;
POINT CWindowStatus::movingPoint;
bool CWindowStatus::isMoving = false;
bool CWindowStatus::isScaling = false;
bool CWindowStatus::isDrawing = false;

CWindowStatus::CWindowStatus()
{
}

bool CWindowStatus::IsScalingPossible(int mouseDirection)
{
	bool result = true;
	if ((mouseDirection < 0) && ((movingPoint.x + movingPoint.y) != 0) && (CWindowStatus::scale <= 1))
		result = false;
	return result;
}



bool CWindowStatus::IsMovingPossible()
{
	bool result = true;
	if (scale < 1)
		result = false;
	return result;
}


CWindowStatus::~CWindowStatus()
{
}
