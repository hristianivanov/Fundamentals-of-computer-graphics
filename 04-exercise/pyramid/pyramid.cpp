// pyramid.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "pyramid.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include <math.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Зелена подсветка (AMBIENT) с яркост 0.2
GLfloat ambient_light[] = { 0.0f, 0.2f, 0.0f, 1.0f };
// Бяла дифузна светлина
GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// Позиция на светлината = позиция на наблюдателя (началото на координатите)
GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// Посока на прожектора - гледа по -Z (към пирамидата)
GLfloat spot_direction[] = { 0.0f, 0.0f, -1.0f };

float angle = 0.0f;   // ъгъл на въртене
HGLRC hRC = NULL;
HDC hdc;
HWND hWnd;

void DefineView(int width, int height);
void Render();
void Lighting();
void DrawPyramid();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PYRAMID, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PYRAMID));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PYRAMID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PYRAMID);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure
		1,                             // Version of this structure
		PFD_DRAW_TO_WINDOW |           // Draw to window (not to bitmap)
		PFD_SUPPORT_OPENGL |           // Support OpenGL calls in window
		PFD_DOUBLEBUFFER,              // Double-buffered mode
		PFD_TYPE_RGBA,                 // RGBA color mode
		32,                            // Want 32-bit color
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		16,                            // Size of depth buffer
		0,
		0,
		0,
		0,
		0, 0, 0 };

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void DefineView(int width, int height)
{
	if (height == 0) height = 1;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 60 градусово видимо пространство, съотношение = ширина/височина на прозореца
	gluPerspective(60.0, (GLdouble)width / (GLdouble)height, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Lighting();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Lighting()
{
	glEnable(GL_LIGHTING);

	// Глобална зелена подсветка с яркост 0.2
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);

	// Прожектор: бяла дифузна светлина, ъгъл 70
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 70);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);
	glEnable(GL_LIGHT0);

	// Цветът на обекта участва в AMBIENT и DIFFUSE
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

// Чертае триъгълник с нормала, насочена навън от пирамидата
static void Tri(const float* a, const float* b, const float* c, const float* center)
{
	float u[3] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
	float v[3] = { c[0] - a[0], c[1] - a[1], c[2] - a[2] };
	float n[3] = { u[1] * v[2] - u[2] * v[1],
				   u[2] * v[0] - u[0] * v[2],
				   u[0] * v[1] - u[1] * v[0] };

	// ако нормалата сочи навътре - обръщаме я
	float d = n[0] * (a[0] - center[0]) + n[1] * (a[1] - center[1]) + n[2] * (a[2] - center[2]);
	if (d < 0) { n[0] = -n[0]; n[1] = -n[1]; n[2] = -n[2]; }

	float len = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	glNormal3f(n[0] / len, n[1] / len, n[2] / len);
	glVertex3fv(a); glVertex3fv(b); glVertex3fv(c);
}

void DrawPyramid()
{
	const float A[3] = { 0.0f,  1.2f,  0.0f };   // връх
	const float B[3] = { -1.2f, -0.8f, -0.7f };
	const float C[3] = { 1.2f, -0.8f, -0.7f };
	const float D[3] = { 0.0f, -0.8f,  1.4f };
	const float center[3] = { (A[0] + B[0] + C[0] + D[0]) / 4,
							  (A[1] + B[1] + C[1] + D[1]) / 4,
							  (A[2] + B[2] + C[2] + D[2]) / 4 };

	glBegin(GL_TRIANGLES);
	Tri(A, B, C, center);
	Tri(A, C, D, center);
	Tri(A, D, B, center);
	Tri(B, D, C, center);   // основа
	glEnd();
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Наблюдател в началото на координатите, гледа по -Z
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

	// Светлината е на мястото на наблюдателя
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);      // отдалечаваме пирамидата пред камерата
	glRotatef(angle, 0.0f, 1.0f, 0.0f);   // въртене
	glRotatef(angle * 0.5f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawPyramid();
	glPopMatrix();

	SwapBuffers(hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		SetDCPixelFormat(hdc);
		hRC = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hRC);
		SetTimer(hWnd, 1, 16, NULL);          // ~60 кадъра в секунда
		break;

	case WM_TIMER:
		angle += 1.0f;
		if (angle >= 360.0f) angle -= 360.0f;
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		int height = HIWORD(lParam);
		int width = LOWORD(lParam);
		DefineView(width, height);
	}
	break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
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
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		Render();
		EndPaint(hWnd, &ps);
	}
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

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")