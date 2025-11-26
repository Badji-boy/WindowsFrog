#pragma once
#include "windows.h"
#include "math.h"
#include "ctime"
#include "vector"
#include <thread>
#include <limits>
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

namespace Cube3D
{
    struct Point3D  //структура вершин
    {
        float x, y, z;
        Point3D(float x, float y, float z) :x(x), y(y), z(z) {}
    };

    struct Point2D
    {
        int x, y;
        Point2D(int x = 0, int y = 0) : x(x), y(y) {}
    };

    struct Matrix4x4
    {
        float m[4][4] = { 0 };
    };

    const int SCREEN_WIDTH = window.width;
    const int SCREEN_HEIGHT = window.height;
    const float CAMERA_DIST = 500.0f;

    vector<vector<float>> zBuffer;
    bool zBufferInitialized = false;
    #undef max
    // Инициализация Z-буфера
    void InitializeZBuffer(int width = window.width, int height = window.height)
    {
        zBuffer.resize(height, vector<float>(width, std::numeric_limits<float>::max()));
        zBufferInitialized = true;
    }

    // Очистка Z-буфера (вызывать каждый кадр)
    void ClearZBuffer()
    {
        if (!zBufferInitialized) return;

        for (int y = 0; y < window.height; y++) {
            for (int x = 0; x < window.width; x++) {
                zBuffer[y][x] = std::numeric_limits<float>::max();
            }
        }
    }

    // Функция для проверки и обновления z-буфера
    bool UpdateZBuffer(int x, int y, float z)
    {
        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
            return false;

        if (z < zBuffer[y][x]) {
            zBuffer[y][x] = z;
            return true;
        }
        return false;
    }

    Matrix4x4 TurnX(float angle) //матрица поворота для х
    {
        Matrix4x4 m;
        m.m[0][0] = 1.0f;
        m.m[1][1] = cos(angle);
        m.m[1][2] = sin(angle);
        m.m[2][1] = -sin(angle);
        m.m[2][2] = cos(angle);
        m.m[3][3] = 1.0f;
        return m;
    };

    Matrix4x4 TurnY(float angle) //матрица поворота для у
    {
        Matrix4x4 m;
        m.m[0][0] = cos(angle);
        m.m[0][2] = -sin(angle);
        m.m[1][1] = 1.0f;
        m.m[2][0] = sin(angle);
        m.m[2][2] = cos(angle);
        m.m[3][3] = 1.0f;
        return m;
    };

    Matrix4x4 TurnZ(float angle) //матрица поворота для z
    {
        Matrix4x4 m;
        m.m[0][0] = cos(angle);
        m.m[0][1] = sin(angle);
        m.m[1][0] = -sin(angle);
        m.m[1][1] = cos(angle);
        m.m[2][2] = 1.0f;
        m.m[3][3] = 1.0f;
        return m;
    };

    vector <Point3D> StartPoints = {
        Point3D(-200,-200,10), //0 нижняя левая
        Point3D(-200,200,10), //1 верхняя левая
        Point3D(200,200,10), //2 верхняя правая
        Point3D(200,-200,10), //3 нижняя правая
        //далее задняя грань
        Point3D(-200,-200,-200), //4 нижняя левая
        Point3D(-200,200,-200), //5 верхняя левая
        Point3D(200,200,-200), //6 верхняя правая
        Point3D(200,-200,-200) //7 нижняя правая
    };

    vector<pair<int, int>> cubeEdges = {
        // Передняя грань
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        // Задняя грань
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        // Соединяющие ребра
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    vector<vector<int>> cubeFaces = {
            {0, 1, 2, 3}, // передняя грань
            {4, 5, 6, 7}, // задняя грань
            {0, 1, 5, 4}, // левая грань
            {2, 3, 7, 6}, // правая грань
            {0, 3, 7, 4}, // нижняя грань
            {1, 2, 6, 5}  // верхняя грань
    };

    Matrix4x4 MultiplyMatrices(const Matrix4x4& a, const Matrix4x4& b)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return result;
    }

