#include "stdafx.h"
#include "EllipseDrawer.h"


CEllipseDrawer::CEllipseDrawer()
{
}

void CEllipseDrawer::Draw(HDC hdc, int x, int y, int x1, int y1)
{
	Ellipse(hdc, x, y, x1, y1);
}


CEllipseDrawer::~CEllipseDrawer()
{
}
