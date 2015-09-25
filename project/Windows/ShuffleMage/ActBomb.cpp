#include "ActBomb.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif


#define DAMAGE   60
#define SPEED_X  4.0f
#define OFFSET_Y 0.5f
#define BUFFER_X 0.1f
#define FUSE_PARTICLE_OFFSET_Y 0.3f
#define EXPLOSION_DURATION 1.0f

ActBomb::ActBomb()
{
    m_fCastX    = 0.0f;
    m_fCastY    = 0.0f;
    m_fCastZ    = 0.0f;

    m_fPosX     = 0.0f;
    m_fPosY     = 0.0f;
    m_fPosZ     = 0.0f;

    m_nTargetX  = 0;
    m_nTargetZ  = 0;
    m_nHit      = 0;

    m_pParticle = 0;
}

ActBomb::~ActBomb()
{

}

void ActBomb::OnCreate(void* pGame,
                       int   nIndex,
                       int   nCaster)
{
    int nCastX = 0;
    int nCastZ = 0;
    int nSide  = 0;

    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined(SM_CLIENT)
    Mage* pMage = 0;
    float arMinColor[4] = {1.0f,0.0f,0.0f, 0.5f};
    float arMaxColor[4] = {1.0f, 0.75f, 0.0f, 1.0f};
    float arMinVelocity[3] = {-4.0f, 6.0f, 0.0f};
    float arMaxVelocity[3] = {4.0f, 8.0f, 0.0f};

    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(nCastX, nCastZ);
    if (nCaster == SIDE_1)
    {
        m_nTargetX = nCastX + 3;
        m_nTargetZ = nCastZ;
    }
    else
    {
        m_nTargetX = nCastX - 3;
        m_nTargetZ = nCastZ;
    }

    if (m_nTargetX < 0           ||
        m_nTargetX >= GRID_WIDTH ||
        m_nTargetZ < 0           ||
        m_nTargetZ >= GRID_HEIGHT)
    {
        // Target is outside of grid
        m_nExpired = 1;
        return;
    }
    
    m_fCastX = static_cast<float>(nCastX * TILE_WIDTH);
    m_fCastY = OFFSET_Y;
    m_fCastZ = static_cast<float>(nCastZ * TILE_HEIGHT);

    m_fPosX = m_fCastX;
    m_fPosY = m_fCastY;
    m_fPosZ = m_fCastZ;

    pTheGame->m_arActMatters[nIndex].SetMaterial(g_pDiffuseMaterial);
    pTheGame->m_arActMatters[nIndex].SetMesh(reinterpret_cast<Mesh*>(g_pActBombMesh));
    pTheGame->m_arActMatters[nIndex].SetTexture(g_pActBombTex);
    pTheGame->m_arActMatters[nIndex].SetPosition(m_fPosX,
                                                 m_fPosY,
                                                 m_fPosZ);

    // Create ParticleSystem.
    m_pParticle = new ParticleSystem();
    m_pParticle->SetParticleCount(20);
    m_pParticle->SetColor(arMinColor, arMaxColor);
    m_pParticle->SetVelocity(arMinVelocity, arMaxVelocity);
    m_pParticle->SetLifetime(0.05f, 0.2f);
    m_pParticle->SetSize(3.0f, 4.0f);
    m_pParticle->SetOrigin(m_fPosX, m_fPosY + FUSE_PARTICLE_OFFSET_Y, m_fPosZ);
    m_pParticle->Initialize();
    pTheGame->GetScene()->AddParticleSystem(m_pParticle);

#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    Mage* pMage          = pTheGame->GetMage(nCaster);

    pMage->GetPosition(nCastX, nCastZ);

    if (nCaster == SIDE_1)
    {
        m_nTargetX = nCastX + 3;
        m_nTargetZ = nCastZ;
    }
    else
    {
        m_nTargetX = nCastX - 3;
        m_nTargetZ = nCastZ;
    }

    if (m_nTargetX < 0           ||
        m_nTargetX >= GRID_WIDTH ||
        m_nTargetZ < 0           ||
        m_nTargetZ >= GRID_HEIGHT)
    {
        // Target is outside of grid
        m_nExpired = 1;
        return;
    }
    
    m_fCastX = static_cast<float>(nCastX * TILE_WIDTH);
    m_fCastY = OFFSET_Y;
    m_fCastZ = static_cast<float>(nCastZ * TILE_HEIGHT);

    m_fPosX = m_fCastX;
    m_fPosY = m_fCastY;
    m_fPosZ = m_fCastZ;
#endif
}

