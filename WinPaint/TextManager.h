#pragma once
#include "string"
class CTextManager
{
public:
	static char status;
	static TEXTMETRIC tm;
	static std::wstring buffer;
	static int position;
	static int charWidth;
	static int charHeight;
	static int x;
	static int y;
	static int length;
	CTextManager();
	static void Initialize(HDC hdc);
	static void CreateInput(HWND hWnd, int x, int y, HDC hdc);
	static void UpdateCaret(HWND hWnd, HDC hdc);
	static void InsertChar();
	static void GetTextSize(HDC hdc, SIZE* size, int n);
	static void OutText(HDC hdc);
	~CTextManager();
};

