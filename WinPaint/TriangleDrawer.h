#pragma once
#include "PrimitiveDrawer.h"
class CTriangleDrawer :
	public CPrimitiveDrawer
{
public:
	CTriangleDrawer();
	void Draw(HDC hdc, int x, int y, int x1, int y1);
	~CTriangleDrawer();
};

