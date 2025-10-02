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
bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}

class StaticObjects
{
public:
    sprite Sprite;
    StaticObjects(float p_x, float p_y, float p_width, float p_height, const wstring& filename)
    {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;

        Sprite.loadBitmapWithNativeSize(filename);

    }
};

class Platform : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
};


class HealingFlask : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
    void healing(auto& player, int i);
};

class Spike : public StaticObjects
{
public:
    using StaticObjects::StaticObjects;
    void damage(auto& player);
};

class portal_ : public StaticObjects
{
public:
    int destination;
    portal_(float p_x, float p_y, float p_width, float p_height, const wstring& filename, int p_destination)
        : StaticObjects(p_x, p_y, p_width, p_height, filename)
    {
        destination = p_destination;
    }
    void Portal(auto& player);
};

void tracer_collide(auto& Character);




class character 
{
public:
    sprite Sprite;
    sprite DialogSprite;
    int health_width;
    int max_lives;
    int current_lives;
    int currentLocation = 0;
    int last_trace_platform_num = -1;
    int characterName;
    float maxjump = 10;
    bool inJump = false;
    const int dashDistance = 20;
    bool inJumpBot = false;
    bool wasShiftPressed = false;
    bool colis = false;
    bool dash_allow = true;
    

    character(float p_x, float p_y, float p_width, float p_height, const wstring& filename, int p_health, int p_max_lives, int p_current_lives, int name_)
    {
        Sprite.x = p_x * window.width;
        Sprite.y = p_y * window.height;
        Sprite.width = p_width * window.width;
        Sprite.height = p_height * window.height;
        DialogSprite.width = window.width;
        DialogSprite.height =window.height / 5.;
        Sprite.loadBitmapWithNativeSize(filename);
        DialogSprite.loadBitmapWithNativeSize(L"dialog");
        characterName = name_;
        health_width = p_health;
        max_lives = p_max_lives;
        current_lives = p_current_lives;
    }
   
    void dialog(auto& player);
    virtual void move() = 0;

};

void processGravity(auto& spriteName)
{
    spriteName.jump *= .9;
    spriteName.dx *= .5;
    spriteName.dy = spriteName.gravity - spriteName.jump;
}


struct Location_
{
    sprite hBack;
    vector<portal_>portal;
    vector<Platform> walls;
    vector<HealingFlask> healingFlask;
    vector<Spike> spike;
    vector<character*> Persona;
    //vector<character*> dialog;

};

Location_ location[5];


class Hero : public character
{
public:
    Hero(float p_x, float p_y, float p_width, float p_height, const wstring& filename, int p_health, int p_max_lives, int p_current_lives, int name_, int current_location)
        : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives, name_)
    {

        //string name = __FUNCTION__;

        Sprite.speed = 15;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        /*location[current_location].Persona.push_back(this);*/
    }



    void move()
    {
        if (GetAsyncKeyState(VK_LEFT)) {
            Sprite.dx = -Sprite.speed;
        }

        if (GetAsyncKeyState(VK_RIGHT)) {
            Sprite.dx = Sprite.speed;
        }

        if (GetAsyncKeyState(VK_SPACE) && inJump == false && inJumpBot == false)
        {
            Sprite.jump = 110;
            inJumpBot = true;
            inJump = true;
        }

        tracer_collide(*this);

        processGravity(Sprite);
    }
    
};

class Wolf : public character
{
public:

    int direction = 1;
    
    Wolf(float p_x, float p_y, float p_width, float p_height, const wstring& filename, int p_health, int p_max_lives, int p_current_lives, int name_, int current_location)
        : character(p_x, p_y, p_width, p_height, filename, p_health, p_max_lives, p_current_lives, name_)
    {
        Sprite.speed = 5;
        Sprite.dx = 0;
        Sprite.dy = 0;
        Sprite.jump = 0;
        Sprite.gravity = 30;
        location[current_location].Persona.push_back(this);
    }

