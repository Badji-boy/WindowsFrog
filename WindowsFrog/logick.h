#pragma once
#include "Structures.h"
#include <memory>
#include "GameFileSystem.h"


//void InitGame()
//{
//   // LoadSVGDataMap("LVL0");
//
//    //player = new Hero(0.2, 0.25, 0.023, 0.032, "racket", 40, 5, 3, ,0);
//    //wolf = new Wolf(0.25, 0.25, 0.023, 0.02, "enemy1", 40, 5, 3, 0);
//    //wolf2 = new Wolf(0.15, 0.25, 0.023, 0.02, "enemy1", 40, 5, 3, 0);
//
//    //-----------------------------location0_______________
//    //location[0].hBack.loadBitmapWithNativeSize("background_0");
//    location[0].walls.emplace_back(0, 0, 0.009, 0.99, "walls");//левая стена
//    location[0].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls");//правая стена
//    location[0].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls");//пол
//    location[0].walls.emplace_back(0, 0, 0.999, 0.04, "walls");//потолок
//    location[0].portal.emplace_back(0.96, 0.89, 0.021, 0.2, hdc, rc, 1, "racket");//портал в локацию 1
//    location[0].healingFlask.emplace_back(0.3, 0.955, 0.02, 0.025, "ball");
//    location[0].healingFlask.emplace_back(0.4, 0.955, 0.02, 0.025, "ball");
//    location[0].spike.emplace_back(0.5, 0.955, 0.025, 0.025, "spike");
//
//    location[0].walls.emplace_back(0.1, 0.8, 0.15, 0.05, "walls");
//    location[0].walls.emplace_back(0.05, 0.9, 0.15, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.7, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.6, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.5, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.3, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.2, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.1, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.1, 0.4, 0.05, 0.05, "walls");
//    location[0].walls.emplace_back(0.3, 0.85, 0.15, 0.05, "walls");
//    location[0].walls.emplace_back(0.6, 0.93, 0.15, 0.05, "walls");
//    location[0].walls.emplace_back(0.8, 0.83, 0.15, 0.05, "walls");
//    //-----------------------------------------------------------------------------
//    //LoadSVGDataMap("LVL0");
//    //___________________________location1________________
//
//    //location[1].hBack.loadBitmapWithNativeSize("background_1");
//    //location[1].walls.emplace_back(0, 0, 0.02, 0.99, "walls");//левая стена
//    //location[1].walls.emplace_back(0.98, 0, 0.02, 0.99, "walls");//правая стена
//    //location[1].walls.emplace_back(0, 0.98, 0.999, 0.02, "walls");//пол
//    //location[1].walls.emplace_back(0, 0, 0.999, 0.02, "walls");//потолок
//    //location[1].portal.emplace_back(0.8, 0.89, 0.021, 0.09, 0, "racket");//портал в локацию 0
//
//}
//
////void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
////{
////    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
////}
//
//
//
//void tracer_collide(auto& Character)
//{
//    Character.last_trace_platform_num = -1;
//    sprite& Sprite = Character.Sprite;
//
//    bool coll_x_found = false;
//    bool coll_y_found = false;
//
//    float lenght = sqrt(pow(Sprite.dx, 2) + pow(Sprite.dy, 2));
//    for (float i = 0; i < lenght; i++)
//    {
//        if (coll_x_found && coll_y_found) return;
//
//        for (int k = 0; k < 4; k++)
//        {
//            //if (coll_x_found|| coll_y_found) break;
//
//
//            for (int j = 0; j < location[Character.currentLocation].walls.size(); j++)
//            {
//                float Bbox[] = {
//                    Sprite.x + Sprite.dx * i / lenght, Sprite.y + Sprite.dy * i / lenght,
//                    Sprite.x + Sprite.width + Sprite.dx * i / lenght - 1, Sprite.y + Sprite.dy * i / lenght,
//                    Sprite.x + Sprite.width + Sprite.dx * i / lenght - 1, Sprite.y + Sprite.height + Sprite.dy * i / lenght - 1,
//                    Sprite.x + Sprite.dx * i / lenght, Sprite.y + Sprite.height + Sprite.dy * i / lenght - 1
//                };
//
//                float pixel_x = Bbox[k * 2];
//                float pixel_y = Bbox[k * 2 + 1];
//
//                SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));
//
//
//
//                auto walls = location[Character.currentLocation].walls[j].Sprite;
//                if ((pixel_x >= walls.x &&
//                    pixel_x <= walls.x + walls.width) &&
//                    (pixel_y >= walls.y &&
//                        pixel_y <= walls.y + walls.height)
//                    )
//                {
//                    float top = pixel_y - walls.y;
//                    float down = (walls.y + walls.height) - pixel_y;
//                    float left = pixel_x - walls.x;
//                    float right = (walls.x + walls.width) - pixel_x;
//
//                    float minX = min(left, right);
//                    float minY = min(top, down);
//                    Character.inJump = false;
//
//                    if (minX < minY && !coll_x_found)
//                    {
//                        Sprite.dx = 0;
//                        coll_x_found = true;
//
//                        if (left < right)
//                        {
//                            Sprite.x = walls.x - Sprite.width - 1;
//                        }
//                        else
//                        {
//                            Sprite.x = walls.x + walls.width + 1;
//                        }
//
//                        j++;
//                    }
//
//                    if (minX >= minY && !coll_y_found)
//                    {
//                        Sprite.dy = 0;
//                        coll_y_found = true;
//
//                        if (down < top)
//                        {
//                            Sprite.y = walls.y + walls.height + 1;
//                            Sprite.jump = 30;
//                        }
//                        else
//                        {
//                            Character.last_trace_platform_num = j;
//                            Sprite.y = walls.y - Sprite.height - 1;
//                            Character.inJumpBot = false;
//                        }
//
//                        j++;
//                    }
//
//                }
//
//            }
//        }
//
//    }
//
//    if (!coll_x_found) Sprite.x += Sprite.dx;
//    if (!coll_y_found) Sprite.y += Sprite.dy;
//}
//
//HANDLE hTimer;
//void TweenDash(int idx)
//{
//    WaitForSingleObject(hTimer, INFINITE);
//
//    if (Sprite->dash_allow == true)
//        if (Sprite->racket.vect_right == true)
//        {
//            Sprite->racket.x += idx;
//        }
//        else
//        {
//            Sprite->racket.x -= idx;
//        }
//
//
//}

//void ProcessDash()
//{
//    bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
//    if (isShiftPressed && !Sprite->wasShiftPressed)
//    {
//        Sprite->wasShiftPressed = true;
//
//        hTimer = CreateWaitableTimer(NULL, false, NULL);
//        LARGE_INTEGER dTime;
//        for (int i = 0; i < Sprite->dashDistance; i++)
//        {
//
//            thread t(TweenDash, i);
//            t.detach();
//
//        }
//        dTime.QuadPart = -10000 * 7;
//        SetWaitableTimer(hTimer, &dTime, 7, NULL, NULL, false);
//
//    }
//    else if (!isShiftPressed)
//    {
//        Sprite->wasShiftPressed = false;
//        Sprite->dash_allow = true;
//    }
//
//
//}
