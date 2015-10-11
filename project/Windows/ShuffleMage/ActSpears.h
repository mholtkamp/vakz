#ifndef ACT_SPEARS_H
#define ACT_SPEARS_H

#include "Activation.h"
#include "Matter.h"
#include "DiffuseMaterial.h"

class ActSpears : public Activation
{
public:

    ActSpears();
    ~ActSpears();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    float m_fPosX;
    float m_fSpawnX;

    int m_nHit;

private:

    void CheckHit();

    Matter*          m_arMatters[3];
    DiffuseMaterial* m_pMaterial;

};


#endif