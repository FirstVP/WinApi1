#pragma once
#include "PrimitiveDrawer.h"
class CEllipseDrawer :
	public CPrimitiveDrawer
{
public:
	CEllipseDrawer();
	void Draw(HDC hdc, int x, int y, int x1, int y1);
	~CEllipseDrawer();
};

