#pragma once
#include "ECSSoft.h"
#include "APPGame.h"
#include "GameFileSystem.h"
using namespace ECC;

void LoadTransform(CTransform& CTransform, float arr[])
{
    CTransform.x = arr[0];
    CTransform.y = arr[1];
    CTransform.Width = arr[2];
    CTransform.Height = arr[3];
}

void CreateObject(CTransform& CTransform)
{
    CTransform.x *= window.width;
    CTransform.y *= window.height;
    CTransform.Width *= window.width;
    CTransform.Height *= window.height;
}

bool CheckCollision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}

void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump)
{
    CCollider.LastTracePlatformNum = -1;
    bool CollXfound = false;
    bool CollYfound = false;

    float Lenght = sqrt(pow(Transform.Dx, 2) + pow(Transform.Dy, 2));
    for (float i = 0; i < Lenght; i++)
    {
        if (CollXfound && CollYfound) return;

        for (int k = 0; k < 4; k++)
        {

            for (int j = 0; j < VLocation[Player->GetLocation()].VWall.size(); j++)
            {


                float Bbox[] = {
                    Transform.x + Transform.Dx * i / Lenght, Transform.y + Transform.Dy * i / Lenght,
                    Transform.x + Transform.Width + Transform.Dx * i / Lenght - 1, Transform.y + Transform.Dy * i / Lenght,
                    Transform.x + Transform.Width + Transform.Dx * i / Lenght - 1, Transform.y + Transform.Height + Transform.Dy * i / Lenght - 1,
                    Transform.x + Transform.Dx * i / Lenght, Transform.y + Transform.Height + Transform.Dy * i / Lenght - 1
                };
                float pixel_x = Bbox[k * 2];
                float pixel_y = Bbox[k * 2 + 1];
                SetPixel(window.context, (pixel_x - player_view.x) * 2 + window.width / 2, (pixel_y - player_view.y) * 2 + window.height / 2, RGB(255, 255, 255));

                auto walls = VLocation[Player->GetLocation()].VWall[j].GetPosition();
                if ((pixel_x >= walls->x &&
                    pixel_x <= walls->x + walls->Width) &&
                    (pixel_y >= walls->y &&
                        pixel_y <= walls->y + walls->Height)
                    )
                {
                    float top = pixel_y - walls->y;
                    float down = (walls->y + walls->Height) - pixel_y;
                    float left = pixel_x - walls->x;
                    float right = (walls->x + walls->Width) - pixel_x;

                    float minX = min(left, right);
                    float minY = min(top, down);
                    CJump.InJump = false;

                    if (minX < minY && !CollXfound)
                    {
                        Transform.Dx = 0;
                        CollXfound = true;

                        if (left < right)
                        {
                            Transform.x = walls->x - Transform.Width - 1;
                        }
                        else
                        {
                            Transform.x = walls->x + walls->Width + 1;
                        }

                        j++;
                    }

                    if (minX >= minY && !CollYfound)
                    {
                        Transform.Dy = 0;
                        CollYfound = true;

                        if (down < top)
                        {
                            Transform.y = walls->y + walls->Height + 1;
                            CJump.Jump = 30;
                        }
                        else
                        {
                            CCollider.LastTracePlatformNum = j;
                            Transform.y = walls->y - Transform.Height - 1;
                            CJump.InJumpBot = false;
                        }
                        j++;
                    }
                }

            }
        }
    }
    if (!CollXfound) Transform.x += Transform.Dx;
    if (!CollYfound) Transform.y += Transform.Dy;
}

void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity)
{
    CJump.Jump *= .9;
    Transform.Dx *= .1;
    Transform.Dy = Gravity.Gravity - CJump.Jump;
}

void ProcessSound(CSound& CSound)
{
    PlaySound(TEXT(CSound.SoundNameFile), NULL, SND_FILENAME | SND_ASYNC);
}

void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity)
{
    CSpeed.SpeedWalk = 12;
   
    if (GetAsyncKeyState(VK_LEFT))
    {
        
        Transform.Dx = -CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_RIGHT))
    {
       
        Transform.Dx = CSpeed.SpeedWalk;
    }
    if (GetAsyncKeyState(VK_SPACE) && CJump.InJump == false && CJump.InJumpBot == false)
    {
        CJump.Jump = 110;
        CJump.InJumpBot = true;
        CJump.InJump = true;
    }
    
}

void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity)
{
    CSpeed.SpeedWalk = 6;

    for (int i = 0; i < VLocation.size(); i++)
    {
        if (Player->GetLocation() == i)
        {
            for (int j = 0; j < VLocation[i].VWall.size();j++)
            {
                if (CCollider.LastTracePlatformNum >= 0)
                {
                    auto& platform = *VLocation[i].VWall[CCollider.LastTracePlatformNum].GetPosition();
                    if (CTransform.x <= platform.x)
                    {
                        CCollider.Direction = 1;
                    }
                    if (CTransform.x + CTransform.Width >= platform.x + platform.Width)
                    {
                        CCollider.Direction = -1;
                    }
                }
            }
        }
        CTransform.Dx = CCollider.Direction * CSpeed.SpeedWalk;
    }
}

