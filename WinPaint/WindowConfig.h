#pragma once
class CWindowStatus
{
public:
	static int width;
	static int hight;
	static int widthClock;
	static int hightClock;
	static double scale;
	static POINT movingPoint;
	static bool isMoving;
	static bool isScaling;
	static bool isDrawing;
	CWindowStatus();
	static bool IsScalingPossible(int mouseDirection);
	static bool IsMovingPossible();
	~CWindowStatus();
};