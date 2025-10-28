#pragma once
#include "Component.h"
using namespace ECC;
//void SpikeEvent(CTransform& Transform);
void LoadTransform(CTransform& CTransform, float arr[]);
void CreateObject(CTransform& CTransform);
void TracerCollide(CCollider& CCollider, CTransform& Transform, CJump& CJump);
void ProcessGravity(CJump& CJump, CTransform& Transform, CGravity& Gravity);
//void ProcessSound(CSound& CSound);
void MovePlayer(CJump& CJump, CTransform& Transform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);
void MoveCharacter(CJump& CJump, CTransform& CTransform, CSpeed& CSpeed, CCollider& CCollider, CGravity& Gravity);




//áàçîâûå êëàññû
class BaseArcheType
{
protected:
    int Entity = ECS.NewEntity();
    CTransform* Transform = ECS.SetComponent<CTransform>(Entity);
    CTexture* Textures = ECS.SetComponent<CTexture>(Entity);
    CSound* Sound = ECS.SetComponent<CSound>(Entity);
    CNameObject* NameObject = ECS.SetComponent<CNameObject>(Entity);
   
    int WhatLocation;

    BaseArcheType(string NameFile, float arr[])
    {
        LoadTransform(*Transform, arr);
        Textures->Texture = NameFile;
        CreateObject(*Transform);
        NameObject->Number = Entity;
    }

    void DeleteAT()
    {
        ECS.DeleteEntity(Entity);
        /*delete this;*/ //êðàøèò ïðîãó 
    }

public:
    virtual void Destroy()
    {
        DeleteAT();
    };
    void SetLocation(int IdLocation)
    {
        WhatLocation = IdLocation;
    }
    int GetLocation()
    {
        return WhatLocation;
    }
    CTransform* GetPosition()
    {
        return ECS.GetComponent<CTransform>(Entity, Transform);
    }
    CTexture* GetTexture()
    {
        return ECS.GetComponent<CTexture>(Entity, Textures);
    }
    
    CNameObject* GetNameObj()
    {
        return ECS.GetComponent<CNameObject>(Entity, NameObject);
    }
};

class BasePerson : public BaseArcheType
{
protected:
    CSpeed* Speed = ECS.SetComponent<CSpeed>(Entity);
    CJump* Jump = ECS.SetComponent<CJump>(Entity);
    CGravity* Gravity = ECS.SetComponent<CGravity>(Entity);
    CCollider* Collider = ECS.SetComponent<CCollider>(Entity);

    BasePerson(string NameFile, float arr[]) : BaseArcheType(NameFile, arr) {}
public:
    CJump* GetJump()
    {
        return ECS.GetComponent<CJump>(Entity, Jump);
    }
    CSpeed* GetSpeed()
    {
        return ECS.GetComponent<CSpeed>(Entity, Speed);
    }
};

//íàñëåäíèêè
class ATWall : public BaseArcheType
{
public:
    ATWall(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
    {
        NameObject->Name = "Wall";
    }
}*Wall;




class ATEnemy : public BasePerson
{
public:
  
    void Start()
    {
        MoveCharacter(*Jump, *Transform, *Speed, *Collider, *Gravity);
        TracerCollide(*Collider, *Transform, *Jump);
        ProcessGravity(*Jump, *Transform, *Gravity);
    }
}*Enemy;

class ATPlayer : public BasePerson
{
public:
    void Start()
    {
        MovePlayer(*Jump, *Transform, *Speed, *Collider, *Gravity);
        TracerCollide(*Collider, *Transform, *Jump);
        ProcessGravity(*Jump, *Transform, *Gravity);
    }

}*Player;

//êîíåö âñåõ àðõåòèïîâ
class ATLocation : public BaseArcheType
{
public:
    ATLocation(string NameFile, float arr[]) : BaseArcheType(NameFile, arr)
    {
        NameObject->Name = "Level";
    }
    vector<ATWall> VWall;
    vector<ATEnemy> VEnemy;

}*Location;
vector<ATLocation> VLocation; 