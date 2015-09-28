#ifndef ACT_SWIPE_H
#define ACT_SWIPE_H

#include "Activation.h"
#include "Matter.h"
#include "FullbrightMaterial.h"

class ActSwipe : public Activation
{
public:

    ActSwipe();
    ~ActSwipe();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    int m_nTargetX;
    int m_nTargetZ;

    Matter*             m_pMatter;
    FullbrightMaterial* m_pMaterial;
    int m_nHit;
};

#endif