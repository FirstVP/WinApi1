#include "stdafx.h"
#include "WindowConfig.h"
#include "PaintConfig.h"
#include "LineDrawer.h"
#include "math.h"

CPrimitiveDrawer* CPaintConfig::drawer = new CLineDrawer();
int CPaintConfig::thickness = 1;
HPEN CPaintConfig::pen = CreatePen(PS_SOLID, thickness * CWindowConfig::scale, color);
HBRUSH CPaintConfig::brush = CreateSolidBrush(color);
COLORREF CPaintConfig::color = RGB(0, 0, 0);

CPaintConfig::CPaintConfig()
{
}

void CPaintConfig::ChooseThickness(int value)
{
	thickness = value;
	pen = CreatePen(PS_SOLID, thickness / CWindowConfig::scale, color);
} 

void CPaintConfig::DrawPrimitive(HDC hdc, int x, int y, int x1, int y1)
{
	drawer->Draw(hdc, x, y, x1, y1);
}

void CPaintConfig::ChoosePrimitiveDrawer(CPrimitiveDrawer* chosenDrawer)
{
	if (drawer != NULL)
		delete (drawer);
	drawer = chosenDrawer;	
}


CPaintConfig::~CPaintConfig()
{
}
