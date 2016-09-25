#pragma once
class CMetafileManager
{
public:
	static HDC mdc;
	CMetafileManager();
	static void CreateMetafileContext(HWND hWnd, HDC hdc);
	~CMetafileManager();
};

