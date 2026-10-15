#include "framework.h"
#include "testcode.h"
#include "gl/gl.h"
#include "gl/glu.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

GLfloat ambient_light[] = { 0.8, 0.0, 0.0, 1.0 }; // цвят за AMBIENT светлина 
GLfloat diffuse_light[] = { 0.0, 0.0, 1.0, 1.0 }; // цвят за DIFFUSE светлина 
GLfloat light_position[] = { 0.0, 0.0, -32.0, 1.0 }; // позиция на източника 
GLfloat emission[] = { 0.8, 0.8, 0.0, 1.0 }; // цвят за EMISSION светлина  
GLfloat emission_none[] = { 0.0, 0.0, 0.0, 1.0 };// черен цвят за изключване на EMISSION 
float r = 0;
HGLRC hRC = NULL;
HDC hdc;
HWND hWnd;

void DefineView(int width, int height);
void Render();
void Lighting();


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
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
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
		0, 0, 0, 0, 0, 0,                   // Not used to select mode

		0, 0,                           // Not used to select mode
		0, 0, 0, 0, 0,                     // Not used to select mode
		16,                            // Size of depth buffer
		0,                             // Not used here
		0,                             // Not used here
		0,                             // Not used here
		0,                             // Not used here
		0, 0, 0 };                       // Not used here


	// Choose a pixel format that best matches that described in pfd

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
void DefineView(int width, int height)
{
	if (height == 0) height = 1;

	// задаване на изглед върху целия екран 
	glViewport(0, 0, width, height);
	// зареждане на матрицата на проекцията
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// определяне на типа на проекцията  
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1, 200.0);
	// зареждане на моделната матрица         
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//зареждане на параметрите на динамичната светлина 
	Lighting();
	//определяне на разстоянията до обектите
	glEnable(GL_DEPTH_TEST);
	//задаване на фонов цвят
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
void Lighting()
{ // осветление

	glEnable(GL_LIGHTING); // включване на динамичната светлина
	// глобална подсветка
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	// определяне на осветяване на фигурите при AMBIENT и DIFFUSE светлина
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_light);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_light);
	// задаване на запазване на цвета на обекта при включване на динамичната светлина
	glEnable(GL_COLOR_MATERIAL);
	// определяне на видовете светлина участващи в крайното оцветяване
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}
void Render()
{ //изтриване на съдържанието на буферите
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // зареждане на параметри   
	// определяне на DIFFUSE излъчване
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	// определяне на позиция на източника
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// включване на светлинен източник
	glEnable(GL_LIGHT0);
	//управление на позицията на наблюдателя               
	r += 0.1; if (r >= 180.0) r = 0.0;
	//определяне на позицията на наблюдателя
	gluLookAt(0.0, r, -20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// поставяне на моделната матрица за обслужване в стека с фигурата
	glPushMatrix();
	//задаване на цвят на фигурата
	glColor3f(1.0, 0.03, 0.92);
	//създаване на фигурата
	glBegin(GL_TRIANGLES);
	glVertex3f(-2.0, -2.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);
	glVertex3f(2.0, -2.0, 0.0);
	glEnd();
	//извеждане на матрицата от стека
	glPopMatrix();
	//смяна на буферите		
	SwapBuffers(hdc);
	//връщане на управлението към системата
	ReleaseDC(hWnd, hdc);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
		// съхраняване на контекста на устройството
		hdc = GetDC(hWnd);
		// селектиране на пикселния формат
		SetDCPixelFormat(hdc);
		// създаване на контекст за рендиране и насочване като текущ
		hRC = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hRC);

		break;

		// Window is being destroyed, clean up
	case WM_DESTROY:

		// деселектиране на текущия контекст за рендиране и изтриване
		hdc = GetDC(hWnd);
		wglMakeCurrent(hdc, NULL);
		wglDeleteContext(hRC);

		//приложението спира след затваряне на прозореца  
		PostQuitMessage(0);
		break;

		// Промяна на размера на прозореца.
		// Извикване на функцията, която задава размер на прозореца и изгледа
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
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);EndPaint(hWnd, &ps);

		Render();

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