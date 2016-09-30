#include "stdafx.h"
#include "WindowConfig.h"
#include "PaintConfig.h"
#include "LineDrawer.h"
#include "MetafileManager.h"
#include "math.h"

CPrimitiveDrawer* CPaintManager::drawer = new CLineDrawer();
int CPaintManager::thickness = 1;
HPEN CPaintManager::pen = CreatePen(PS_SOLID, thickness * CWindowStatus::scale, color);
HBRUSH CPaintManager::brush = CreateSolidBrush(color);
COLORREF CPaintManager::color = RGB(0, 0, 0);
COLORREF CPaintManager::colorFill = RGB(20, 50, 90);
POINT CPaintManager::checkedPoint = { 0, 0 };

CPaintManager::CPaintManager()
{
}

void CPaintManager::ChooseBrushColor(COLORREF cc)
{
	CPaintManager::color = cc;
	CPaintManager::pen = CreatePen(PS_SOLID, CPaintManager::thickness * CWindowStatus::scale, CPaintManager::color);
	CPaintManager::brush = CreateSolidBrush(CPaintManager::color);
}

void CPaintManager::ChooseFillColor(COLORREF cc)
{
	colorFill = cc;
}

void CPaintManager::ChooseThickness(int value)
{
	thickness = value;
	pen = CreatePen(PS_SOLID, thickness / CWindowStatus::scale, color);
} 

void CPaintManager::DrawPrimitive(HDC hdc, int x, int y, int x1, int y1)
{
	drawer->Draw(hdc, x, y, x1, y1);
}

void CPaintManager::ChoosePrimitiveDrawer(CPrimitiveDrawer* chosenDrawer)
{
	if (drawer != NULL)
		delete (drawer);
	drawer = chosenDrawer;	
}

void CPaintManager::Fill(HDC hdc)
{
	HBRUSH fillBrush = CreateSolidBrush(colorFill);
	SelectObject(hdc, fillBrush);
	int correctedX = (CPaintManager::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale;
	int correctedY = (CPaintManager::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale;
	ExtFloodFill(hdc, correctedX, correctedY, GetPixel(hdc, correctedX, correctedY), FLOODFILLSURFACE);
}


CPaintManager::~CPaintManager()
{
}
