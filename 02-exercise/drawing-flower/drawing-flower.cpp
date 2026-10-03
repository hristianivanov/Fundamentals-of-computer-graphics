// drawing-flower.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "drawing-flower.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int ak = 0;

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
	LoadStringW(hInstance, IDC_DRAWINGFLOWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAWINGFLOWER));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAWINGFLOWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DRAWINGFLOWER);
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

void DrawStem(Graphics& graphics, int centerX, int centerY)
{
	GraphicsPath stem;

	stem.AddBezier(
		centerX, centerY,
		centerX - 30, centerY + 80,
		centerX + 30, centerY + 160,
		centerX, centerY + 250
	);

	Pen stemPen(Color (255, 0, 150, 0), 8);

	graphics.DrawPath(&stemPen, &stem);
}

void DrawPetal(Graphics& graphics, int centerX, int centerY, float angle)
{
	GraphicsPath petal;

	petal.AddBezier(
		centerX, centerY,
		centerX - 40, centerY - 50,
		centerX - 40, centerY - 100,
		centerX, centerY - 140
	);

	petal.AddBezier(
		centerX, centerY - 140,
		centerX + 40, centerY - 100,
		centerX + 40, centerY - 50,
		centerX, centerY
	);

	petal.CloseFigure();

	Matrix matrix;

	matrix.Translate(-centerX, -centerY);
	matrix.Rotate(angle, MatrixOrderAppend);
	matrix.Translate(centerX, centerY, MatrixOrderAppend);

	petal.Transform(&matrix);

	SolidBrush petalBrush(Color(255, 255, 150, 200));
	Pen outline(Color(255, 220, 80, 140),3);

	graphics.FillPath(
		&petalBrush,
		&petal
	);

	graphics.DrawPath(
		&outline,
		&petal
	);
}

void DrawCenter(Graphics& graphics, int centerX, int centerY)
{
	SolidBrush yellowBrush(Color(255, 255, 220, 0));

	graphics.FillEllipse(
		&yellowBrush,
		centerX - 30,
		centerY - 30,
		60,
		60
	);
}

void DrawSky(Graphics& graphics, int width, int height)
{
	SolidBrush skyBrush(Color(255, 180, 220, 255));

	graphics.FillRectangle(
		&skyBrush,
		0,
		0,
		width,
		height
	);
}

void DrawGround(Graphics& graphics, int width, int height)
{
	SolidBrush groundBrush(Color(255, 100, 180, 80));

	graphics.FillRectangle(
		&groundBrush,
		0,
		height * 2 / 3,
		width,
		height / 3
	);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		ak++;

		if (ak > 8)
			ak = 0;

		InvalidateRect(hWnd, NULL, TRUE);
		break;
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

		Graphics graphics(hdc);

		RECT rect;
		GetClientRect(hWnd, &rect);

		int centerX = rect.right / 2;
		int centerY = rect.bottom / 3;

		DrawSky(graphics, rect.right, rect.bottom);
		DrawGround(graphics, rect.right, rect.bottom);

		DrawStem(graphics, centerX, centerY);

		for (int i = 0; i < ak; i++)
		{
			float angle = i * 45.0f;

			DrawPetal(graphics, centerX, centerY, angle);
		}

		DrawCenter(graphics, centerX, centerY);

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
