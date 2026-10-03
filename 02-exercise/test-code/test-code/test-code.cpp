// test-code.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "test-code.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int ak = 0; // помощен брояч за анимация на вълните и кораба

// Forward declarations of functions included in this code module:
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

	// TODO: Place code here.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	GdiplusStartup(
		&gdiplusToken,
		&gdiplusStartupInput,
		NULL
	);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTCODE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTCODE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTCODE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTCODE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

VOID DRAW_Waves_and_Ship(HDC hdc, int ak, int xend, int yend)
{

	// ak – помощен брояч
	// xend, yend – размер на растера

	Graphics graphics(hdc);  // инициализация на графичен режим

	int dx = xend / 9;			       // определяне на стъпка по х	
	int dy = yend / 2;			       // определяне на стъпка по y
	Point p[] = {			// вълни при положение 1
		   Point(0, dy + 5),   // стартова точка на първия сплайн
		   Point(dx, dy + 30),    // първа управляваща точка на първия сплайн
		   Point(2 * dx, dy - 30),    // втора управляваща точка на първия сплайн
		   Point(3 * dx, dy + 5),  // крайна точка на първия сплайн и 
		   // стартова точка на втория сплайн
Point(4 * dx, dy + 30),   // първа управляваща точка на втория сплайн
Point(5 * dx, dy - 30),  // втора управляваща точка на втория сплайн
Point(6 * dx, dy + 5),  // крайна точка на втория сплайн и 
// стартова точка на третия сплайн
Point(7 * dx, dy + 30),  // първа управляваща точка на третия сплайн
Point(8 * dx, dy - 30),  // втора управляваща точка на третия сплайн
Point(xend,dy + 5)   // крайна точка на третия сплайн
	};
	Point p1[] = {		// вълни при положение 2
		   Point(0, dy + 10),
		   Point(dx, dy + 20),
		   Point(2 * dx, dy - 20),
		   Point(3 * dx, dy + 10),
		   Point(4 * dx, dy + 20),
		   Point(5 * dx, dy - 20),
		   Point(6 * dx, dy + 10),
		   Point(7 * dx, dy + 20),
		   Point(8 * dx, dy - 20),
		   Point(xend, dy + 10)
	};
	Point p2[] = {		// вълни при положение 3
		   Point(0, dy + 5),
		   Point(dx, dy + 10),
		   Point(2 * dx, dy - 10),
		   Point(3 * dx, dy + 5),
		   Point(4 * dx, dy + 10),
		   Point(5 * dx, dy - 10),
		   Point(6 * dx, dy + 5),
		   Point(7 * dx, dy + 10),
		   Point(8 * dx, dy - 10),
		   Point(xend, dy + 5)
	};
	Point p3[] = {		// вълни при положение 4
		   Point(0, dy),
		   Point(dx, dy - 30),
		   Point(2 * dx, dy + 30),
		   Point(3 * dx, dy),
		   Point(4 * dx, dy - 30),
		   Point(5 * dx, dy + 30),
		   Point(6 * dx, dy),
		   Point(7 * dx, dy - 30),
		   Point(8 * dx, dy + 30),
		   Point(xend, dy)
	};

	Pen Neavypen(Color(63, 0, 168, 196)); //цветове на вълните - молив
	SolidBrush Neavybr(Color(63, 0, 168, 196)); //цветове на вълните - четка
	GraphicsPath myPath, myPath1;

	myPath.StartFigure();
	switch (ak)
	{
	case 0:
	{
		myPath.AddLine(0, yend, 0, dy + 5);
		myPath.AddBeziers(p2, 10);
		myPath.AddLine(xend, dy + 5, xend, yend);
	}
	break;
	case 1:
	{
		myPath.AddLine(0, yend, 0, dy + 10);
		myPath.AddBeziers(p1, 10);
		myPath.AddLine(xend, dy + 10, xend, yend);
	}
	break;
	case 2:
	{
		myPath.AddLine(0, yend, 0, dy + 5);
		myPath.AddBeziers(p2, 10);
		myPath.AddLine(xend, dy + 5, xend, yend);
	}
	break;
	case 3:
	{
		myPath.AddLine(0, yend, 0, dy);
		myPath.AddBeziers(p3, 10);
		myPath.AddLine(xend, dy, xend, yend);
	}
	break;
	case 4:
	{
		myPath.AddLine(0, yend, 0, dy + 10);
		myPath.AddBeziers(p, 10);
		myPath.AddLine(xend, dy + 10, xend, yend);
	}
	break;
	}

	myPath.CloseFigure();

	myPath1.StartFigure(); // съставна фигура - кораб

	myPath1.AddLine(xend / 4, yend / 2 - 30, xend / 4 + xend / 5, yend / 2 - 30);
	myPath1.AddBezier(xend / 4, yend / 2 - 30, xend / 4 + xend / 12, yend / 2 + 60, xend / 4 + xend / 6, yend / 2 + 60, xend / 4 + xend / 5, yend / 2 - 30);
	myPath1.AddLine(xend / 4 + xend / 10, yend / 2 - 30, xend / 4 + xend / 10, yend / 2 - 80);
	myPath1.AddLine(xend / 4 + xend / 10 - 1, yend / 2 - 80, xend / 4 + xend / 10 - 1, yend / 2 - 30);
	myPath1.CloseFigure();
	SolidBrush Shipbr(Color(164, 0, 168, 16)); //оцветяване на кораба


	Matrix myMatrix; //иницииране на матрица за трансформации

	if ((ak == 1) || (ak == 3))
	{
		//задаване на отместване до началото на координатната система
		myMatrix.Translate(-float(xend / 4 + xend / 10), -float(yend / 4));
		myMatrix.Rotate(-8.0f);  //задаване на ротация
		myPath1.Transform(&myMatrix);
		float sx = float(1) + float(ak) / 10;
		myMatrix.Scale(sx, sx); //задаване на мащабиране
		myMatrix.Translate(+float(1.8 * (xend / 4 + xend / 10)), +float(3 * (yend / 5)), MatrixOrderAppend);//задаване на транслация
		myPath1.Transform(&myMatrix); // придвижване на кораба
	}
	if (ak == 2)
	{

		myMatrix.Scale(1.125f, 1.125f);
		myMatrix.Translate(-20.0f, -10.0f, MatrixOrderAppend);
		myPath1.Transform(&myMatrix); // придвижване на кораба 
	}

	if (ak == 4)
	{
		myMatrix.Translate(-float(xend / 4 + xend / 10), -float(yend / 4));
		myMatrix.Rotate(+5.0f);
		myPath1.Transform(&myMatrix);
		myMatrix.Scale(1.3f, 1.3f);
		myMatrix.Translate(+float(2.2 * (xend / 4 + xend / 10)), +float(1.8 * (yend / 4)), MatrixOrderAppend);
		myPath1.Transform(&myMatrix);
	}

	graphics.FillPath(&Shipbr, &myPath1);

	graphics.FillPath(&Neavybr, &myPath);

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		Graphics graphics(hdc);
		RECT rect;
		GetClientRect(hWnd, &rect);
		DRAW_Waves_and_Ship(hdc, ak, (int)rect.right, (int)rect.bottom);
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

// Message handler for about box.
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

#pragma comment(lib, "gdiplus.lib");