    Point3D MultiplyPointMatrix(const Point3D& point, const Matrix4x4& matrix)
    {
        Point3D result(0, 0, 0);

        result.x = point.x * matrix.m[0][0] + point.y * matrix.m[1][0] + point.z * matrix.m[2][0];
        result.y = point.x * matrix.m[0][1] + point.y * matrix.m[1][1] + point.z * matrix.m[2][1];
        result.z = point.x * matrix.m[0][2] + point.y * matrix.m[1][2] + point.z * matrix.m[2][2];

        return result;
    }

    Point3D ProjectPoint(const Point3D& point)
    {
        Point3D projected(0, 0, 0);

        if (point.z + CAMERA_DIST != 0) {
            projected.x = SCREEN_WIDTH / 2. + point.x * CAMERA_DIST / (point.z + CAMERA_DIST);
            projected.y = SCREEN_HEIGHT / 2. - point.y * CAMERA_DIST / (point.z + CAMERA_DIST);
        }
        else {
            projected.x = SCREEN_WIDTH / 2.;
            projected.y = SCREEN_HEIGHT / 2.;
        }
        projected.z = point.z;

        return projected;
    }

    // Функция интерполяции z-координаты в треугольнике
    float InterpolateZ(const Point3D& p1, const Point3D& p2, const Point3D& p3, int px, int py)
    {
        // Вычисляем барицентрические координаты
        float denom = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
        if (fabs(denom) < 1e-6) return p1.z; // избегаем деления на ноль

        float lambda1 = ((p2.y - p3.y) * (px - p3.x) + (p3.x - p2.x) * (py - p3.y)) / denom;
        float lambda2 = ((p3.y - p1.y) * (px - p3.x) + (p1.x - p3.x) * (py - p3.y)) / denom;
        float lambda3 = 1.0f - lambda1 - lambda2;

        // Интерполируем z-координату
        return lambda1 * p1.z + lambda2 * p2.z + lambda3 * p3.z;
    }

    void DrawLine(int x1, int y1, int x2, int y2)
    {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        int x = x1;
        int y = y1;

        while (true) {
            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                SetPixel(window.context, x, y, RGB(255, 255, 255));
            }

            if (x == x2 && y == y2)
                break;

            int err2 = err * 2;

            if (err2 > -dy) {
                err -= dy;
                x += sx;
            }

            if (err2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    void DrawCube(const vector <Point3D>& point)
    {
        vector <Point3D> projectedPoints;
        for (const auto& p : point)
        {
            projectedPoints.push_back(ProjectPoint(p));
        }

        for (const auto& edge : cubeEdges)
        {
            int startIdx = edge.first;
            int endIdx = edge.second;
            DrawLine(
                projectedPoints[startIdx].x,
                projectedPoints[startIdx].y,
                projectedPoints[endIdx].x,
                projectedPoints[endIdx].y
            );
        }
    }

    void DrawFilledTriangleWithZBuffer(int x1, int y1, float z1,
        int x2, int y2, float z2,
        int x3, int y3, float z3,
        COLORREF color)
    {
        // Находим ограничивающий прямоугольник треугольника
        int minX = min(x1, min(x2, x3));
        int maxX = max(x1, max(x2, x3));
        int minY = min(y1, min(y2, y3));
        int maxY = max(y1, max(y2, y3));

        // Ограничиваем область экраном
        minX = max(0, minX);
        maxX = min(SCREEN_WIDTH - 1, maxX);
        minY = max(0, minY);
        maxY = min(SCREEN_HEIGHT - 1, maxY);

        // Создаем 3D точки для интерполяции
        Point3D p1(x1, y1, z1);
        Point3D p2(x2, y2, z2);
        Point3D p3(x3, y3, z3);

        // Проходим по каждому пикселю в ограничивающем прямоугольнике
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                // Вычисляем барицентрические координаты
                float denom = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
                if (fabs(denom) < 1e-6) continue;

                float lambda1 = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / denom;
                float lambda2 = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / denom;
                float lambda3 = 1.0f - lambda1 - lambda2;

                // Проверяем, находится ли точка внутри треугольника
                if (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0) {
                    // Интерполируем z-координату
                    float z = lambda1 * p1.z + lambda2 * p2.z + lambda3 * p3.z;

                    // Обновляем z-буфер и рисуем пиксель
                    if (UpdateZBuffer(x, y, z)) {
                        SetPixel(window.context, x, y, color);
                    }
                }
            }
        }
    
    }

