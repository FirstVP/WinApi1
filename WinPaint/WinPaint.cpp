// WinPaint.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "string"
#include "PaintConfig.h"
#include "WindowConfig.h"
#include "RectangleDrawer.h"
#include "LineDrawer.h"
#include "EllipseDrawer.h"
#include "TriangleDrawer.h"
#include "MetafileManager.h"
#include "WinPaint.h"


#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
WCHAR szTempFilePath[MAX_LOADSTRING];

HDC memory, secondMemory;
HBITMAP memoryBitmap, secondMemoryBitmap;
bool isDrawing;
char isPrinting;
char isPolygon;
bool isMoving;
bool isScaling;
bool isPencil;

int x = 0;
int y = 0;
int xPencil = 0;
int yPencil = 0;
int xPrint = 0;
int yPrint = 0;
int xPolygon = 0;
int yPolygon = 0;



// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���������� ��� �����.

    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINPAINT));

    MSG msg;

    // ���� ��������� ���������:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	//HBRUSH hb = ::CreateSolidBrush(RGB(222, 231, 249));
	//wcex.hbrBackground = hb;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 128, 256));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   RECT wr = { 0, 0, CWindowConfig::width, CWindowConfig::hight };

   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);  

   return TRUE;  
}

std::wstring GetExeFileName()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}

std::wstring GetExePath()
{
	std::wstring f = GetExeFileName();
	return f.substr(0, f.find_last_of(L"\\/"));
}

void InitializePath()
{
	//GetModuleFileName(NULL, szTempFilePath, MAX_LOADSTRING);
	std::wstring name(GetExePath());
	wchar_t* path = const_cast<wchar_t*>(name.c_str());
	MessageBox(NULL, szTempFilePath, NULL, MB_OK);
	//wcscat(szTempFilePath, L"\\Temp\\Temp.emf");
}

void InitializeWindow(HWND hWnd)
{
	isDrawing = false;
	isPrinting = 0;
	isPolygon = 0;
	isMoving = false;
	isScaling = false;
	isPencil = false;
	CWindowConfig::movingPoint.x = 0;
	CWindowConfig::movingPoint.y = 0;
}

int Print(int x, int y, int a, int b)
{
	
	PRINTDLG prDlg;
	memset(&prDlg, 0, sizeof(prDlg));
	prDlg.lStructSize = sizeof(prDlg);

	prDlg.Flags = PD_RETURNDC;

	if (!PrintDlg(&prDlg))
	{
		MessageBox(NULL, L"Can�elled", L"Print", MB_OK | MB_ICONERROR);
		return -1;
	}

	DOCINFO  documentInformation;
	HDC hPrinter = prDlg.hDC;

	memset(&documentInformation, 0, sizeof(documentInformation));
	documentInformation.cbSize = sizeof(documentInformation);
	StartDoc(hPrinter, &documentInformation);

	StartPage(hPrinter);

	StretchBlt(hPrinter, 30, 30, a * 2, b * 2, secondMemory, x, y, abs(a-x), abs(b-y), SRCCOPY);

	EndPage(hPrinter);

	EndDoc(hPrinter);
	DeleteDC(hPrinter);
	isPrinting = 0;
	return 0;
}

int CreateNewLayer(HWND hWnd, HDC* newDC, HBITMAP* newBmp, int width, int hight)
{
	HDC hdc = GetDC(hWnd);
	*newDC = CreateCompatibleDC(hdc);
	*newBmp = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(*newDC, *newBmp);
	HPEN printPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0)); // change to wh-0
	SelectObject(*newDC, printPen);
	Rectangle(*newDC, -1, -1, width + 1, hight + 1);
	ReleaseDC(hWnd, hdc);
	return 0;
}

void OpenColorDialog(HWND hWnd)
{
	CHOOSECOLOR cc;                 
	static COLORREF acrCustClr[16];                      
	HBRUSH hbrush;                
	static DWORD rgbCurrent;       
									
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		CPaintConfig::color = cc.rgbResult;
		CPaintConfig::pen = CreatePen(PS_SOLID, CPaintConfig::thickness * CWindowConfig::scale, CPaintConfig::color);
		CPaintConfig::brush = CreateSolidBrush(CPaintConfig::color);
	}
}

int GetIntSign(int number)
{
	int result = 1;
	if (number < 0) result = -1;
	return result;
}

void ClearDC(HDC hdc, int border)
{
	//border *= 0;
	int countedBorder = border*CWindowConfig::scale;
	if (countedBorder < 1) countedBorder = 1;
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HPEN pen = CreatePen(PS_SOLID, border*CWindowConfig::scale, RGB(0, 0, 0));
	SelectObject(hdc, pen);	
	SelectObject(hdc, brush);
	Rectangle(hdc, 0, 0, CWindowConfig::width*CWindowConfig::scale, CWindowConfig::hight*CWindowConfig::scale);
}

