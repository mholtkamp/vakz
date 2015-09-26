#ifndef ACT_INVIS_H
#define ACT_INVIS_H

#include "Activation.h"
#include "ParticleSystem.h"

class ActInvis : public Activation
{
public:

    ActInvis();
    ~ActInvis();

    void Update();

    void OnCreate(void* pGame,
                  int   nIndex,
                  int   nCaster);

    void OnDestroy();

    float m_fOriginX;
    float m_fOriginY;
    float m_fOriginZ;

private:

    ParticleSystem* m_pParticle;
};

#endif