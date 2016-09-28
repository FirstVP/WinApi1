#pragma once
#include "string"
class CTextManager
{
public:
	static char status;
	CTextManager();
	static void Initialize(HDC hdc);
	static void CreateInput(HWND hWnd, HDC hdc, int x, int y);
	static void UpdateCaret(HWND hWnd, HDC hdc);
	static void InsertChar(wchar_t* ch);
	static void DeleteChar();
	static void DeleteInput();
	static int GetBufferLength();
	static void IncPosition();
	static void DecPosition();
	static void GetTextSize(HDC hdc, SIZE* size, int n);
	static void OutText(HDC hdc);
	~CTextManager();
private:
	static TEXTMETRIC tm;
	static std::wstring buffer;
	static int position;
	static int x;
	static int y;
	static int charHeight;
};

