#ifndef ACT_HEAL_H
#define ACT_HEAL_H

#include "Activation.h"
#include "Matter.h"
#include "FullbrightMaterial.h"

class ActHeal : public Activation
{
public:

    ActHeal();
    ~ActHeal();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    float m_fPosX;
    float m_fPosY;
    float m_fPosZ;

    Matter*   m_pMatter;
    FullbrightMaterial* m_pMaterial;
};

#endif