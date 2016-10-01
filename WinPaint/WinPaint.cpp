#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "string"
#include "PaintConfig.h"
#include "WindowConfig.h"
#include "SaveLoadManager.h"
#include "RectangleDrawer.h"
#include "LineDrawer.h"
#include "EllipseDrawer.h"
#include "TriangleDrawer.h"
#include "TextManager.h"
#include "Clock.h"
#include "PolygonDrawer.h"
#include "PencilDrawer.h"
#include "MetafileManager.h"
#include "PrintRectangleDrawer.h"
#include "WinPaint.h"
#include "MainWindowController.h"


HINSTANCE hInst;                                
HWND mainWindow;
HWND clockWindow;
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];          
WCHAR szTitleClock[MAX_LOADSTRING];
WCHAR szWindowClassClock[MAX_LOADSTRING];
WCHAR szTempFilePath[MAX_LOADSTRING];

HDC memory, secondMemory;

CTextManager* textManager = new CTextManager();
CMetafileManager* metafileManager = new CMetafileManager();
CClock* clock = new CClock();
CPolygonDrawer* polygonDrawer = new CPolygonDrawer();
CPrintRectangleDrawer* printDrawer = new CPrintRectangleDrawer();
CPencilDrawer* pencilDrawer = new CPencilDrawer();

ATOM                RegisterClass(HINSTANCE hInstance);
ATOM                RegisterClassClock(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcClock(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINPAINT, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_CLOCK_TITLE, szTitleClock, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLOCK, szWindowClassClock, MAX_LOADSTRING);

    RegisterClass(hInstance);
	RegisterClassClock(hInstance);
  
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINPAINT));

    MSG msg;

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




ATOM RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style =  CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM RegisterClassClock(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcClock;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClassClock;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   RECT wr = { 0, 0, CWindowStatus::width, CWindowStatus::hight };

   AdjustWindowRect(&wr, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
	   WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
      0, 0, wr.right - wr.left + 5, wr.bottom - wr.top + 30, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   mainWindow = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);  

   HWND hWndClock = CreateWindowW(szWindowClassClock, szTitleClock,
	   WS_OVERLAPPED,
	   CW_USEDEFAULT, 0, CWindowStatus::widthClock, CWindowStatus::hightClock, nullptr, nullptr, hInstance, nullptr);

   if (!hWndClock)
   {
	   return FALSE;
   }
   clockWindow = hWndClock;
   ShowWindow(hWndClock, nCmdShow);
   UpdateWindow(hWndClock);  


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
	std::wstring name(GetExePath());
	wchar_t* path = const_cast<wchar_t*>(name.c_str());
	wcscpy(szTempFilePath, path);
	wcscat(szTempFilePath, L"\\Temp");
	CreateDirectory(szTempFilePath, NULL);
	wcscat(szTempFilePath, L"\\Temp.emf");	
}

void InitializeWindow(HWND hWnd)
{
	CWindowStatus::isDrawing = false;
	CPrintRectangleDrawer::status = PRINT_NO;
	CPolygonDrawer::status = POLYGON_NO;
	CWindowStatus::isMoving = false;
	CWindowStatus::isScaling = false;
	CPencilDrawer::status = false;
	CWindowStatus::movingPoint.x = 0;
	CWindowStatus::movingPoint.y = 0;
}

int ShowPrintDialog(int x, int y, int a, int b)
{
	PRINTDLG prDlg;
	memset(&prDlg, 0, sizeof(prDlg));
	prDlg.lStructSize = sizeof(prDlg);
	prDlg.Flags = PD_RETURNDC;
	if (!PrintDlg(&prDlg))
	{
		CPrintRectangleDrawer::status = PRINT_NO;
		MessageBox(NULL, L"Canñelled", L"Print", MB_OK | MB_ICONERROR);
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
	return 0;
}



void OpenColorDialog(HWND hWnd, bool isBrush)
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
		if (isBrush)
			CPaintManager::ChooseBrushColor(cc.rgbResult);
		else
			CPaintManager::ChooseFillColor(cc.rgbResult);
	}
}

