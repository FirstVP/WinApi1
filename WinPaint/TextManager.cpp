#include "stdafx.h"
#include "WindowConfig.h"
#include "MetafileManager.h"
#include "TextManager.h"

char CTextManager::status = 0;
TEXTMETRIC CTextManager::tm;
std::wstring CTextManager::buffer;
int CTextManager::position = 0;
int CTextManager::x = 0;
int CTextManager::y = 0;
int CTextManager::charHeight = 0;

CTextManager::CTextManager()
{
}

void CTextManager::Initialize(HDC hdc)
{
	GetTextMetrics(hdc, &tm);
	charHeight = tm.tmHeight;
}

void CTextManager::CreateInput(HWND hWnd, HDC hdc, int a, int b)
{
	buffer.clear();
	UpdateCaret(hWnd, hdc);
}

void CTextManager::UpdateCaret(HWND hWnd, HDC hdc)
{	
	SIZE size;
	GetTextSize(hdc, &size, position);
	DestroyCaret();
	CreateCaret(hWnd, (HBITMAP)1, 0, charHeight*CWindowConfig::scale);
	SetCaretPos( (x - CWindowConfig::movingPoint.x) * CWindowConfig::scale + size.cx, (y - CWindowConfig::movingPoint.y) * CWindowConfig::scale);
	ShowCaret(hWnd);	
}

void CTextManager::InsertChar(wchar_t* ch)
{
	buffer.insert(position++, ch);
}

void CTextManager::DeleteChar()
{
	if (position >= 1)
	{
		buffer.erase(position - 1, 1);
		position--;
	}	
}

void CTextManager::DeleteInput()
{
	OutText(CMetafileManager::mdc);
	status = 0;
	position = 0;
	x = 0;
	y = 0;
	DestroyCaret();
}

int CTextManager::GetBufferLength()
{
	return buffer.length();
}

void CTextManager::IncPosition()
{
	if (position < GetBufferLength())
		position++;
}

void CTextManager::DecPosition()
{
	if (position >= 1)
		position--;
}

void CTextManager::GetTextSize(HDC hdc, SIZE * size, int n)
{
	wchar_t* text = const_cast<wchar_t*>(buffer.c_str());
	GetTextExtentPoint32(hdc, text, n, size);
}

void CTextManager::OutText(HDC hdc)
{
	SIZE size;
	GetTextSize(hdc, &size, buffer.length());
	RECT rc;
	SetRect(&rc, (x - CWindowConfig::movingPoint.x)*CWindowConfig::scale, (y - CWindowConfig::movingPoint.y)*CWindowConfig::scale,
		(x - CWindowConfig::movingPoint.x)*CWindowConfig::scale + size.cx, (y - CWindowConfig::movingPoint.y)*CWindowConfig::scale+ size.cy);
	wchar_t* text = const_cast<wchar_t*>(buffer.c_str());
	DrawText(hdc, text, -1, &rc, DT_LEFT);
}



CTextManager::~CTextManager()
{
}
