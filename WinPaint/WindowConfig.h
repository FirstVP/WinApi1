#pragma once
class CWindowConfig
{
public:
	static int width;
	static int hight;
	static double scale;
	static POINT movingPoint;
	CWindowConfig();
	static bool IsScalingPossible(int mouseDirection);
	static bool IsMovingPossible();
	~CWindowConfig();
};