#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Texture.h"

class ParticleSystem
{
public:

    ParticleSystem();
    ~ParticleSystem();

    void Initialize();

    void Update();

    void Render();

    void SetParticleCount(int nParticleCount);

    void SetEmissionRate(int nEmissionRate);

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
        MAX_PARTICLE_COUNT = 1024,
        PARTICLE_DATA_SIZE = (12 + 12 + 16 + 4)*sizeof(float)
    };

private:

    int m_nParticleCount;

    int m_nEmissionRate;
    float m_fEmissionPeriod;

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

    //## VBO to hold particle properties
    //## Position = 12 bytes
    //## Velocity = 12 bytes
    //## Color    = 16 bytes
    //## Life     = 4 bytes
    unsigned int m_arVBOs[2];

    int m_nUpdateBuffer;
};

#endif