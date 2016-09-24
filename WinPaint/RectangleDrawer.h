#pragma once
#include "PrimitiveDrawer.h"
class CRectangleDrawer :
	public CPrimitiveDrawer
{
public:
	CRectangleDrawer();
	void Draw(HDC hdc, int x, int y, int x1, int y1);
	~CRectangleDrawer();
};

