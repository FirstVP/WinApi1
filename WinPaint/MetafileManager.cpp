#include "stdafx.h"
#include "WinPaint.h"
#include "WindowConfig.h"
#include "MetafileManager.h"

HDC CMetafileManager::mdc;

CMetafileManager::CMetafileManager()
{
}

void CMetafileManager::CreateMetafileContext(HWND hWnd, HDC hdc, LPCWSTR tempFilePath)
{
	RECT rect;
	int iWidthMM = GetDeviceCaps(hdc, HORZSIZE);
	int iHeightMM = GetDeviceCaps(hdc, VERTSIZE);
	int iWidthPels = GetDeviceCaps(hdc, HORZRES);
	int iHeightPels = GetDeviceCaps(hdc, VERTRES);

	GetClientRect(hWnd, &rect); 

	rect.left = (rect.left * iWidthMM * 100) / iWidthPels;
	rect.top = (rect.top * iHeightMM * 100) / iHeightPels;
	rect.right = (rect.right * iWidthMM * 80) / iWidthPels;
	rect.bottom = (rect.bottom * iHeightMM * 80) / iHeightPels;

	mdc = CreateEnhMetaFile(hdc,
		tempFilePath,
		&rect, NULL);
}


CMetafileManager::~CMetafileManager()
{
}
