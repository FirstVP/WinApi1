#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Clock.h"


CClock::CClock()
{
}

void CClock::SetIsotropic(HDC hdc, int cxClient, int cyClient)
{
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 1000, 1000, NULL);
	SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);
	SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
}

void CClock::RotatePoint(POINT pt[], int iNum, int iAngle)
{
	int   i;
	POINT ptTemp;

	for (i = 0; i < iNum; i++)
	{
		ptTemp.x = (int)(pt[i].x * cos(2*M_PI * iAngle / 360) +
			pt[i].y * sin(2*M_PI * iAngle / 360));

		ptTemp.y = (int)(pt[i].y * cos(2*M_PI * iAngle / 360) -
			pt[i].x * sin(2*M_PI * iAngle / 360));

		pt[i] = ptTemp;
	}
}

void CClock::DrawClock(HDC hdc)
{
	int   iAngle;
	POINT pt[3];

	for (iAngle = 0; iAngle < 360; iAngle += 6)
	{
		pt[0].x = 0;
		pt[0].y = 900;

		RotatePoint(pt, 1, iAngle);

		pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;

		pt[0].x -= pt[2].x / 2;
		pt[0].y -= pt[2].y / 2;

		pt[1].x = pt[0].x + pt[2].x;
		pt[1].y = pt[0].y + pt[2].y;

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));

		Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}
}

void CClock::DrawHands(HDC hdc, SYSTEMTIME * pst, BOOL fChange)
{
	static POINT pt[3][5] = { 0, -150, 100, 0, 0, 600, -100, 0, 0, -150,
		0, -200,  50, 0, 0, 800,  -50, 0, 0, -200,
		0,    0,   0, 0, 0,   0,    0, 0, 0,  800 };
	int          i, iAngle[3];
	POINT        ptTemp[3][5];

	iAngle[0] = (pst->wHour * 30) % 360 + pst->wMinute / 2;
	iAngle[1] = pst->wMinute * 6;
	iAngle[2] = pst->wSecond * 6;

	memcpy(ptTemp, pt, sizeof(pt));

	for (i = fChange ? 0 : 2; i < 3; i++)
	{
		RotatePoint(ptTemp[i], 5, iAngle[i]);

		Polyline(hdc, ptTemp[i], 5);
	}
}


CClock::~CClock()
{
}