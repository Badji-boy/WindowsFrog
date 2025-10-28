#pragma once
#include "Structures.h"
    //базовые компоненты
namespace ECC
{
    struct CTransform
    {
        float x, y, Width, Height, Dx = 0., Dy = 0.;
        int Scale = 2;
    };
    struct CTexture
    {
        string Texture;
    };

    struct CGravity
    {
        float Gravity = 30;
    };

    struct CSound
    {
        const char* SoundNameFile;
    };

    struct CCollider
    {
        int LastTracePlatformNum = -1;
        int Direction = 1;
    };

    struct CNameObject
    {
        const char* Name;
        int Number;
    };

    //Character Components
    struct CSpeed
    {
        int SpeedWalk = 15;
    };

    struct CJump
    {
        float Jump = 0.;
        bool InJumpBot = false;
        bool InJump = false;
    };
}
