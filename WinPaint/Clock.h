#pragma once
class CClock
{
public:
	CClock();
	void SetIsotropic(HDC hdc, int cxClient, int cyClient);
	void RotatePoint(POINT pt[], int iNum, int iAngle);
	void DrawClock(HDC hdc);
	void DrawHands(HDC hdc, SYSTEMTIME * pst, BOOL fChange);
	~CClock();
};

