// WinPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "PaintConfig.h"
#include "WindowConfig.h"
#include "RectangleDrawer.h"
#include "LineDrawer.h"
#include "EllipseDrawer.h"
#include "TriangleDrawer.h"
#include "WinPaint.h"


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


HDC memory, secondMemory;
HBITMAP memoryBitmap, secondMemoryBitmap;
bool isDrawing;
char isPrinting;
bool isMoving;
bool isScaling;
bool isPencil;

int x = 0;
int y = 0;
int xPencil = 0;
int yPencil = 0;
int xPrint = 0;
int yPrint = 0;



// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINPAINT));

    MSG msg;

    // Цикл основного сообщения:
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
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);  

   return TRUE;  
}

void InitializeWindow()
{
	isDrawing = false;
	isPrinting = 0;
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
		MessageBox(NULL, L"Canсelled", L"Print", MB_OK | MB_ICONERROR);
		return -1;
	}

	DOCINFO  documentInformation;
	HDC hPrinter = prDlg.hDC;

	memset(&documentInformation, 0, sizeof(documentInformation));
	documentInformation.cbSize = sizeof(documentInformation);
	StartDoc(hPrinter, &documentInformation);

	StartPage(hPrinter);

	StretchBlt(hPrinter, 0, 0, a * 2, b * 2, secondMemory, x, y, abs(a-x), abs(b-y), SRCCOPY);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

    switch (message)
    {
	case WM_CREATE:
	    {
			InitializeWindow();
			CreateNewLayer(hWnd, &memory, &memoryBitmap, CWindowConfig::width, CWindowConfig::hight);
			CreateNewLayer(hWnd, &secondMemory, &secondMemoryBitmap, CWindowConfig::width, CWindowConfig::hight);
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
		
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
			case ID_TOOLS_PENCIL:
			{
				isPencil = true;
				break;
			}			
			case ID_TOOLS_LINE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CLineDrawer());
				isPencil = false;
				break;
			}							
			case ID_TOOLS_TRIANGLE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CTriangleDrawer());
				isPencil = false;
				break;
			}								
			case ID_TOOLS_ELLIPSE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CEllipseDrawer());
				isPencil = false;
				break;
			}						
			case ID_TOOLS_RECTANGLE:
			{
				CPaintConfig::ChoosePrimitiveDrawer(new CRectangleDrawer());
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
		if ((isPencil) && ((GetKeyState(VK_LBUTTON) & 0x100) != 0))
		{
			isDrawing = false;
			int a = LOWORD(lParam);
			int b = HIWORD(lParam);
			CLineDrawer* lineDrawer = new CLineDrawer();
			HPEN pen = CreatePen(PS_SOLID, 5, CPaintConfig::color);
			SelectObject(memory, pen);
			if (!((xPencil == 0) && (yPencil == 0)))
				lineDrawer->Draw(memory, xPencil, yPencil, a, b);
			else
				lineDrawer->Draw(memory, a, b, a+1, b+1);
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
		else
		{			
			isDrawing = true;
		}		
	}
	break;
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
			CWindowConfig::scale += 0.1*mouseDirection;
			isScaling = true;		
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
				HPEN printPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				SelectObject(hdc, printPen);
				SetMapMode(memory, MM_ANISOTROPIC);
				SetMapMode(secondMemory, MM_ANISOTROPIC);
				SetViewportExtEx(memory, CWindowConfig::width, CWindowConfig::hight, NULL);
				SetWindowExtEx(memory, CWindowConfig::width * CWindowConfig::scale, CWindowConfig::hight * CWindowConfig::scale, NULL);
				SetViewportExtEx(secondMemory, CWindowConfig::width, CWindowConfig::hight, NULL);
				SetWindowExtEx(secondMemory, CWindowConfig::width * CWindowConfig::scale, CWindowConfig::hight * CWindowConfig::scale, NULL);
				Rectangle(hdc, -1, -1, CWindowConfig::width + 1, CWindowConfig::hight + 1);
				isScaling = false;			
			}	
			if (isMoving)
			{	
				SetViewportOrgEx(memory, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
				SetViewportOrgEx(secondMemory, CWindowConfig::movingPoint.x, CWindowConfig::movingPoint.y, NULL);
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

