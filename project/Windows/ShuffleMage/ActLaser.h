#ifndef ACT_LASER_H
#define ACT_LASER_H

#include "Activation.h"
#include "DiffuseMaterial.h"

class ActLaser : public Activation
{
public:

    ActLaser();
    ~ActLaser();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();


    int m_nHit;
    DiffuseMaterial* m_pMaterial;
};

#endif