#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <iostream>
#include <string>
#include <source_location>
#include <fstream>
#include "resource.h"
#include "framework.h"
#include "gdiplus.h"
#include "gdiplusgraphics.h"
#include <shlwapi.h> // Для PathFileExists
#pragma comment(lib, "shlwapi.lib")

using namespace std;
using namespace Gdiplus;
HDC hdc;
int name = 0;
int currenttime = 0;
POINT mouse;
bool startDialog = false;
bool endDialog = false;

struct
{
    HDC context;
    int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);//сюда сохраним размеры окна которое создаст программа
} window;

struct
{
    int x = 0;
    int y = 0;
    int w;
    int h;
} player_view;
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
};

class Window
{
private:
    const char* NameClass = "Window";
    RECT rc;
    HINSTANCE hIns;
    HWND hWnd;
    PAINTSTRUCT ps;
public:
    Window(int Width, int Height, const char* NameWind)
    {
        rc = { 0,0,Width,Height
        };

        AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

        WNDCLASSEX wc = { 0 };
        wc.cbSize = sizeof(wc);
        wc.lpszClassName = NameClass;
        wc.hInstance = hIns;
        wc.lpfnWndProc = &WindowProc;

        auto NameClassId = RegisterClassEx(&wc);

        hWnd = CreateWindowEx(
            NULL,
            MAKEINTATOM(NameClassId),
            NameWind,
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rc.right - rc.left,
            rc.bottom - rc.top,
            NULL,
            NULL,
            hIns,
            NULL
        );
        
        hdc = GetDC(hWnd);
        window.context = CreateCompatibleDC(hdc);
        SelectObject(window.context, CreateCompatibleBitmap(hdc, window.width, window.height));
        ShowWindow(hWnd, SW_SHOW);
        
    };
   /* ~Window()
    {
        DestroyWindow(hWnd);
    };*/

    HWND GetHWND()
    {
        return hWnd;
    }
}win(window.width, window.height, "GameFrog");



float scale = 2;
struct sprite {
    float x, y, width, height, dx, dy, speed, jump, gravity;
    Image* image;

    void loadBitmapWithNativeSize(const wstring& filename)
    {
        const wstring s = filename + L".bmp";
        image = new Image(s.c_str());
    }

    void show(Graphics& g)
    {
        float vx = (x - player_view.x) * scale + window.width / 2.;
        float vy = (y - player_view.y) * scale + window.height / 2.;
        float vw = width * scale;
        float vh = height * scale;

        bool in = false;

        if (vx + vw >= 0 && vx < window.width &&
            vy + vh >= 0 && vy < window.height)
            in = true;

        if (!in) return;
        g.DrawImage(image, vx, vy, vw, vh);
    }
    void showBack(Graphics& g)
    {
        g.DrawImage(image, 0, 0, window.width, window.height);
    }
    void showDialog(Graphics& g)
    {
        float vx = 0;
        float vy = window.height - (window.height / 4.);
        g.DrawImage(image, vx, vy, width, height);
    }

};
