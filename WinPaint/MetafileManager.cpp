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

	rect.left = (rect.left * iWidthMM * TRANSFORM_COEFFICIENT_1) / iWidthPels;
	rect.top = (rect.top * iHeightMM * TRANSFORM_COEFFICIENT_1) / iHeightPels;
	rect.right = (rect.right * iWidthMM * TRANSFORM_COEFFICIENT_2) / iWidthPels;
	rect.bottom = (rect.bottom * iHeightMM * TRANSFORM_COEFFICIENT_2) / iHeightPels;

	mdc = CreateEnhMetaFile(hdc,
		tempFilePath,
		&rect, NULL);
}

void CMetafileManager::SaveMetafile(HWND hWnd, LPCWSTR szFileName, LPCWSTR szTempFilePath)
{
	DeleteEnhMetaFile(CloseEnhMetaFile(mdc));
	HENHMETAFILE metafile = GetEnhMetaFile(szTempFilePath);
	CopyEnhMetaFile(metafile, szFileName);
	DeleteEnhMetaFile(metafile);
	DeleteObject(mdc);
	DeleteFile(szTempFilePath);

	CreateMetafileContext(hWnd, GetDC(hWnd), szTempFilePath);
	HENHMETAFILE restore = GetEnhMetaFile(szFileName);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom *= CWindowStatus::scale;
	rect.right *= CWindowStatus::scale;
	PlayEnhMetaFile(mdc, restore, &rect);
	DeleteEnhMetaFile(restore);
}


void CMetafileManager::OpenMetafile(HWND hWnd, HDC hdc, LPCWSTR szFileName, LPCWSTR szTempFilePath)
{
	DeleteEnhMetaFile(CloseEnhMetaFile(mdc));
	DeleteObject(mdc);
	DeleteFile(szTempFilePath);

	CreateMetafileContext(hWnd, GetDC(hWnd), TEMPORARY_METAFILE_PATH);
	HENHMETAFILE restore = GetEnhMetaFile(szFileName);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom *= CWindowStatus::scale;
	rect.right *= CWindowStatus::scale;
	CWindowStatus::movingPoint.x = 0;
	CWindowStatus::movingPoint.y = 0;
	SetViewportOrgEx(hdc, CWindowStatus::movingPoint.x, CWindowStatus::movingPoint.y, NULL);
	SetViewportOrgEx(mdc, CWindowStatus::movingPoint.x, CWindowStatus::movingPoint.y, NULL);

	PlayEnhMetaFile(mdc, restore, &rect);
	ClearDC(hdc, BORDER_WIDTH);
	PlayEnhMetaFile(hdc, restore, &rect);
	InvalidateRect(hWnd, 0, FALSE);
	UpdateWindow(hWnd);
	DeleteEnhMetaFile(restore);

}

void CMetafileManager::ClearDC(HDC hdc, int border)
{
	int countedBorder = border*CWindowStatus::scale;
	if (countedBorder < 1) countedBorder = 1;
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HPEN pen = CreatePen(PS_SOLID, border*CWindowStatus::scale, RGB(0, 0, 0));
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, CWindowStatus::width*CWindowStatus::scale, CWindowStatus::hight*CWindowStatus::scale);
}

CMetafileManager::~CMetafileManager()
{
}
