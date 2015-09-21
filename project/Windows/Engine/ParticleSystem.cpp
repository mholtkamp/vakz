#include "ParticleSystem.h"
#include "VGL.h"
#include "Log.h"

ParticleSystem::ParticleSystem()
{
    m_nParticleCount = 200;

    m_nEmissionRate = 20;

    m_fMinLifetime = 2.0f;
    m_fMaxLifetime = 4.0f;

    m_arMinColor[0] = 1.0f;
    m_arMinColor[1] = 1.0f;
    m_arMinColor[2] = 1.0f;
    m_arMinColor[3] = 1.0f;

    m_arMaxColor[0] = 1.0f;
    m_arMaxColor[1] = 1.0f;
    m_arMaxColor[2] = 1.0f;
    m_arMaxColor[3] = 1.0f;

    m_arMinVelocity[0] = 0.0f;
    m_arMinVelocity[1] = 1.0f;
    m_arMinVelocity[2] = 0.0f;

    m_arMaxVelocity[0] = 0.0f;
    m_arMaxVelocity[1] = 2.0f;
    m_arMaxVelocity[2] = 0.0f;

    m_fMinSize = 0.1f;
    m_fMaxSize = 0.2f;

    m_arGravity[0] = 0.0f;
    m_arGravity[1] = 0.0f;
    m_arGravity[2] = 0.0f;

    m_pTexture = 0;

    m_arVBOs[0] = 0;
    m_arVBOs[1] = 0;

    m_nDrawBuffer = 0;
}

ParticleSystem::~ParticleSystem()
{
    // Free video memory
    // Allow for a glError if the VBOs are 0.
    glDeleteBuffers(1, m_arVBOs);
}

void ParticleSystem::Initialize()
{   
    // Create and allocate the two buffers that will be swapped
    // when performing transform feedback
    glGenBuffers(2, m_arVBOs);

    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 PARTICLE_DATA_SIZE * m_nParticleCount,
                 0,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 PARTICLE_DATA_SIZE * m_nParticleCount,
                 0,
                 GL_DYNAMIC_DRAW);
}

void ParticleSystem::Update()
{
    
}

void ParticleSystem::Render()
{

}

void ParticleSystem::SetParticleCount(int nParticleCount)
{
    if (nParticleCount > 0 &&
        nParticleCount <= MAX_PARTICLE_COUNT)
    {
        m_nParticleCount = nParticleCount;
    }
    else
    {
        LogError("Particle count cannot be set above engine maximum.");
    }
}

void ParticleSystem::SetEmissionRate(int nEmissionRate)
{
    if (nEmissionRate > 0)
    {
        m_nEmissionRate = nEmissionRate;
        m_fEmissionPeriod = 1.0f/m_nEmissionRate;
    } 
}

void ParticleSystem::SetLifetime(float fMinLifetime,
                                 float fMaxLifetime)
{
    m_fMinLifetime = fMinLifetime;
    m_fMaxLifetime = fMaxLifetime;
}

void ParticleSystem::SetColor(float arMinColor[4],
                              float arMaxColor[4])
{
    m_arMinColor[0] = arMinColor[0];
    m_arMinColor[1] = arMinColor[1];
    m_arMinColor[2] = arMinColor[2];
    m_arMinColor[3] = arMinColor[3];

    m_arMaxColor[0] = arMaxColor[0];
    m_arMaxColor[1] = arMaxColor[1];
    m_arMaxColor[2] = arMaxColor[2];
    m_arMaxColor[3] = arMaxColor[3];
}

void ParticleSystem::SetVelocity(float arMinVelocity[3],
                                 float arMaxVelocity[3])
{
    m_arMinVelocity[0] = arMinVelocity[0];
    m_arMinVelocity[1] = arMinVelocity[1];
    m_arMinVelocity[2] = arMinVelocity[2];

    m_arMaxVelocity[0] = arMaxVelocity[0];
    m_arMaxVelocity[1] = arMaxVelocity[1];
    m_arMaxVelocity[2] = arMaxVelocity[2];
}

void ParticleSystem::SetSize(float fMinSize,
                             float fMaxSize)
{
    m_fMinSize = fMinSize;
    m_fMaxSize = fMaxSize;
}

void ParticleSystem::SetGravity(float arGravity[3])
{
    m_arGravity[0] = arGravity[0];
    m_arGravity[1] = arGravity[1];
    m_arGravity[2] = arGravity[2];
}

void ParticleSystem::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}
