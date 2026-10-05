// imgae-encoder.cpp : Defines the entry point for the application.
/*
Да се направи запис на изображения от Тема 2 в BMP, GIF и JPEG формати.
*/

#include "framework.h"
#include "imgae-encoder.h"
#include <string>

using namespace std;

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

// Рисуващи функции за Тема 2
VOID DrawSky(Graphics& graphics, int width, int height)
{
    SolidBrush skyBrush(Color(255, 180, 220, 255));
    graphics.FillRectangle(&skyBrush, 0, 0, width, height);
}

VOID DrawGround(Graphics& graphics, int width, int height)
{
    SolidBrush groundBrush(Color(255, 100, 180, 80));
    graphics.FillRectangle(&groundBrush, 0, height * 2 / 3, width, height / 3);
}

VOID DrawStem(Graphics& graphics, int centerX, int centerY)
{
    GraphicsPath stem;
    stem.AddBezier(
        centerX, centerY,
        centerX - 30, centerY + 80,
        centerX + 30, centerY + 160,
        centerX, centerY + 250
    );

    Pen stemPen(Color(255, 0, 150, 0), 8);
    graphics.DrawPath(&stemPen, &stem);
}

VOID DrawPetal(Graphics& graphics, int centerX, int centerY, float angle)
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
    Pen outline(Color(255, 220, 80, 140), 3);

    graphics.FillPath(&petalBrush, &petal);
    graphics.DrawPath(&outline, &petal);
}

VOID DrawCenter(Graphics& graphics, int centerX, int centerY)
{
    SolidBrush yellowBrush(Color(255, 255, 220, 0));
    graphics.FillEllipse(&yellowBrush, centerX - 30, centerY - 30, 60, 60);
}

// Помощна функция за вземане на CLSID за съответния формат
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;
    UINT size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;

    ImageCodecInfo* imageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (imageCodecInfo == nullptr)
        return -1;

    GetImageEncoders(num, size, imageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(imageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = imageCodecInfo[j].Clsid;
            free(imageCodecInfo);
            return j;
        }
    }

    free(imageCodecInfo);
    return -1;
}

// Универсална функция за запис на Bitmap в BMP, GIF и JPEG
VOID SAVE_BITMAP_TO_FORMATS(Bitmap& bitmap, const wchar_t* baseFilename)
{
    CLSID clsid;

    // 1. BMP
    if (GetEncoderClsid(L"image/bmp", &clsid) != -1) {
        wstring bmpName = wstring(baseFilename) + L".bmp";
        bitmap.Save(bmpName.c_str(), &clsid, NULL);
    }

    // 2. GIF
    if (GetEncoderClsid(L"image/gif", &clsid) != -1) {
        wstring gifName = wstring(baseFilename) + L".gif";
        bitmap.Save(gifName.c_str(), &clsid, NULL);
    }

    // 3. JPEG
    if (GetEncoderClsid(L"image/jpeg", &clsid) != -1) {
        EncoderParameters encoderParameters;
        ULONG quality = 95;
        encoderParameters.Count = 1;
        encoderParameters.Parameter[0].Guid = EncoderQuality;
        encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
        encoderParameters.Parameter[0].NumberOfValues = 1;
        encoderParameters.Parameter[0].Value = &quality;

        wstring jpgName = wstring(baseFilename) + L".jpg";
        bitmap.Save(jpgName.c_str(), &clsid, &encoderParameters);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IMGAEENCODER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMGAEENCODER));
    MSG msg;

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
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMGAEENCODER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IMGAEENCODER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

        RECT rect;
        GetClientRect(hWnd, &rect);

        int width = rect.right;
        int height = rect.bottom;

        if (width > 0 && height > 0)
        {
            // 1. Създаваме буфер в паметта
            Bitmap bufferBitmap(width, height, PixelFormat32bppARGB);
            Graphics bufferGraphics(&bufferBitmap);
            bufferGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

            int centerX = width / 2;
            int centerY = height / 3;

            // 2. Чертаем цялата сцена от Тема 2 върху буфера
            DrawSky(bufferGraphics, width, height);
            DrawGround(bufferGraphics, width, height);
            DrawStem(bufferGraphics, centerX, centerY);

            for (int i = 0; i < ak; i++)
            {
                float angle = i * 45.0f;
                DrawPetal(bufferGraphics, centerX, centerY, angle);
            }

            DrawCenter(bufferGraphics, centerX, centerY);

            // 3. Показваме изображението на екрана
            Graphics windowGraphics(hdc);
            windowGraphics.DrawImage(&bufferBitmap, 0, 0);

            // 4. Запазваме изчертаното изображение във формати BMP, GIF и JPEG
            SAVE_BITMAP_TO_FORMATS(bufferBitmap, L"Flower_Tema2");
        }

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

#pragma comment(lib, "gdiplus.lib")