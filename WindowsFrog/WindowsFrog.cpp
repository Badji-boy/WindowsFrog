#include "Structures.h"
#include "logick.h"
#include "GameFileSystem.h"
#include "APPGame.h"

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


//if (dialogCollision == true)
//{
//
//    float txtX = location[player->currentLocation].Persona[name]->DialogSprite.x;
//    float txtY = location[player->currentLocation].Persona[name]->DialogSprite.y;
//    location[player->currentLocation].Persona[name]->DialogSprite.show(hdcBuffer);
//    PointF txtBounds(txtX, txtY);
//    g.DrawString(L"Hello", -1, &font, txtBounds, &solidBrush);
//
//}