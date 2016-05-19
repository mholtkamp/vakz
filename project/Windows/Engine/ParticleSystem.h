#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Actor.h"
#include "Texture.h"
#include "Matrix.h"
#include "Timer.h"

class ParticleSystem : public Actor
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    ParticleSystem();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~ParticleSystem();

    //## **********************************************************************
    //## Initialize
    //## 
    //## Initializes data on VRAM that is needed for updating and rendering
    //## particle effects. This function should be called after setting 
    //## the desired particle count.
    //## **********************************************************************
    void Initialize();

    //## **********************************************************************
    //## Update
    //##
    //## Updates the position/lifetime/etc of particles and stores them on 
    //## transform feedback buffer.
    //## **********************************************************************
    void Update();

    //## **********************************************************************
    //## Render
    //## 
    //## Renders particles to the screen.
    //##
    //## Input:
    //##   pMatrixVP - the combined View/Projection matrix of camera.
    //## **********************************************************************
    void Render(Matrix* pMatrixVP);


    //## **********************************************************************
    //## SetParticleCount
    //##
    //## Sets the number of particles maintained by this particle system.
    //##
    //## Input:
    //##   nParticleCount - number of particles to maintain. 
    //##                    Should be between 0 and MAX_PARTICLE_COUNT.
    //## **********************************************************************
    void SetParticleCount(int nParticleCount);

    //## **********************************************************************
    //## SetEmissionRate
    //##
    //## Sets the emission rate, currently doesn't do anything because
    //## particles are always respawned as soon as their lifetime expires.
    //## **********************************************************************
    void SetEmissionRate(int nEmissionRate);

    //## **********************************************************************
    //## SetLifetime
    //##
    //## Sets min and max lifetime of particles in system.
    //## **********************************************************************
    void SetLifetime(float fMinLifetime,
                     float fMaxLifetime);

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets min and max color of particles in system.
    //## The way this works is that the color is chosen by getting a random
    //## float value between 0.0f and 1.0f, and then using that same value
    //## to lerp between arMinColor and arMaxColor for each component.
    //## If arMinColor is (0.2f, 0.2f, 0.0f, 1.0f) and
    //##    arMaxColor is (1.0f, 1.0f, 0.0f, 1.0f) for example,
    //## Then only yellows could be produced (from dark, unsaturated yellow
    //## to a fully saturated/bright yellow). There could be no red color
    //## produced, or no green color.
    //## **********************************************************************
    void SetColor(float arMinColor[4],
                  float arMaxColor[4]);

    //## **********************************************************************
    //## SetVelocity
    //##
    //## Sets the min and max Velocity of particles.
    //## **********************************************************************
    void SetVelocity(float arMinVelocity[3],
                     float arMaxVelocity[3]);

    //## **********************************************************************
    //## SetSize
    //##
    //## Sets the min and max size of particles in pixels.
    //## **********************************************************************
    void SetSize(float fMinSize,
                 float fMaxSize);

    //## **********************************************************************
    //## SetGravity
    //##
    //## Sets a constant acceleration.
    //## **********************************************************************
    void SetGravity(float arGravity[3]);

    //## **********************************************************************
    //## SetOrigin
    //##
    //## Sets point in world space where particles are spawned. (A random
    //## spawn variance is still added to this location. When spawn variance
    //## is set to (0.0f, 0.0f, 0.0f) then all particles will spawn at this
    //## origin.
    //## **********************************************************************
    void SetOrigin(float fX,
                   float fY,
                   float fZ);

    //## **********************************************************************
    //## SetSpawnVariance
    //##
    //## Sets the distance from origin that particles can be spawned.
    //## **********************************************************************
    void SetSpawnVariance(float fX,
                          float fY,
                          float fZ);

    //## **********************************************************************
    //## SetGenerate
    //##
    //## Sets flag to indicate that particles should be respawned when their
    //## lifetime expires.
    //## **********************************************************************
    void SetGenerate(int nGenerate);

    //## **********************************************************************
    //## SetTexture
    //##
    //## Sets the texture to be displayed for point-sprite particles.
    //## **********************************************************************
    void SetTexture(Texture* pTexture);

    enum ParticleSystemEnum
    {
        MAX_PARTICLE_COUNT = 1024,
        PARTICLE_DATA_SIZE = (12 + 12 + 16 + 4 + 4)
    };

private:

    // Static initialization flag
    // Should be 0 if no particle systems have been initialize.
    // Will be 1 if any particle system has been initialized.
    static int s_nStaticInit;

    // Particle system settings
    int m_nGenerate;
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
    float m_arOrigin[3];
    float m_arSpawnVariance[3];

    // Texture handle
    Texture* m_pTexture;

    //## VBO to hold particle properties
    //## Position = 12 bytes
    //## Velocity = 12 bytes
    //## Color    = 16 bytes
    //## Life     = 4 bytes
    unsigned int m_arVBOs[2];

    // Value to keep track of which buffer to use
    // when updating / rendering.
    int m_nUpdateBuffer;

    // Timer used by particle system for updating
    Timer m_timer;

    // Handle to sync object.
    void* m_pSync;
};

#endif