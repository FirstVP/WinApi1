#include "stdafx.h"
#include "WindowConfig.h"

int CWindowConfig::hight = 500;
int CWindowConfig::width = 1000;
double CWindowConfig::scale = 1;
POINT CWindowConfig::movingPoint;

CWindowConfig::CWindowConfig()
{
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
