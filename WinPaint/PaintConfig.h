#pragma once
#include "PrimitiveDrawer.h"

class CPaintConfig
{
public:
	static CPrimitiveDrawer* drawer;
	static int thickness;
	static HPEN pen;
	static HBRUSH brush;
	static COLORREF color;
	CPaintConfig();
	static void ChooseThickness(int value);
	static void DrawPrimitive(HDC hdc, int x, int y, int x1, int y1);
	static void ChoosePrimitiveDrawer(CPrimitiveDrawer* chosenDrawer);
	~CPaintConfig();
};

