#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Texture.h"

class ParticleSystem
{
public:

    ParticleSystem();
    ~ParticleSystem();

    void Update();

    void Render();

    void SetMaxParticles(int nMaxParticles);

    void SetEmissionRate(float fEmissionRate);

    void SetLifetime(float fMinLifetime,
                     float fMaxLifetime);

    void SetColor(float arMinColor[4],
                  float arMaxColor[4]);

    void SetVelocity(float arMinVelocity[3],
                     float arMaxVelocity[3]);

    void SetSize(float fMinSize,
                 float fMaxSize);

    void SetGravity(float arGravity[3]);

    void SetTexture(Texture* pTexture);

    enum ParticleSystemEnum
    {
        MAX_PARTICLE_COUNT = 1024
    };

private:

    int m_nMaxParticles;

    float m_fEmissionRate;

    float m_fMinLifetime;
    float m_fMaxLifetime;

    float m_arMinColor[4];
    float m_arMaxColor[4];

    float m_arMinVelocity[3];
    float m_arMaxVelocity[3];

    float m_fMinSize;
    float m_fMaxSize;

    float m_arGravity[3];

    Texture* m_pTexture;

    int m_hVBO;
};

#endif