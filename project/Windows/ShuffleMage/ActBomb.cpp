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

    //pTheGame->m_arActMatters[nIndex].SetRotation(0.0f, 
    //                                             nCaster ? -90.0f : 90.0f,
    //                                             0.0f
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

    if (m_nCaster == SIDE_1)
    {
        m_fPosX = m_fCastX + fTime*SPEED_X;
        m_fPosY = (-8.0f/9.0f)*(m_fPosX - (m_fCastX - 1.5f))*(m_fPosX - (m_fCastX - 1.5f)) 
                  + (20.0f/3.0f)*(m_fPosX - (m_fCastX - 1.5f))
                  - 7.5f;

        if (m_fPosX > static_cast<float>(m_nTargetX * TILE_WIDTH) + BUFFER_X)
        {
            m_nExpired = 1;
            CheckHit();
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
            m_nExpired = 1;
            CheckHit();
        }

    }

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pTheGame->m_arActMatters[m_nActivityIndex].SetPosition(m_fPosX,
                                                           m_fPosY,
                                                           m_fPosZ);
#endif
}

void ActBomb::OnDestroy()
{
#if defined (SM_CLIENT)
    Activation::OnDestroy();
#endif
}

void ActBomb::CheckHit()
{
#if defined (SM_SERVER)

    int nEnemyX;
    int nEnemyZ;

    Mage* pEnemyMage = 0;
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);

    if (m_nCaster == SIDE_1)
    {
        pEnemyMage = pTheGame->GetMage(SIDE_2);
    }
    else
    {
        pEnemyMage = pTheGame->GetMage(SIDE_1);
    }

    pEnemyMage->GetPosition(nEnemyX, nEnemyZ);

    if(nEnemyX == m_nTargetX &&
        nEnemyZ == m_nTargetZ)
    {
        pEnemyMage->Damage(DAMAGE);
    }
#endif
}