#pragma once
#include "PrimitiveDrawer.h"

class CPaintManager
{
public:
	static HPEN pen;
	static HBRUSH brush;
	static COLORREF color;
	static POINT checkedPoint;
	CPaintManager();
	static void ChooseBrushColor(COLORREF cc);
	static void ChooseFillColor(COLORREF cc);
	static void ChooseThickness(int value);
	static void DrawPrimitive(HDC hdc, int x, int y, int x1, int y1);
	static void ChoosePrimitiveDrawer(CPrimitiveDrawer* chosenDrawer);
	static void Fill(HDC hdc);
	~CPaintManager();
private:
	static CPrimitiveDrawer* drawer;
	static int thickness;
	static COLORREF colorFill;
};