    void move()
    {
        tracer_collide(*this);

        processGravity(Sprite);

        if (last_trace_platform_num >= 0)
        {
            if (Sprite.x <= location[currentLocation].walls[last_trace_platform_num].Sprite.x)
            {
                direction = 1;
            }

            auto& platform = location[currentLocation].walls[last_trace_platform_num].Sprite;
            if (Sprite.x + Sprite.width >= platform.x + platform.width)
            {
                direction = -1;
            }
            Sprite.dx = direction * Sprite.speed;
        }
    }
    
    
};



Hero* player;
Wolf* wolf;
Wolf* wolf2;

//class health_bar {
//public:
//    sprite health_full, health_empty;
//    
//    health_bar() {
//        health_full.loadBitmapWithNativeSize(L"health_full");
//        health_empty.loadBitmapWithNativeSize(L"health_empty");
//    }
//
//    void Show(HDC hdc, const RECT& rc) {
//
//        Graphics g(hdc);
//        int h_w = 50;
//        int margin = 10;
//        int startX = rc.right - h_w - 20;
//        int startY = 10;
//
//        for (int i = 0; i < player->max_lives; i++) {
//            if (i < player->current_lives) {
//                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_full.hBitmap, false);
//                g.DrawImage(health_full.image, startX - (i * (h_w + margin)), startY, h_w, h_w);
//            }
//            else {
//                ShowBitmap(window.context, startX - (i * (h_w + margin)), startY, h_w, h_w, health_empty.hBitmap, false);
//                g.DrawImage(health_empty.image, startX - (i * (h_w + margin)), startY, h_w, h_w);
//            }
//        }
//    }
//
//};
//
//health_bar Health_bar;

void portal_::Portal(auto& player)
{
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        if (destination > player->currentLocation)
        {
            player->currentLocation = destination;
            player->Sprite.x = location[player->currentLocation].walls[0].Sprite.x + location[player->currentLocation].walls[0].Sprite.width;
        }
        else
        {
            player->currentLocation = destination;
            player->Sprite.x = location[player->currentLocation].walls[1].Sprite.x - player->Sprite.width - Sprite.width - 60;
        }
    }
}

void HealingFlask::healing(auto& player, int i)
{
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        location[player->currentLocation].healingFlask.erase(location[player->currentLocation].healingFlask.begin() + i);
        player->current_lives++;
    }
}
void Spike::damage(auto& player)
{
    static int lastDamageTime = 0;
    bool spikeCollision = false;
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height))
    {
        spikeCollision = true;

    }
    if (spikeCollision /*&& currenttime > lastDamageTime + 1000*/) {
        player->current_lives--;
        lastDamageTime = currenttime;
        player->Sprite.jump = 60;
        player->Sprite.x += 20;
        player->inJump = true;
       // SetTimer(hWnd, 1, 1000, NULL);
    }
}
void character::dialog(auto& player)
{
    Graphics g(window.context);
    Font font(L"Times New Roman", 20.f, FontStyleBold);
    Pen blackPen(Color(255, 0, 0, 0), 3);
    SolidBrush solidBrush(Color::Yellow);
        float txtdX =player->Sprite.x;
        float txtdY = player->Sprite.y - player->Sprite.height * 2.;
        float txtdW = 50.0f;
        float txtdH = 50.0f;
    if (CheckCollision(player->Sprite.x, player->Sprite.y, player->Sprite.width, player->Sprite.height, Sprite.x, Sprite.y, Sprite.width, Sprite.height)  && endDialog == false)
    {
        RectF        rectF(txtdX, txtdY, txtdW, txtdH);
        g.DrawString(L"F", -1, &font, rectF, NULL, &solidBrush);
        g.DrawRectangle(&blackPen, rectF);
        name = characterName;
        
      if (GetAsyncKeyState('F'))
      {
        startDialog = true;

      }
        
    }
}

