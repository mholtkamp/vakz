#ifndef ACT_BOMB_H
#define ACT_BOMB_H

#include "Activation.h"
#include "ParticleSystem.h"

class ActBomb : public Activation
{
public:

    ActBomb();
    ~ActBomb();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    float m_fCastX;
    float m_fCastY;
    float m_fCastZ;

    float m_fPosX;
    float m_fPosY;
    float m_fPosZ;

    int m_nTargetX;
    int m_nTargetZ;

    int m_nHit;

private:

    void CheckHit();
    void BeginExplosion();
    ParticleSystem* m_pParticle;
};

#endif