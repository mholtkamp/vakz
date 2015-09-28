#ifndef ACT_AREAGRAB_H
#define ACT_AREAGRAB_H

#include "Activation.h"
#include "Matter.h"
#include "FullbrightMaterial.h"

class ActAreaGrab : public Activation
{
public:
    ActAreaGrab();
    ~ActAreaGrab();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    int   m_arTargetX[3];
    int   m_arTargetZ[3];
    float m_arPosY[3];
    float m_arWaveScale[3];
    float m_arWaveAlpha[3];

    float m_arColor[4];
    int   m_arHit[3];
    float m_arHitTimes[3];
    

private:

    Timer m_timerDelta;
    Matter* m_arMatters[3];
    FullbrightMaterial* m_arMaterials[3];
};

#endif