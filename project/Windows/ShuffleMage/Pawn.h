#ifndef PAWN_H
#define PAWN_H

#include "Scene.h"
#include "Matter.h"
#include "Tile.h"

class Pawn
{
public:

    Pawn();
    Pawn(int nSide);
    virtual ~Pawn();

    void Construct();

    virtual void Register(Scene* pScene);

    virtual void SetPosition(int nX,
                             int nZ);

    virtual void Move(int nX,
                      int nZ);

    virtual void Damage(int nDamage);
    virtual void Heal(int nHealth);

    virtual void SetVisible(int nVisible);
    virtual void SetSpeed(float fSpeed);

    void SetSide(int nSide);

    void SetGame(void* pGame);
    
protected:

#if defined(SM_CLIENT)
    Matter m_matter;
#endif

    int m_nSide;

    int m_nX;
    int m_nZ;

    int m_nVisible;
    int m_nHealth;
    float m_fSpeed;

    void* m_pGame;
};

#endif