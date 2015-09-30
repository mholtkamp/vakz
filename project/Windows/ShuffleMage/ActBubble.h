#ifndef ACT_BUBBLE_H
#define ACT_BUBBLE_H

#include "Activation.h"
#include "Matter.h"
#include "DiffuseMaterial.h"

class ActBubble : public Activation
{
public:

    ActBubble();
    ~ActBubble();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    float m_fPosX;
    float m_fPosY;
    float m_fPosZ;

    int m_nHit;
    int m_nTargetX;
    int m_nTargetZ;

private:

    void DestroyPreviousHits();

    Matter*          m_pMatter;
    DiffuseMaterial* m_pMaterial;

};

#endif