// test-code.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "test-code.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* imageCodecInfo = nullptr;

	GetImageEncodersSize(&num, &size);

	if (size == 0)
		return -1;

	imageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (imageCodecInfo == nullptr)
		return -1;

	GetImageEncoders(
		num,
		size,
		imageCodecInfo
	);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(
			imageCodecInfo[j].MimeType,
			format
		) == 0)
		{
			*pClsid = imageCodecInfo[j].Clsid;

			free(imageCodecInfo);

			return j;
		}
	}

	free(imageCodecInfo);

	return -1;
}
VOID Example_SaveFile(HDC hdc)
{
	Graphics graphics(hdc);

	// създаване на Image object на базата на PNG файл.
	Image  image(L"Mosaic.png");

	// визуализация на изображението
	graphics.DrawImage(&image, 10, 10);

	// създаване на графичен обект на базата на image.
	Graphics imageGraphics(&image);

	// промяна на изображението с изчертаване на елипса в него
	SolidBrush brush(Color(255, 0, 0, 255));
	imageGraphics.FillEllipse(&brush, 20, 30, 80, 50);

	// визуализация на промяната.
	graphics.DrawImage(&image, 200, 10);

	// запазване на промененото изображение
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	image.Save(L"Proba2.png", &pngClsid, NULL);

	//промени в параметрите на енкодера 
	CLSID clsid;
	EncoderParameters encoderParameters;
	ULONG             quality;

	GetEncoderClsid(L"image/jpeg", &clsid);
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	// запис на JPEG формат изображение с компресия 99.
	quality = 99;
	encoderParameters.Parameter[0].Value = &quality;
	image.Save(L"Proba3.jpg", &clsid, &encoderParameters);


}
VOID Example_BMPbuffer(HDC hdc, int ak, int xend, int yend)
{
	Graphics graphics(hdc);  // инициализация на графичен режим 
	Bitmap bmp(xend, yend, &graphics); //създаване на обект - буфер
	Graphics imgr(&bmp); // създаване на обект от буфера
	imgr.Clear(Color(255, 255, 255)); //задаване на базов цвят бял екран

	// изчертаване на елипса в буфера
	SolidBrush brush(Color(255, 0, 0, 255));
	imgr.FillEllipse(&brush, 20, 30, 80, 50);

	graphics.DrawImage(&bmp, 0, 0); // визуализация на буфера върху екрана

	//промени в параметрите на енкодера 
	CLSID clsid;
	EncoderParameters encoderParameters;
	ULONG             quality;

	GetEncoderClsid(L"image/jpeg", &clsid);
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;

	// запис на JPEG формат изображение с компресия 99.
	quality = 99;
	encoderParameters.Parameter[0].Value = &quality;

	bmp.Save(L"Proba_elips.jpg", &clsid, &encoderParameters);
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

		Example_BMPbuffer(hdc, 0, 500, 500);

		EndPaint(hWnd, &ps);
	}
	break;
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

#pragma comment(lib, "gdiplus.lib")