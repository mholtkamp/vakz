#ifndef MAGE_H
#define MAGE_H

#include "Scene.h"
#include "Matter.h"

class Pawn
{
public:

    Pawn(int nPlayerNum);
    virtual ~Pawn();

    virtual void Register(Scene* pScene);

    virtual void SetPosition(int nX,
                             int nZ);

    virtual void Damage(int nDamage);
    virtual void Heal(int nHealth);

    virtual void SetVisible(int nVisible);
    virtual void SetSpeed(float fSpeed);
    
protected:

    Matter m_matter;

    int m_nX;
    int m_nZ;

    int m_nVisible;
    int m_nHealth;
    float m_fSpeed;
};

#endif