#include "stdafx.h"
#include "RectangleDrawer.h"


CRectangleDrawer::CRectangleDrawer()
{
}

void CRectangleDrawer::Draw(HDC hdc, int x, int y, int x1, int y1)
{
	Rectangle(hdc, x, y, x1, y1);
}


CRectangleDrawer::~CRectangleDrawer()
{
}
