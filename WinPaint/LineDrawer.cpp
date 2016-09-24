#include "stdafx.h"
#include "LineDrawer.h"


CLineDrawer::CLineDrawer()
{
}

void CLineDrawer::Draw(HDC hdc, int x, int y, int x1, int y1)
{
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x1, y1);	
}


CLineDrawer::~CLineDrawer()
{
}