    void DrawFilledTriangle3D(const Point3D& p1, const Point3D& p2, const Point3D& p3, COLORREF color)
    {
        // Проецируем 3D точки на экран
        Point3D proj1 = ProjectPoint(p1);
        Point3D proj2 = ProjectPoint(p2);
        Point3D proj3 = ProjectPoint(p3);

        // Рисуем залитый треугольник с z-буферизацией
        DrawFilledTriangleWithZBuffer(
            static_cast<int>(proj1.x), static_cast<int>(proj1.y), proj1.z,
            static_cast<int>(proj2.x), static_cast<int>(proj2.y), proj2.z,
            static_cast<int>(proj3.x), static_cast<int>(proj3.y), proj3.z,
            color
        );
    }

    void DrawFilledCube(const vector<Point3D>& points)
    {
        vector<COLORREF> colors = {
            RGB(255, 0, 0),   // красный - передняя грань
            RGB(0, 255, 0),   // зеленый - задняя грань  
            RGB(0, 0, 255),   // синий - левая грань
            RGB(255, 255, 0), // желтый - правая грань
            RGB(255, 0, 255), // пурпурный - нижняя грань
            RGB(0, 255, 255)  // голубой - верхняя грань
        };

        int colorIndex = 0;

        // Для каждой грани рисуем 2 треугольника
        for (const auto& face : cubeFaces) {
            COLORREF color = colors[colorIndex];

            // Первый треугольник грани
            DrawFilledTriangle3D(
                points[face[0]], points[face[1]], points[face[2]], color
            );
            // Второй треугольник грани
            DrawFilledTriangle3D(
                points[face[0]], points[face[2]], points[face[3]], color
            );

            colorIndex++;
        }
    }

    void DrawRotatingCube()
    {
        // Инициализируем z-буфер если еще не инициализирован
        if (!zBufferInitialized) {
            InitializeZBuffer();
        }

        ClearZBuffer();

        static float angleX = 0;
        static float angleY = 0;
        static float angleZ = 0;

        Matrix4x4 rotX = TurnX(angleX);
        Matrix4x4 rotY = TurnY(angleY);
        Matrix4x4 rotZ = TurnZ(angleZ);

        Matrix4x4 rotation = MultiplyMatrices(rotZ, MultiplyMatrices(rotY, rotX));
        vector <Point3D> transformedPoints;

        // Применяем преобразования к исходным точкам
        for (const auto& point : StartPoints) {
            Point3D transformed = MultiplyPointMatrix(point, rotation);
            transformedPoints.push_back(transformed);
        }

        // Рисуем куб с z-буферизацией
        DrawFilledCube(transformedPoints);

        // Обновляем углы для анимации
        angleX += 0.05f;
        angleY += 0.05f;
        angleZ += 0.05f;

        // Ограничиваем углы чтобы избежать переполнения
        if (angleX > 2 * 3.14159f) angleX -= 2 * 3.14159f;
        if (angleY > 2 * 3.14159f) angleY -= 2 * 3.14159f;
        if (angleZ > 2 * 3.14159f) angleZ -= 2 * 3.14159f;
    }
}