int GetIntSign(int number)
{
	int result = 1;
	if (number < 0) result = -1;
	return result;
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
		metafileManager->SaveMetafile(hWnd, szFileName, TEMPORARY_METAFILE_PATH);
	}
	else
	{
		MessageBox(hWnd, L"Cancelled", L"Save file", MB_OK);
	}
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
		metafileManager->OpenMetafile(hWnd, memory, szFileName, TEMPORARY_METAFILE_PATH);
	}
	else
	{
		MessageBox(hWnd, L"Cancelled", L"Open file", MB_OK);
	}
}

int CreateNewLayer(HWND hWnd, HDC* newDC, HBITMAP* newBmp, int width, int hight)
{
	HDC hdc = GetDC(hWnd);
	*newDC = CreateCompatibleDC(hdc);
	*newBmp = CreateCompatibleBitmap(hdc, width, hight);
	SelectObject(*newDC, *newBmp);
	HPEN printPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	SelectObject(*newDC, printPen);
	Rectangle(*newDC, -1, -1, width + 1, hight + 1);
	ReleaseDC(hWnd, hdc);
	return 0;
}

void InitializeTextConfiguration(HDC hdc, CTextManager* textManager)
{
	textManager->Initialize(hdc);
	textManager->Initialize(CMetafileManager::mdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	    {
		//CMainWindowController::OnCreate(hWnd, memory, secondMemory, szTempFilePath, metafileManager, textManager);
		HBITMAP memoryBitmap, secondMemoryBitmap;
	InitializeWindow(hWnd);
	InitializePath();
	CreateNewLayer(hWnd, &memory, &memoryBitmap, CWindowStatus::width, CWindowStatus::hight);
	CreateNewLayer(hWnd, &secondMemory, &secondMemoryBitmap, CWindowStatus::width, CWindowStatus::hight);
	CPaintManager::ChooseThickness(THIN_LINE);
	InitializeTextConfiguration(memory, textManager);
		}
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			if (textManager->GetStatus() == TEXT_EDIT)
			{				
				textManager->DeleteInput();
			}
			break;
		}
		case VK_BACK:
		{
			textManager->DeleteChar();
			break;
		}
		case VK_RIGHT:
		{
			textManager->IncPosition();
			break;
		}
		case VK_LEFT:
		{
			textManager->DecPosition();
			break;
		}
		default:
			return 0;
		}
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);
	}
	case WM_CHAR:
	{
		if (textManager->GetStatus() == TEXT_EDIT)
		{
			wchar_t ch;
			switch (wParam)
			{	
			case VK_ESCAPE:
			case VK_BACK:
			case VK_RIGHT:
			case VK_LEFT:
			{
				break;
			}
			default:
				ch = (wchar_t)wParam;
				textManager->InsertChar(&ch);		
				InvalidateRect(hWnd, 0, FALSE);
				UpdateWindow(hWnd);
			}		
		}	
	}
	break;
	case WM_LBUTTONDBLCLK :
		{		
		}
		break;
	case WM_RBUTTONDBLCLK:
	{
		CPaintManager::checkedPoint.x = LOWORD(lParam);
		CPaintManager::checkedPoint.y = HIWORD(lParam);
		CPaintManager::checkedPoint.x /= CWindowStatus::scale;
		CPaintManager::checkedPoint.y /= CWindowStatus::scale;
		CPaintManager::checkedPoint.x += CWindowStatus::movingPoint.x;
		CPaintManager::checkedPoint.y += CWindowStatus::movingPoint.y;
		CPaintManager::Fill(memory);
		CPaintManager::Fill(CMetafileManager::mdc);
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
				BringWindowToTop(clockWindow);
				break;
			}         
			case IDM_OPEN:
			{
				ShowOpenDialog(hWnd);
				BringWindowToTop(clockWindow);
				break;
			}
			case ID_TOOLS_POLYGON:
			{
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_BEGIN;
				CPencilDrawer::status = false;
				break;
			}
			case ID_TOOLS_PENCIL:
			{
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = true;
				break;
			}			
			case ID_TOOLS_LINE:
			{
				CPaintManager::ChoosePrimitiveDrawer(new CLineDrawer());
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = false;
				break;
			}							
			case ID_TOOLS_TRIANGLE:
			{
				CPaintManager::ChoosePrimitiveDrawer(new CTriangleDrawer());
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = false;
				break;
			}								
			case ID_TOOLS_ELLIPSE:
			{
				CPaintManager::ChoosePrimitiveDrawer(new CEllipseDrawer());
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = false;
				break;
			}						
			case ID_TOOLS_RECTANGLE:
			{
				CPaintManager::ChoosePrimitiveDrawer(new CRectangleDrawer());
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = false;
				break;
			}					
			case ID_TOOLS_TEXT:
			{
				textManager->DeleteInput();
				textManager->SetStatus(TEXT_PLACED);
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPencilDrawer::status = false;
			}
			case ID_LINESETTINGS_THIN:
				CPaintManager::ChooseThickness(THIN_LINE);
				break;
			case ID_LINESETTINGS_NORMAL:
				CPaintManager::ChooseThickness(NORMAL_LINE);
				break;
			case ID_LINESETTINGS_THICK:
				CPaintManager::ChooseThickness(THICK_LINE);
				break;
			case ID_COLOR_CHOOSECOLOR:
				OpenColorDialog(hWnd, true);
				BringWindowToTop(clockWindow);
				break;
			case ID_COLOR_CHOOSEFILLCOLOR:
				OpenColorDialog(hWnd, false);
				BringWindowToTop(clockWindow);
				break;
			case IDM_PRINT:
				CPencilDrawer::status = false;
				textManager->DeleteInput();
				CPrintRectangleDrawer::status = PRINT_NO;
				CPolygonDrawer::status = POLYGON_NO;
				CPrintRectangleDrawer::status = PRINT_START;
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
		if (PRINT_SELECTING == CPrintRectangleDrawer::status)
		{
			CWindowStatus::CWindowStatus::isDrawing = false;
			HPEN printPen = CreatePen(PS_DASH, 1, RGB (90, 10, 10));
			SelectObject(memory, GetStockObject(HOLLOW_BRUSH));
			SelectObject(memory, printPen);
			BitBlt(memory, 0, 0, CWindowStatus::width, CWindowStatus::hight, secondMemory, 0, 0, SRCCOPY);
			int a = LOWORD(lParam);
			int b = HIWORD(lParam);
			if ((a > printDrawer->checkedPoint.x) && (b > printDrawer->checkedPoint.y))
			Rectangle(memory, printDrawer->checkedPoint.x, printDrawer->checkedPoint.y, a, b);
		}	
		if (POLYGON_PROCESS == CPolygonDrawer::status)
		{
			CWindowStatus::isDrawing = false;
			BitBlt(memory, 0, 0, CWindowStatus::width, CWindowStatus::hight, secondMemory, 0, 0, SRCCOPY);
			int a = LOWORD(lParam);
			int b = HIWORD(lParam);
			CLineDrawer lineDrawer;
			lineDrawer.Draw(memory, (polygonDrawer->checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, (polygonDrawer->checkedPoint.y- CWindowStatus::movingPoint.y) * CWindowStatus::scale, a, b);
		}
		if ((CPencilDrawer::status) && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
		{
			CWindowStatus::isDrawing = false;
			int a = LOWORD(lParam);
			int b = HIWORD(lParam);
			CLineDrawer lineDrawer;
			HPEN pen = CreatePen(PS_SOLID, 5, CPaintManager::color);
			SelectObject(memory, pen);
			SelectObject(CMetafileManager::mdc, pen);
			if (!((pencilDrawer->checkedPoint.x == 0) && (pencilDrawer->checkedPoint.y == 0)))
			{
				lineDrawer.Draw(memory, pencilDrawer->checkedPoint.x, pencilDrawer->checkedPoint.y, a, b);
				lineDrawer.Draw(CMetafileManager::mdc, pencilDrawer->checkedPoint.x, pencilDrawer->checkedPoint.y, a, b);
			}				
			else
			{
				lineDrawer.Draw(memory, a, b, a + 1, b + 1);
				lineDrawer.Draw(CMetafileManager::mdc, a, b, a + 1, b + 1);
			}			
			pencilDrawer->checkedPoint.x = a;
			pencilDrawer->checkedPoint.y = b;

		}
		if (true == CWindowStatus::isDrawing)
		{
			BitBlt(memory, 0, 0, CWindowStatus::width*CWindowStatus::scale, CWindowStatus::hight*CWindowStatus::scale, secondMemory, 0, 0, SRCCOPY);
			int a = 0;
			int b = 0;
			a = LOWORD(lParam);
			b = HIWORD(lParam);
			CPaintManager::DrawPrimitive(memory, (CPaintManager::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, (CPaintManager::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale, a, b);
		}
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);
	}
	break;
	case WM_LBUTTONDOWN:
	{	
		if (CPrintRectangleDrawer::status != PRINT_SELECTING) BitBlt(secondMemory, 0, 0, CWindowStatus::width, CWindowStatus::hight, memory, 0, 0, SRCCOPY);
		SelectObject(memory, CPaintManager::pen);
		SelectObject(memory, CPaintManager::brush);
		SelectObject(CMetafileManager::mdc, CPaintManager::pen);
		SelectObject(CMetafileManager::mdc, CPaintManager::brush);
		CPaintManager::checkedPoint.x = LOWORD(lParam);
		CPaintManager::checkedPoint.y = HIWORD(lParam);
		CPaintManager::checkedPoint.x /= CWindowStatus::scale;
		CPaintManager::checkedPoint.y /= CWindowStatus::scale;
		CPaintManager::checkedPoint.x += CWindowStatus::movingPoint.x;
		CPaintManager::checkedPoint.y += CWindowStatus::movingPoint.y;

		if (textManager->GetStatus() > TEXT_NO)
		{
			if (textManager->GetStatus() == TEXT_PLACED)
			{
				textManager->SetStatus(TEXT_EDIT);
				textManager->CreateInput(hWnd, memory, CPaintManager::checkedPoint.x, CPaintManager::checkedPoint.y);
			}
		}

		if (CPrintRectangleDrawer::status > PRINT_NO)
		{
			if (CPrintRectangleDrawer::status == PRINT_START)
			{
				CPrintRectangleDrawer::status = PRINT_SELECTING;
				printDrawer->checkedPoint.x = CPaintManager::checkedPoint.x;
				printDrawer->checkedPoint.y = CPaintManager::checkedPoint.y;
			}				
		}

		if (CPolygonDrawer::status > POLYGON_NO)
		{
			if (CPolygonDrawer::status == POLYGON_PROCESS)
			{
				CLineDrawer lineDrawer;
				lineDrawer.Draw(CMetafileManager::mdc, (polygonDrawer->checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, 
			    (polygonDrawer->checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale, (CPaintManager::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, 
			     (CPaintManager::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale);
			}
			if (CPolygonDrawer::status == POLYGON_BEGIN)
			{
				CPolygonDrawer::status = POLYGON_PROCESS;
			}			
			polygonDrawer->checkedPoint.x = CPaintManager::checkedPoint.x;
			polygonDrawer->checkedPoint.y = CPaintManager::checkedPoint.y;
		}

		if (CPrintRectangleDrawer::status + CPolygonDrawer::status + textManager->GetStatus() > 0)
			CWindowStatus::isDrawing = false;
		else
			CWindowStatus::isDrawing = true;
				
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (CPolygonDrawer::status == POLYGON_PROCESS)
		{
			BitBlt(memory, 0, 0, CWindowStatus::width, CWindowStatus::hight, secondMemory, 0, 0, SRCCOPY);
			CPolygonDrawer::status = POLYGON_NO;
			InvalidateRect(hWnd, 0, FALSE);
			UpdateWindow(hWnd);
		}

	}
	case WM_LBUTTONUP:
	{
		int a = LOWORD(lParam);
		int b = HIWORD(lParam);
		if (CPencilDrawer::status > 0)
		{
			pencilDrawer->checkedPoint.x = 0;
			pencilDrawer->checkedPoint.y = 0;
		}
		if (CPrintRectangleDrawer::status == PRINT_SELECTING)
		{					
			CPrintRectangleDrawer::status = PRINT_FINAL;
			BitBlt(memory, 0, 0, CWindowStatus::width, CWindowStatus::hight, secondMemory, 0, 0, SRCCOPY);			
			ShowPrintDialog(printDrawer->checkedPoint.x, printDrawer->checkedPoint.y, a, b);
			ShowWindow(clockWindow, SW_RESTORE);
			BringWindowToTop(clockWindow);
		}			
		else
		if (CWindowStatus::isDrawing)
		{
			CWindowStatus::isDrawing = false;
			CPaintManager::DrawPrimitive(memory, (CPaintManager::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, (CPaintManager::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale, a, b);
			CPaintManager::DrawPrimitive(CMetafileManager::mdc, (CPaintManager::checkedPoint.x - CWindowStatus::movingPoint.x) * CWindowStatus::scale, (CPaintManager::checkedPoint.y - CWindowStatus::movingPoint.y) * CWindowStatus::scale, a, b);
		}
		
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);
	}
	break;
	case WM_MOUSEWHEEL:
	{		
		int mouseDirection = GetIntSign(GET_WHEEL_DELTA_WPARAM(wParam));
		if (wParam & MK_CONTROL)
		{	
			if (CWindowStatus::IsScalingPossible(mouseDirection))
			{
				CWindowStatus::scale += 0.1*mouseDirection;
				CWindowStatus::isScaling = true;
			}			
		}
		else if (CWindowStatus::IsMovingPossible())
		{		
			if (wParam & MK_SHIFT)
			{
				CWindowStatus::movingPoint.x += 10 * mouseDirection;			
			}
			else
			{
				CWindowStatus::movingPoint.y += 10 * mouseDirection;				
			}
			CWindowStatus::isMoving = true;
		}
			
		InvalidateRect(hWnd, 0, FALSE);
		UpdateWindow(hWnd);		
	}
	break;
    case WM_PAINT:
        {	
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);				
			if (CWindowStatus::isScaling)
			{				
				SetMapMode(memory, MM_ANISOTROPIC);
				SetMapMode(secondMemory, MM_ANISOTROPIC);
				SetMapMode(CMetafileManager::mdc, MM_ANISOTROPIC);
				SetViewportExtEx(memory, CWindowStatus::width, CWindowStatus::hight, NULL);
				SetWindowExtEx(memory, CWindowStatus::width * CWindowStatus::scale, CWindowStatus::hight * CWindowStatus::scale, NULL);
				SetViewportExtEx(secondMemory, CWindowStatus::width, CWindowStatus::hight, NULL);
				SetWindowExtEx(secondMemory, CWindowStatus::width * CWindowStatus::scale, CWindowStatus::hight * CWindowStatus::scale, NULL);
				SetViewportExtEx(CMetafileManager::mdc, CWindowStatus::width, CWindowStatus::hight, NULL);
				SetWindowExtEx(CMetafileManager::mdc, CWindowStatus::width * CWindowStatus::scale, CWindowStatus::hight * CWindowStatus::scale, NULL);				
				HPEN printPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				SelectObject(hdc, printPen);
				Rectangle(hdc, -1, -1, CWindowStatus::width + 1, CWindowStatus::hight + 1);
				CWindowStatus::isScaling = false;	
			}	
			if (CWindowStatus::isMoving)
			{	
				SetViewportOrgEx(memory, CWindowStatus::movingPoint.x, CWindowStatus::movingPoint.y, NULL);
				SetViewportOrgEx(secondMemory, CWindowStatus::movingPoint.x, CWindowStatus::movingPoint.y, NULL);
				SetViewportOrgEx(CMetafileManager::mdc, CWindowStatus::movingPoint.x, CWindowStatus::movingPoint.y, NULL);
				HPEN printPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				SelectObject(hdc, printPen);
				Rectangle(hdc, -1, -1, CWindowStatus::width + 1, CWindowStatus::hight + 1);
				CWindowStatus::isMoving = false;	
			}
			if (textManager->GetStatus() == TEXT_EDIT)
			{
				BitBlt(memory, 0, 0, CWindowStatus::width, CWindowStatus::hight, secondMemory, 0, 0, SRCCOPY);
				textManager->OutText(memory);
				textManager->UpdateCaret(hWnd, memory);
			}


			if (textManager->GetStatus() + CPolygonDrawer::status+ int(CPencilDrawer::status) + CPrintRectangleDrawer::status + int(CWindowStatus::isDrawing))
				ShowWindow(clockWindow, SW_HIDE); 
			else
				
			{
				ShowWindow(clockWindow, SW_RESTORE);
				BringWindowToTop(clockWindow);
			}

			BitBlt(hdc, 0, 0, CWindowStatus::width, CWindowStatus::hight, memory, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
			
        }
        break;
	case WM_ACTIVATE:
	{
		BringWindowToTop(clockWindow);
		break;
	}
	case WM_MOVE:
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);
		MoveWindow(clockWindow, rc.right - CWindowStatus::widthClock - CLOCK_POSITION_CORRECTION_X, rc.top + CLOCK_POSITION_CORRECTION_Y, CWindowStatus::widthClock, CWindowStatus::hightClock, true);
		UpdateWindow(clockWindow);	
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}





LRESULT CALLBACK WndProcClock(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int        cxClient, cyClient;
	static SYSTEMTIME stPrevious;
	BOOL              fChange;
	HDC               hdc;
	PAINTSTRUCT       ps;
	SYSTEMTIME        st;

	switch (message)
	{
	case WM_MOVE:
	{
		break;
	}
	case WM_SIZE:
	{
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	}
	case WM_CREATE:
	{

		SetTimer(hWnd, ID_TIMER, 1000, NULL);
		GetLocalTime(&st);
		stPrevious = st;
		
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);
		style &= ~(WS_CAPTION);
		style |= WS_CHILD;
		SetWindowLong(hWnd, GWL_STYLE, style);
		RECT rc;
		GetWindowRect(mainWindow, &rc);
		MoveWindow(hWnd, rc.right - CWindowStatus::widthClock - CLOCK_POSITION_CORRECTION_X, rc.top + CLOCK_POSITION_CORRECTION_Y, CWindowStatus::widthClock, CWindowStatus::hightClock, true);
		UpdateWindow(hWnd);
		break;
	}
	case WM_TIMER:
	{
		GetLocalTime(&st);

		fChange = st.wHour != stPrevious.wHour ||
			st.wMinute != stPrevious.wMinute;

		hdc = GetDC(hWnd);

		clock->SetIsotropic(hdc, cxClient, cyClient);

		SelectObject(hdc, GetStockObject(WHITE_PEN));
		clock->DrawHands(hdc, &stPrevious, fChange);

		SelectObject(hdc, GetStockObject(BLACK_PEN));
		clock->DrawHands(hdc, &st, TRUE);

		ReleaseDC(hWnd, hdc);

		stPrevious = st;
		return 0;
	}		

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		clock->SetIsotropic(hdc, cxClient, cyClient);

		clock->DrawClock(hdc);
		clock->DrawHands(hdc, &stPrevious, TRUE);

		EndPaint(hWnd, &ps);
		return 0;
	}
		
	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMER);
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