void ActBomb::Update()
{
    float fTargetX = 0.0f;
    float fTime = 0;
    Activation::Update();

    fTime = m_timer.Time();

    if (m_nHit == 0)
    {
        if (m_nCaster == SIDE_1)
        {
            m_fPosX = m_fCastX + fTime*SPEED_X;
            m_fPosY = (-8.0f/9.0f)*(m_fPosX - (m_fCastX - 1.5f))*(m_fPosX - (m_fCastX - 1.5f)) 
                      + (20.0f/3.0f)*(m_fPosX - (m_fCastX - 1.5f))
                      - 7.5f;

            if (m_fPosX > static_cast<float>(m_nTargetX * TILE_WIDTH) + BUFFER_X)
            {
                m_nHit = 1;
                CheckHit();
                BeginExplosion();
            }
        }
        else
        {
            // Use the target as origin of parabola instead of cast position
            fTargetX = TILE_WIDTH * m_nTargetX;
            m_fPosX = m_fCastX - fTime*SPEED_X;
            m_fPosY = (-8.0f/9.0f)*(m_fPosX - (fTargetX - 1.5f))*(m_fPosX - (fTargetX - 1.5f)) 
                      + (20.0f/3.0f)*(m_fPosX - (fTargetX - 1.5f))
                      - 7.5f;

            if (m_fPosX < static_cast<float>(m_nTargetX * TILE_WIDTH) - BUFFER_X)
            {
                m_nHit = 1;
                CheckHit();
                BeginExplosion();
            }
        }
    }
    else
    {
        // The bomb already hit the target, now the particle effect is producing 
        // flames on the target tile.
        if (fTime >= EXPLOSION_DURATION)
        {
            m_nExpired = 1;
        }
    }

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pTheGame->m_arActMatters[m_nActivityIndex].SetPosition(m_fPosX,
                                                           m_fPosY,
                                                           m_fPosZ);
    m_pParticle->SetOrigin(m_fPosX, m_fPosY + FUSE_PARTICLE_OFFSET_Y, m_fPosZ);
#endif
}

void ActBomb::OnDestroy()
{
#if defined (SM_CLIENT)
    Activation::OnDestroy();
    reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveParticleSystem(m_pParticle);
    delete m_pParticle;
    m_pParticle = 0;
#endif
}

void ActBomb::CheckHit()
{
#if defined (SM_SERVER)

    Pawn* pTargPawn = 0;
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);

    // Check if the target tile is occupied, if so, apply damage
    pTargPawn = pTheGame->m_arTiles[m_nTargetX][m_nTargetZ].GetPawn();
    if (pTargPawn != 0)
    {
        pTargPawn->Damage(DAMAGE);
    }
#endif
}

void ActBomb::BeginExplosion()
{

#if defined (SM_CLIENT)
    // Reset the timer for client, the server will immediately
    // expire the activation since the timer is already past
    // EXPLOSION_DURATION
    m_timer.Start();

    float arMinVelocity[3] = {0.0f, 0.5f, 0.0f};
    float arMaxVelocity[3] = {0.0f, 2.0f, 0.0f};

    m_pParticle->SetSize(10.0f, 48.0f);
    m_pParticle->SetOrigin(m_nTargetX*TILE_WIDTH,
                           0.0f,
                           m_nTargetZ*TILE_HEIGHT);
    m_pParticle->SetSpawnVariance(TILE_WIDTH/2.0f,
                                  0.0f,
                                  TILE_HEIGHT/2.0f);
    m_pParticle->SetLifetime(0.3f, 0.7f);
    m_pParticle->SetVelocity(arMinVelocity, arMaxVelocity);
#endif
}
