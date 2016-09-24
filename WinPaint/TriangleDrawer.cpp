#include "stdafx.h"
#include "WinPaint.h"
#include "LineDrawer.h"
#include "TriangleDrawer.h"


CTriangleDrawer::CTriangleDrawer()
{
}

void CTriangleDrawer::Draw(HDC hdc, int x, int y, int x1, int y1)
{	
	int side = abs(x1 - x);
	POINT vertexes[TRIANGLE_POINTS_COUNT] = { {x,y}, { x + (0.5*side), y - (0.87*side) }, { x + side, y } };
	Polygon(hdc, vertexes, TRIANGLE_POINTS_COUNT);
}


CTriangleDrawer::~CTriangleDrawer()
{
}