void SaveMetafile(HWND hWnd, LPCWSTR szFileName)
{	
	DeleteEnhMetaFile(CloseEnhMetaFile(CMetafileManager::mdc));
	HENHMETAFILE metafile = GetEnhMetaFile(TEMPORARY_METAFILE_PATH);
	CopyEnhMetaFile(metafile, szFileName);
	DeleteEnhMetaFile(metafile);
	DeleteObject(CMetafileManager::mdc);
	DeleteFile(TEMPORARY_METAFILE_PATH);

	CMetafileManager::CreateMetafileContext(hWnd, GetDC(hWnd), TEMPORARY_METAFILE_PATH);
	HENHMETAFILE restore = GetEnhMetaFile(szFileName);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom *= CWindowConfig::scale;
	rect.right *= CWindowConfig::scale;
	PlayEnhMetaFile(CMetafileManager::mdc, restore, &rect);
	DeleteEnhMetaFile(restore);
}

void ShowSaveDialog(HWND hWnd)
{
	OPENFILENAME ofn;
	WCHAR szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Enhanced metafile (*.emf)\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"emf";

	if (GetSaveFileName(&ofn))
	{
		SaveMetafile(hWnd, szFileName);
	}
	else
	{
		MessageBox(hWnd, L"Cancelled", L"Save file", MB_OK);
	}
}

