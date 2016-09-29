#include "stdafx.h"
#include "WindowConfig.h"
#include "MetafileManager.h"
#include "TextManager.h"

CTextManager::CTextManager()
{
}

void CTextManager::Initialize(HDC hdc)
{
	GetTextMetrics(hdc, &tm);
	charHeight = tm.tmHeight;
}

char CTextManager::GetStatus()
{
	return status;
}

void CTextManager::SetStatus(char value)
{
	status = value;
}

void CTextManager::CreateInput(HWND hWnd, HDC hdc, int a, int b)
{
	buffer.clear();
	x = a;
	y = b;
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
	buffer.clear();
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

	HFONT hFont = CreateFont(charHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
	SelectObject(hdc, hFont);

	DrawText(hdc, text, -1, &rc, DT_NOCLIP);
}



CTextManager::~CTextManager()
{
}
