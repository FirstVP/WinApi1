#include "stdafx.h"
#include "WindowConfig.h"
#include "PaintConfig.h"
#include "LineDrawer.h"
#include "MetafileManager.h"
#include "math.h"

CPrimitiveDrawer* CPaintConfig::drawer = new CLineDrawer();
int CPaintConfig::thickness = 1;
HPEN CPaintConfig::pen = CreatePen(PS_SOLID, thickness * CWindowStatus::scale, color);
HBRUSH CPaintConfig::brush = CreateSolidBrush(color);
COLORREF CPaintConfig::color = RGB(0, 0, 0);
COLORREF CPaintConfig::colorFill = RGB(20, 50, 90);
POINT CPaintConfig::checkedPoint = { 0, 0 };

CPaintConfig::CPaintConfig()
{
}

void CPaintConfig::ChooseBrushColor(COLORREF cc)
{
	CPaintConfig::color = cc;
	CPaintConfig::pen = CreatePen(PS_SOLID, CPaintConfig::thickness * CWindowStatus::scale, CPaintConfig::color);
	CPaintConfig::brush = CreateSolidBrush(CPaintConfig::color);
}

void CPaintConfig::ChooseFillColor(COLORREF cc)
{
	colorFill = cc;
}

void CPaintConfig::ChooseThickness(int value)
{
	thickness = value;
	pen = CreatePen(PS_SOLID, thickness / CWindowStatus::scale, color);
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

void CPaintConfig::Fill(HDC hdc)
{
	HBRUSH fillBrush = CreateSolidBrush(colorFill);
	SelectObject(hdc, fillBrush);
	SelectObject(CMetafileManager::mdc, fillBrush);
	int correctedX = (CPaintConfig::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale;
	int correctedY = (CPaintConfig::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale;
	ExtFloodFill(hdc, correctedX, correctedY, GetPixel(hdc, correctedX, correctedY), FLOODFILLSURFACE);
	ExtFloodFill(CMetafileManager::mdc, correctedX, correctedY, GetPixel(CMetafileManager::mdc, correctedX, correctedY), FLOODFILLSURFACE);
}


CPaintConfig::~CPaintConfig()
{
}
