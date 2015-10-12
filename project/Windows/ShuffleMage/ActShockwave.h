#ifndef ACT_SHOCKWAVE_H
#define ACT_SHOCKWAVE_H

#include "Activation.h"
#include "Matter.h"
#include "Material.h"

class ActShockwave : public Activation
{
public:

    ActShockwave();
    ~ActShockwave();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    int m_nCastX;
    int m_nCastZ;

    int m_arTarget[2];
    int m_arHit[2];

private:

    Timer            m_arWaveTimers;
    DiffuseMaterial* m_arMaterials[2];
    Matter*          m_arMatter[2];

};

#endif