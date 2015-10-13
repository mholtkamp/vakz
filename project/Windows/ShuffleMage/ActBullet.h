#ifndef ACT_BULLET_H
#define ACT_BULLET_H

#include "Activation.h"
#include "ParticleSystem.h"

class ActBullet : public Activation
{
public:

    ActBullet();
    ~ActBullet();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    int m_nCastX;
    int m_nCastZ;

    int m_nHitX;
    int m_nHitZ;

private:

    ParticleSystem* m_arParticles[2];

};

#endif