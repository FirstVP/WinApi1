#pragma once
#include "string"

#define TEXT_NO 0
#define TEXT_PLACED 1
#define TEXT_EDIT 2

class CTextManager
{
public:
	CTextManager();
	void Initialize(HDC hdc);
	char GetStatus();
	void SetStatus(char value);
	void CreateInput(HWND hWnd, HDC hdc, int x, int y);
	void UpdateCaret(HWND hWnd, HDC hdc);
	void InsertChar(wchar_t* ch);
	void DeleteChar();
	void DeleteInput();
	int GetBufferLength();
	void IncPosition();
	void DecPosition();
	void GetTextSize(HDC hdc, SIZE* size, int n);
	void OutText(HDC hdc);
	~CTextManager();
private:
	char status = 0;
	TEXTMETRIC tm;
	std::wstring buffer;
	int position = 0;
	int x = 0;
	int y = 0;
	int charHeight = 0;
};

