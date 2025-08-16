#include "Structures.h"
#include "logick.h"
#include "GameFileSystem.h"
#pragma comment(lib, "GdiPlus.lib")
#define MAX_LOADSTRING 100
float lerp(float x1, float x2, float a)
{
    return x1 * (1 - a) + x2 * a;
}

float length(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
bool isLevelLoaded = false;
HDC hdcBuffer = NULL;
HBITMAP hBitmap = NULL;
int bufferWidth = 0, bufferHeight = 0;
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
    LoadStringW(hInstance, IDC_WINDOWSFROG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSFROG));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSFROG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSFROG);
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

    SetTimer(hWnd, 1, 16, NULL); // ~60 FPS

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        InvalidateRect(hWnd, NULL, FALSE); // Перерисовываем окно
    }
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
        FontFamily  fontFamily(L"Times New Roman");
        Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        SolidBrush  solidBrush(Color(255, 0, 0, 255));
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        Graphics g(hdc);
        
        GetClientRect(hWnd, &rc);
        if (!isLevelLoaded)
        {
            LoadSVGDataMap(L"LVL0", rc);
            isLevelLoaded = true;
        }
        
        // Создаем буфер, если нужно
        if (hdcBuffer == NULL || bufferWidth != rc.right || bufferHeight != rc.bottom)
        {
            if (hdcBuffer) DeleteDC(hdcBuffer);
            if (hBitmap) DeleteObject(hBitmap);

            hdcBuffer = CreateCompatibleDC(hdc);
            bufferWidth = rc.right;
            bufferHeight = rc.bottom;
            hBitmap = CreateCompatibleBitmap(hdc, bufferWidth, bufferHeight);
            SelectObject(hdcBuffer, hBitmap);
        }

        // Отрисовка в буфер
        FillRect(hdcBuffer, &rc, (HBRUSH)(COLOR_WINDOW + 1));

        
        location[player->currentLocation].hBack.showBack(hdcBuffer, rc);
        for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {

            location[player->currentLocation].Persona[i]->Sprite.show(hdcBuffer, rc);
            location[player->currentLocation].Persona[i]->move();
            location[player->currentLocation].Persona[i]->dialog(player);
        }
        player->Sprite.show(hdcBuffer, rc);
        player->move();
        //Health_bar.Show(hdcBuffer, rc);
        for (int i = 0; i < location[player->currentLocation].walls.size(); i++) {
            location[player->currentLocation].walls[i].Sprite.show(hdcBuffer, rc);
        }
        for (int i = 0; i < location[player->currentLocation].healingFlask.size(); i++) {
            location[player->currentLocation].healingFlask[i].Sprite.show(hdcBuffer, rc);
            location[player->currentLocation].healingFlask[i].healing(player, i);
        }
        for (int i = 0; i < location[player->currentLocation].spike.size(); i++) {
            location[player->currentLocation].spike[i].Sprite.show(hdcBuffer, rc);
            location[player->currentLocation].spike[i].damage(player);
        }
        for (int i = 0; i < location[player->currentLocation].portal.size(); i++) {
            location[player->currentLocation].portal[i].Sprite.show(hdcBuffer, rc);
            location[player->currentLocation].portal[i].Portal(player);
        }


        if (dialogCollision == true)
        {
            
            float txtX = location[player->currentLocation].Persona[name]->DialogSprite.x;
            float txtY = location[player->currentLocation].Persona[name]->DialogSprite.y;
            location[player->currentLocation].Persona[name]->DialogSprite.show(hdcBuffer, rc);
            PointF txtBounds(txtX, txtY);
            g.DrawString(L"Hello", -1, &font, txtBounds, &solidBrush);

        }
        


        float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
        ls = max(ls - .2, 0.1);
        ls = min(ls, 1);

        float cameraHalfWidth = (rc.right / 2) / scale;
        float cameraHalfHeight = (rc.bottom / 2) / scale;

        float targetX = player->Sprite.x;
        float targetY = player->Sprite.y;

        targetX = max(0 + cameraHalfWidth,
            min(rc.right - cameraHalfWidth, targetX));
        targetY = max(0 + cameraHalfHeight,
            min(rc.bottom - cameraHalfHeight, targetY));

        player_view.x = lerp(player_view.x, targetX, 0.1f);
        player_view.y = lerp(player_view.y, targetY, 0.1f);
        BitBlt(hdc, 0, 0, bufferWidth, bufferHeight, hdcBuffer, 0, 0, SRCCOPY);
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

