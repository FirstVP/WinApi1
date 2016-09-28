#include "stdafx.h"
#include "WindowConfig.h"
#include "TextManager.h"

char CTextManager::status = 0;
TEXTMETRIC CTextManager::tm;
std::wstring CTextManager::buffer;
int CTextManager::position = 0;
int CTextManager::charWidth = 5;
int CTextManager::charHeight = 10;
int CTextManager::x = 0;
int CTextManager::y = 0;
int CTextManager::length = 0;

CTextManager::CTextManager()
{
}

void CTextManager::Initialize(HDC hdc)
{
	GetTextMetrics(hdc, &tm);
	charWidth = tm.tmAveCharWidth;
	charHeight = tm.tmHeight;
}

void CTextManager::CreateInput(HWND hWnd, int a, int b, HDC hdc)
{
	x = a / CWindowConfig::scale;
	y = b / CWindowConfig::scale;
	x += CWindowConfig::movingPoint.x;
	y += CWindowConfig::movingPoint.y;
	UpdateCaret(hWnd, hdc);
}

void CTextManager::UpdateCaret(HWND hWnd, HDC hdc)
{	
	wchar_t* text = const_cast<wchar_t*>(buffer.c_str());
	int len = buffer.length();
	SIZE size;
	GetTextExtentPoint32(hdc, text, len, &size);
	int w = size.cx;
	int h = size.cy;

	DestroyCaret();
	CreateCaret(hWnd, (HBITMAP)1, 0, charHeight*CWindowConfig::scale);
	SetCaretPos( (x - CWindowConfig::movingPoint.x) * CWindowConfig::scale + w, (y - CWindowConfig::movingPoint.y) * CWindowConfig::scale);
	ShowCaret(hWnd);	
}

void CTextManager::GetTextSize(HDC hdc, SIZE * size, int n)
{
	wchar_t* text = const_cast<wchar_t*>(buffer.c_str());
	GetTextExtentPoint32(hdc, text, n, size);
}

void CTextManager::OutText(HDC hdc)
{
	wchar_t* text = const_cast<wchar_t*>(buffer.c_str());
	int len = buffer.length();
	SIZE size;
	GetTextSize(hdc, &size, len);
	int width = size.cx;
	int height = size.cy;

	//wchar_t buffer[256];
	//wsprintf(buffer, L"%d", w);
	//MessageBox(NULL, buffer, NULL, MB_OKCANCEL);

	RECT rc;
	SetRect(&rc, (x - CWindowConfig::movingPoint.x)*CWindowConfig::scale, (y - CWindowConfig::movingPoint.y)*CWindowConfig::scale,
		(x - CWindowConfig::movingPoint.x)*CWindowConfig::scale + w, (charHeight+y - CWindowConfig::movingPoint.y)*CWindowConfig::scale);
	
	DrawText(hdc, text, -1, &rc, DT_LEFT);
	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
}



CTextManager::~CTextManager()
{
}