void AppGame::Init()
{
    LoadSVGDataMap(L"LVLDemoDay0");
    LoadSVGDataMap(L"LVLDemoDay1");
}

void AppGame::UpdateApp(MSG* msg)
{
    if (GetAsyncKeyState(VK_ESCAPE))
    {
        msg->message = WM_QUIT;
    }
}
void AppGame::Render(Graphics& g, Font& font, SolidBrush& solidBrush, Pen& blackPen)
{
    location[player->currentLocation].hBack.showBack(g);
    player->Sprite.show(g);

    for (int i = 0; i < location[player->currentLocation].Persona.size(); i++) {
        location[player->currentLocation].Persona[i]->dialog(player);
        location[player->currentLocation].Persona[i]->Sprite.show(g);
        location[player->currentLocation].Persona[i]->move();
    }
    //Health_bar.Show();

    for (int i = 0; i < location[player->currentLocation].walls.size(); i++) {
        location[player->currentLocation].walls[i].Sprite.show(g);
    }
    for (int i = 0; i < location[player->currentLocation].healingFlask.size(); i++) {
        location[player->currentLocation].healingFlask[i].Sprite.show(g);
        location[player->currentLocation].healingFlask[i].healing(player, i);
    }
    for (int i = 0; i < location[player->currentLocation].spike.size(); i++) {
        location[player->currentLocation].spike[i].Sprite.show(g);
        location[player->currentLocation].spike[i].damage(player);
    }
    for (int i = 0; i < location[player->currentLocation].portal.size(); i++) {
        location[player->currentLocation].portal[i].Sprite.show(g);
        location[player->currentLocation].portal[i].Portal(player);
    }
    float ls = .2 * length(player_view.x, player->Sprite.x, player_view.y, player->Sprite.y) / 500.;
    ls = max(ls - .2, 0.1);
    ls = min(ls, 1);

    float cameraHalfWidth = (window.width / 2.) / scale;
    float cameraHalfHeight = (window.height / 2.) / scale;

    float targetX = player->Sprite.x;
    float targetY = player->Sprite.y;

    targetX = max(0 + cameraHalfWidth,
        min(window.width - cameraHalfWidth, targetX));
    targetY = max(0 + cameraHalfHeight,
        min(window.height - cameraHalfHeight, targetY));

    player_view.x = lerp(player_view.x, targetX, 0.1f);
    player_view.y = lerp(player_view.y, targetY, 0.1f);

    if (startDialog == false)
    {
        player->move();
        endDialog = false;
        // Сброс состояния диалога при выходе
        dialogState = 0;
        keyProcessed = false;
    }
    else if (startDialog == true)
    {
        float txtX = 0;
        float txtY = window.height - (window.height / 4.);

        location[player->currentLocation].Persona[name]->DialogSprite.showDialog(g);
        PointF txtBounds(txtX, txtY);

        // Обработка нажатий клавиш (только когда клавиша отпущена после нажатия)
        bool key1Pressed = (GetAsyncKeyState('1') & 0x8000) != 0;
        bool key2Pressed = (GetAsyncKeyState('2') & 0x8000) != 0;
        bool key3Pressed = (GetAsyncKeyState('3') & 0x8000) != 0;

        // Если клавиши отпущены, сбрасываем флаг обработки
        if (!key1Pressed && !key2Pressed && !key3Pressed) {
            keyProcessed = false;
        }

        // Обрабатываем нажатия только если они еще не обработаны
        if (!keyProcessed) {
            if (key1Pressed) {
                dialogState = 1;
                keyProcessed = true;
            }
            else if (key2Pressed) {
                dialogState = 2;
                keyProcessed = true;
            }
            else if (key3Pressed) {
                endDialog = true;
                startDialog = false;
                keyProcessed = true;
            }
        }

        // Отображение текста в зависимости от состояния
        switch (dialogState) {
        case 0:
            g.DrawString(L"Как ваше настроение, сэр Выберите вариант ответа:\n1 - Да ниче так \n2 - Все плохо, сэр\n3 - Выйти", -1, &font, txtBounds, &solidBrush);
            break;
        case 1:
            g.DrawString(L"Отрадно слышать.", -1, &font, txtBounds, &solidBrush);
            break;
        case 2:
            g.DrawString(L"Печально сэр, очень печально.", -1, &font, txtBounds, &solidBrush);
            break;
        }
    }

    GetCursorPos(&mouse);
    ScreenToClient(win.GetHWND(), &mouse);
    BitBlt(hdc, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
}


