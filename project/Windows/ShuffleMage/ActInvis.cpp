#include "ActInvis.h"
#include "Resources.h"
#include "Constants.h"

#if defined (SM_SERVER)
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define LIFETIME 10.0f
#define MIN_PSPEED -1.0f
#define MAX_PSPEED  1.0f
#define PARTICLE_GEN_DURATION 1.0f

ActInvis::ActInvis()
{
    m_nType    = CARD_INVIS;

    m_fOriginX = 0.0f;
    m_fOriginY = 0.0f;
    m_fOriginZ = 0.0f;

    m_pParticle = 0;
}

ActInvis::~ActInvis()
{
    
}

void ActInvis::OnCreate(void* pGame,
                        int   nIndex,
                        int   nCaster)
{   
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined (SM_CLIENT)
    int nCastX = 0;
    int nCastZ = 0;

    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    Mage* pMage    = pTheGame->GetMage(nCaster);

    pMage->GetPosition(nCastX, nCastZ);
    m_fOriginX = static_cast<float>(nCastX * TILE_WIDTH);
    m_fOriginY = MAGE_Y_OFFSET;
    m_fOriginZ = static_cast<float>(nCastZ * TILE_HEIGHT);

    // Create particle system
    float arMinVelocity[3] = {MIN_PSPEED, MIN_PSPEED, 0.0f};
    float arMaxVelocity[3] = {MAX_PSPEED, MAX_PSPEED, 0.0f};
    float arGravity[3]     = {0.0f, 0.0f, 0.0f};
    m_pParticle = new ParticleSystem();
    m_pParticle->SetParticleCount(40);
    m_pParticle->SetLifetime(1.0f, 2.0f);
    m_pParticle->SetVelocity(arMinVelocity, arMaxVelocity);
    m_pParticle->SetSize(20.0f, 40.0f);
    m_pParticle->SetOrigin(m_fOriginX, m_fOriginY, m_fOriginZ);
    m_pParticle->SetGravity(arGravity);
    m_pParticle->SetTexture(g_pActInvisTex);
    m_pParticle->Initialize();
    pTheGame->GetScene()->AddParticleSystem(m_pParticle);
    
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    Mage*       pMage    = pTheGame->GetMage(nCaster);

    pMage->SetInvisible(1);
#endif
}

void ActInvis::Update()
{
    float fTime = 0;
    Activation::Update();

    fTime = m_timer.Time();
    
#if defined (SM_CLIENT)
    if (fTime >= PARTICLE_GEN_DURATION)
    {
       m_nExpired = 1;
    }
#endif

#if defined (SM_SERVER)
    if (fTime >= LIFETIME)
    {
        m_nExpired = 1;
        
        ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
        Mage*       pMage    = pTheGame->GetMage(m_nCaster);

        pMage->SetInvisible(0);
    }
#endif
}

void ActInvis::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveParticleSystem(m_pParticle);
    delete m_pParticle;
    m_pParticle = 0;
#endif
}
