#pragma once

#define BORDER_WIDTH 5
#define TRANSFORM_COEFFICIENT_1 100
#define TRANSFORM_COEFFICIENT_2 80

class CMetafileManager
{
public:
	static HDC mdc;
	CMetafileManager();
	void CreateMetafileContext(HWND hWnd, HDC hdc, LPCWSTR tempFilePath);
	void SaveMetafile(HWND hWnd, LPCWSTR szFileName, LPCWSTR szTempFilePath);
	void OpenMetafile(HWND hWnd, HDC hdc, LPCWSTR szFileName, LPCWSTR szTempFilePath);
	void ClearDC(HDC hdc, int border);
	~CMetafileManager();
};