void OpenMetafile(HWND hWnd, LPCWSTR szFileName)
{
	DeleteEnhMetaFile(CloseEnhMetaFile(CMetafileManager::mdc));
	DeleteObject(CMetafileManager::mdc);
	DeleteFile(TEMPORARY_METAFILE_PATH);

	CMetafileManager::CreateMetafileContext(hWnd, GetDC(hWnd), TEMPORARY_METAFILE_PATH);
	HENHMETAFILE restore = GetEnhMetaFile(szFileName);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.bottom *= CWindowConfig::scale;
	rect.right *= CWindowConfig::scale;
	CWindowConfig::movingPoint.x = 0;
	CWindowConfig::movingPoint.y = 0;
	SetViewportOrgEx(memory, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
	SetViewportOrgEx(CMetafileManager::mdc, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
	
	PlayEnhMetaFile(CMetafileManager::mdc, restore, &rect);
	ClearDC(memory, 5);
	PlayEnhMetaFile(memory, restore, &rect);
	InvalidateRect(hWnd, 0, FALSE);
	UpdateWindow(hWnd);
	DeleteEnhMetaFile(restore);
}

void ShowOpenDialog(HWND hWnd)
{
	OPENFILENAME ofn;
	WCHAR szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Enhanced metafile (*.emf)\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"emf";

	if (GetOpenFileName(&ofn))
	{
		OpenMetafile(hWnd, szFileName);
	}
	else
	{
		MessageBox(hWnd, L"Cancelled", L"Open file", MB_OK);
	}
}

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_ERASEBKGND:
		{		
		SetBkColor((HDC)wParam, 0x000000ff); 
		break;
		}
	case WM_CREATE:
	    {
			InitializeWindow(hWnd);
			InitializePath();
			CreateNewLayer(hWnd, &memory, &memoryBitmap, CWindowConfig::width, CWindowConfig::hight);
			CreateNewLayer(hWnd, &secondMemory, &secondMemoryBitmap, CWindowConfig::width, CWindowConfig::hight);
			CMetafileManager::CreateMetafileContext(hWnd, GetDC(hWnd), TEMPORARY_METAFILE_PATH);
			CPaintConfig::ChooseThickness(THIN_LINE);
		}
		break;
	case WM_CHAR:
	{
		static int xText; //textMode
		static int yText;
		xText = x;
		yText = y;
		static int charWidth = 3;
		TCHAR ch = (TCHAR)wParam;
		GetCharWidth32(memory, (UINT)wParam, (UINT)wParam,
			&charWidth);
		TextOut(memory, xText, yText,
			&ch, 1);
		xText += charWidth+1;
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);
	}
	break;
	case WM_LBUTTONDBLCLK :
		{	
		wchar_t buffer[256];
		//wsprintfW(buffer, L"%d", 3);
		//if (dirExists("C:\\Metafile"))
		GetModuleFileName(NULL, szTempFilePath, MAX_LOADSTRING);
		//CloseEnhMetaFile(CMetafileManager::mdc);
		wcscat(szTempFilePath, L"\\Temp\\Temp.emf");
		MessageBoxW(nullptr, szTempFilePath, NULL, MB_OK);
		}
		break;
	case WM_RBUTTONDBLCLK:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		rect.bottom *= CWindowConfig::scale;
		rect.right *= CWindowConfig::scale;
		HENHMETAFILE save = GetEnhMetaFile(L"D:\\Metafile\\wh.emf"); 
		PlayEnhMetaFile(memory, save, &rect);
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId) {
			case IDM_SAVEAS:
			{
				ShowSaveDialog(hWnd);
				break;
			}         
			case IDM_OPEN:
			{
				ShowOpenDialog(hWnd);
				break;
			}
			case ID_TOOLS_POLYGON:
			{
				isPrinting = 0;
				isPolygon = 1;
				isPencil = false;
				break;
			}
			case ID_TOOLS_PENCIL:
			{
				isPrinting = 0;
				isPolygon = 0;
				isPencil = true;
				break;
			}			
			case ID_TOOLS_LINE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CLineDrawer());
				isPrinting = 0;
				isPolygon = 0;
				isPencil = false;
				break;
			}							
			case ID_TOOLS_TRIANGLE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CTriangleDrawer());
				isPrinting = 0;
				isPolygon = 0;
				isPencil = false;
				break;
			}								
			case ID_TOOLS_ELLIPSE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CEllipseDrawer());
				isPrinting = 0;
				isPolygon = 0;
				isPencil = false;
				break;
			}						
			case ID_TOOLS_RECTANGLE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CRectangleDrawer());
				isPrinting = 0;
				isPolygon = 0;
				isPencil = false;
				break;
			}								
			case ID_LINESETTINGS_THIN:
				CPaintConfig::ChooseThickness(THIN_LINE);
				break;
			case ID_LINESETTINGS_NORMAL:
				CPaintConfig::ChooseThickness(NORMAL_LINE);
				break;
			case ID_LINESETTINGS_THICK:
				CPaintConfig::ChooseThickness(THICK_LINE);
				break;
			case ID_COLOR_CHOOSECOLOR:
				OpenColorDialog(hWnd);
				break;
			case IDM_PRINT:
				isPencil = false;
				isPrinting = 1;			
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_MOUSEMOVE:
	{	
		if (2 == isPrinting)
		{
			isDrawing = false;
			HPEN printPen = CreatePen(PS_DASH, 1, RGB (90, 10, 10));
			SelectObject(memory, GetStockObject(HOLLOW_BRUSH));
			SelectObject(memory, printPen);
			BitBlt(memory, 0, 0, CWindowConfig::width, CWindowConfig::hight, secondMemory, 0, 0, SRCCOPY);
			int a = 0; 
			int b = 0; 
			a = LOWORD(lParam);
			b = HIWORD(lParam);
			if ((a > xPrint) && (b > yPrint))
			Rectangle(memory, xPrint, yPrint, a, b);
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}	
		if (2 == isPolygon)
		{
			isDrawing = false;
			BitBlt(memory, 0, 0, CWindowConfig::width, CWindowConfig::hight, secondMemory, 0, 0, SRCCOPY);
			int a = 0;
			int b = 0;
			a = LOWORD(lParam);
			b = HIWORD(lParam);
			CLineDrawer lineDrawer;
			lineDrawer.Draw(memory, xPolygon, yPolygon, a, b);
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}
		if ((isPencil) && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
		{
			isDrawing = false;
			int a = LOWORD(lParam);
			int b = HIWORD(lParam);
			CLineDrawer lineDrawer;
			HPEN pen = CreatePen(PS_SOLID, 5, CPaintConfig::color);
			SelectObject(memory, pen);
			SelectObject(CMetafileManager::mdc, pen);
			if (!((xPencil == 0) && (yPencil == 0)))
			{
				lineDrawer.Draw(memory, xPencil, yPencil, a, b);
				lineDrawer.Draw(CMetafileManager::mdc, xPencil, yPencil, a, b);
			}				
			else
			{
				lineDrawer.Draw(memory, a, b, a + 1, b + 1);
				lineDrawer.Draw(CMetafileManager::mdc, a, b, a + 1, b + 1);
			}			
			xPencil = a;
			yPencil = b;
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}
		else
		if (true == isDrawing)
		{
			BitBlt(memory, 0, 0, CWindowConfig::width*CWindowConfig::scale, CWindowConfig::hight*CWindowConfig::scale, secondMemory, 0, 0, SRCCOPY);
			int a = 0;
			int b = 0;
			a = LOWORD(lParam);
			b = HIWORD(lParam);
			CPaintConfig::DrawPrimitive(memory, x, y, a, b);
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}
		
	}
	break;
	case WM_LBUTTONDOWN:
	{	
		if (isPrinting != 2) BitBlt(secondMemory, 0, 0, CWindowConfig::width, CWindowConfig::hight, memory, 0, 0, SRCCOPY);
		SelectObject(memory, CPaintConfig::pen);
		SelectObject(memory, CPaintConfig::brush);
		SelectObject(CMetafileManager::mdc, CPaintConfig::pen);
		SelectObject(CMetafileManager::mdc, CPaintConfig::brush);
		x = LOWORD(lParam);
		y = HIWORD(lParam);		
		if (isPrinting > 0)
		{
			if (isPrinting == 1)
			{
				isPrinting = 2;
				xPrint = x;
				yPrint = y;
			}				
		}

		if (isPolygon > 0)
		{
			if (isPolygon == 2)
			{
				CLineDrawer lineDrawer;
				lineDrawer.Draw(CMetafileManager::mdc, xPolygon, yPolygon, x, y);
			}
			else
			if (isPolygon == 1)
			{
				isPolygon = 2;
			}			
            xPolygon = x;
			yPolygon = y;
		}

		if (isPrinting + isPolygon > 0)
			isDrawing = false;
		else
			isDrawing = true;
				
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (isPolygon == 2)
		{
			BitBlt(memory, 0, 0, CWindowConfig::width, CWindowConfig::hight, secondMemory, 0, 0, SRCCOPY);
			isPolygon = 0;
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}
	}
	case WM_LBUTTONUP:
	{
		int a = LOWORD(lParam);
		int b = HIWORD(lParam);
		if (isPencil > 0)
		{
			xPencil = 0;
			yPencil = 0;
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}
		if (isPolygon)
		{

		}
		if (isPrinting == 2)
		{					
			isPrinting = 3;
			BitBlt(memory, 0, 0, CWindowConfig::width, CWindowConfig::hight, secondMemory, 0, 0, SRCCOPY);			
			Print(xPrint, yPrint, a, b);
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);			
		}			
		else
		if (isDrawing)
		{
			isDrawing = false;
			CPaintConfig::DrawPrimitive(memory, x, y, a, b);
			CPaintConfig::DrawPrimitive(CMetafileManager::mdc, x, y, a, b);		
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}				
	}
	break;
	case WM_MOUSEWHEEL:
	{		
		int mouseDirection = GetIntSign(GET_WHEEL_DELTA_WPARAM(wParam));
		if (wParam & MK_CONTROL)
		{	
			if (CWindowConfig::IsScalingPossible(mouseDirection))
			{
				CWindowConfig::scale += 0.1*mouseDirection;
				isScaling = true;
			}			
		}
		else if (CWindowConfig::IsMovingPossible())
		{		
			if (wParam & MK_SHIFT)
			{
				CWindowConfig::movingPoint.x += 10 * mouseDirection;			
			}
			else
			{
				CWindowConfig::movingPoint.y += 10 * mouseDirection;				
			}
			isMoving = true;
		}
			
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);		
	}
	break;
    case WM_PAINT:
        {	
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);				

			if (isScaling)
			{				
				SetMapMode(memory, MM_ANISOTROPIC);
				SetMapMode(secondMemory, MM_ANISOTROPIC);
				SetMapMode(CMetafileManager::mdc, MM_ANISOTROPIC);
				SetViewportExtEx(memory, CWindowConfig::width, CWindowConfig::hight, NULL);
				SetWindowExtEx(memory, CWindowConfig::width * CWindowConfig::scale, CWindowConfig::hight * CWindowConfig::scale, NULL);
				SetViewportExtEx(secondMemory, CWindowConfig::width, CWindowConfig::hight, NULL);
				SetWindowExtEx(secondMemory, CWindowConfig::width * CWindowConfig::scale, CWindowConfig::hight * CWindowConfig::scale, NULL);
				SetViewportExtEx(CMetafileManager::mdc, CWindowConfig::width, CWindowConfig::hight, NULL);
				SetWindowExtEx(CMetafileManager::mdc, CWindowConfig::width * CWindowConfig::scale, CWindowConfig::hight * CWindowConfig::scale, NULL);
				HPEN printPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				SelectObject(hdc, printPen);
				Rectangle(hdc, -1, -1, CWindowConfig::width + 1, CWindowConfig::hight + 1);
				isScaling = false;			
			}	
			if (isMoving)
			{	
				SetViewportOrgEx(memory, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
				SetViewportOrgEx(secondMemory, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
				SetViewportOrgEx(CMetafileManager::mdc, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
				HPEN printPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				SelectObject(hdc, printPen);
				Rectangle(hdc, -1, -1, CWindowConfig::width + 1, CWindowConfig::hight + 1);
				isMoving = false;
			}

			BitBlt(hdc, 0, 0, CWindowConfig::width, CWindowConfig::hight, memory, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

