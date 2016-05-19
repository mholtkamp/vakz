#include "ActBullet.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 30
#define LIFETIME 0.3f
#define PARTICLE_LIFETIME 1.0f

ActBullet::ActBullet()
{
    m_nCastX = 0;
    m_nCastZ = 0;
    m_nHitX  = 0;
    m_nHitZ  = 0;
}

ActBullet::~ActBullet()
{

}

void ActBullet::OnCreate(void* pGame,
                         int   nIndex,
                         int   nCaster)
{
    int i = 0;
    Pawn* pTargPawn = 0;

    Activation::OnCreate(pGame, nIndex, nCaster);

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    // Get cast location
    Mage* pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(m_nCastX, m_nCastZ);

    // Find the hit location for both server and client
    // but only client will deal damage of course.

    m_nHitX = -1;
    m_nHitZ = -1;

    for (i = m_nCastX + (nCaster ? -1 : 1); (i >= 0) && (i < GRID_WIDTH); i += (nCaster ? -1 : 1))
    {
        pTargPawn = pTheGame->m_arTiles[i][m_nCastZ].GetPawn();

        if (pTargPawn != 0)
        {
            m_nHitX = i;
            m_nHitZ = m_nCastZ;

#if defined (SM_SERVER)
            pTargPawn->Damage(DAMAGE);
#endif
            break;
        }
    }

#if defined (SM_CLIENT)
    if (m_nHitX != -1 && 
        m_nHitZ != -1)
    {
        // Create caster smoke particle effect
        float fOriginX = 0.0f;
        float arMinColor[4] = {0.0f, 0.0f, 0.0f, 0.4f};
        float arMaxColor[4] = {1.0f, 1.0f, 1.0f, 0.6f};
        float arMinVelocity[3];
        float arMaxVeloctiy[3];
        float arGravity[3] = {0.0f, 0.0f, 0.0f};

        if (m_nCaster == SIDE_2)
        {
            arMinVelocity[0] = -10.0f;
            arMinVelocity[1] = -3.0f;
            arMinVelocity[2] = 0.0f;

            arMaxVeloctiy[0] = -14.0f;
            arMaxVeloctiy[1] = 3.0f;
            arMaxVeloctiy[2] = 0.0f;

            fOriginX = m_nHitX * TILE_WIDTH + 0.5f*TILE_WIDTH;
        }
        else
        {
            arMinVelocity[0] = 10.0f;
            arMinVelocity[1] = -3.0f;
            arMinVelocity[2] = 0.0f;

            arMaxVeloctiy[0] = 14.0f;
            arMaxVeloctiy[1] = 3.0f;
            arMaxVeloctiy[2] = 0.0f;

            fOriginX = m_nHitX * TILE_WIDTH - 0.5f*TILE_WIDTH;
        }

        m_arParticles[0] = new ParticleSystem();
        m_arParticles[0]->SetOrigin(fOriginX,
                                    MAGE_Y_OFFSET,
                                    m_nHitZ * TILE_HEIGHT);
        m_arParticles[0]->SetSpawnVariance(0.1f, 0.1f, 0.1f);
        m_arParticles[0]->SetSize(5.0f, 20.0f);
        m_arParticles[0]->SetColor(arMinColor, arMaxColor);
        m_arParticles[0]->SetVelocity(arMinVelocity, arMaxVeloctiy);
        m_arParticles[0]->SetLifetime(PARTICLE_LIFETIME, PARTICLE_LIFETIME);
        m_arParticles[0]->SetGravity(arGravity);
        m_arParticles[0]->SetParticleCount(50);
        m_arParticles[0]->Initialize();
        pTheGame->GetScene()->AddActor(m_arParticles[0]);
    }
#endif
}

void ActBullet::Update()
{
    float fTime = 0.0f;

    Activation::Update();

    fTime = m_timer.Time();

    if (fTime >= LIFETIME)
    {
        m_nExpired = 1;
    }
}

void ActBullet::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    for (int i = 0; i < 2; i++)
    {
        if (m_arParticles[i] != 0)
        {
            reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveParticleSystem(m_arParticles[i]);
            delete m_arParticles[i];
            m_arParticles[i] = 0;
        }
    }
#endif
}
