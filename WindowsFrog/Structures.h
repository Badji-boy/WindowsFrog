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
#include <gdiplus.h>
using namespace std;
int currenttime = 0;
POINT mouse;
struct {
    int x = 0;
    int y = 0;
    int w;
    int h;

} player_view;

struct float2 {
    float x;
    float y;
};
float scale = 2;
struct sprite {
    float x, y, width, height, dx, dy, speed, jump, gravity;
    HBITMAP hBitmap;
    Image* image;
    void show(HDC hdc, const RECT& rc)
    {

        Graphics g(hdc);
        g.SetPageUnit(UnitPixel);
        Image im(L"back.bmp");
        float vx = (x - player_view.x) * scale + rc.right / 2;
        float vy = (y - player_view.y) * scale + rc.bottom / 2;
        float vw = width * scale;
        float vh = height * scale;

        bool in = false;

        if (vx + vw >= 0 && vx < rc.right &&
            vy + vh >= 0 && vy < rc.bottom)
            in = true;

        if (!in) return;
        //ShowBitmap(window.context, vx, vy, vw, vh, hBitmap, false);
        g.DrawImage(&im, vx, vy, vw, vh);
    }
};

class StaticObjects
{
public:
    struct Sprite {
        float x;
        float y;
        float width;
        float height;
        Image* image;  // Указатель на изображение GDI+
    } Sprite;

    StaticObjects(float p_x, float p_y, float p_width, float p_height, HDC hdc, const RECT& rc, const wstring& filename)
    {
        // Инициализация позиции и размеров
        Sprite.x = p_x * rc.right;
        Sprite.y = p_y * rc.bottom;
        Sprite.width = p_width * rc.right;
        Sprite.height = p_height * rc.bottom;

        // Загрузка изображения
        Sprite.image = new Image(filename.c_str());

        // Проверка успешности загрузки
        if (Sprite.image->GetLastStatus() != Ok)
        {
            // Обработка ошибки загрузки
            delete Sprite.image;
            Sprite.image = nullptr;
            MessageBox(NULL, L"Не удалось загрузить изображение", L"Ошибка", MB_ICONERROR);
        }
    }

    ~StaticObjects()
    {
        // Не забываем освободить память
        if (Sprite.image)
            delete Sprite.image;
    }

    /*void Draw(HDC hdc)
    {
        if (Sprite.image)
        {
            Graphics graphics(hdc);
            graphics.DrawImage(Sprite.image,
                RectF(Sprite.x, Sprite.y, Sprite.width, Sprite.height));
        }
    }*/

    bool CheckCollision(float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2)
    {
        return x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2;
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
    portal_(float p_x, float p_y, float p_width, float p_height, HDC hdc, const RECT& rc, const wstring& filename, int p_destination)
        : StaticObjects(p_x, p_y, p_width, p_height, hdc, rc, filename)
    {
        destination = p_destination;
    }
    void Portal(auto& player);
};

struct Location_
{
    sprite hBack;
    vector<portal_>portal;
    vector<Platform> walls;
    vector<HealingFlask> healingFlask;
    vector<Spike> spike;
   //vector<character*> Persona;

};

Location_ location[5];