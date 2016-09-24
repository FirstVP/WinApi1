#pragma once
#include "PrimitiveDrawer.h"
class CLineDrawer :
	public CPrimitiveDrawer
{
public:
	CLineDrawer();
	void Draw(HDC hdc, int x, int y, int x1, int y1);
	~CLineDrawer();
};

