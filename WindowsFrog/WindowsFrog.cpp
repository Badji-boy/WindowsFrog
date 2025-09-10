#include "Structures.h"
#include "logick.h"
#include "GameFileSystem.h"
#include "APPGame.h"

#pragma comment(lib, "GdiPlus.lib")
#define MAX_LOADSTRING 100

bool isLevelLoaded = false;

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    App.FrameGo();
    GdiplusShutdown(gdiplusToken);
    return 0;
};


