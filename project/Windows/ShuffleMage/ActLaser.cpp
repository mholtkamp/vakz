#include "ActLaser.h"
#include "Game.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#endif

#define DAMAGE   20
#define HIT_TIME 0.1f
#define LIFETIME 0.5f
#define OFFSET_Y 0.5f


ActLaser::ActLaser()
{
    m_nType     = CARD_LASER;

    m_nCastX    = 0;
    m_nCastZ    = 0;
    m_nHit      = 0;
    m_pMaterial = 0;
}

ActLaser::~ActLaser()
{

}

void ActLaser::Update()
{
    float fTime = 0;
    Activation::Update();

    fTime = m_timer.Time();

#if defined(SM_SERVER)
    int i = 0;
    int nEnemyX = 0;
    int nEnemyZ = 0;

    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    Mage*       pEnemyMage = 0;

    if (fTime > HIT_TIME &&
        m_nHit == 0)
    {
        // Perform hit check and damage calculation
        if (m_nCaster == SIDE_1)
        {
            for (i = m_nCastX + 1; i < GRID_WIDTH; i++)
            {
                if (pTheGame->m_arTiles[i][m_nCastZ].GetPawn() != 0)
                {
                    pTheGame->m_arTiles[i][m_nCastZ].GetPawn()->Damage(DAMAGE);
                }
            }
        }
        else if (m_nCaster == SIDE_2)
        {
            for (i = m_nCastX - 1; i >= 0; i--)
            {
                if (pTheGame->m_arTiles[i][m_nCastZ].GetPawn() != 0)
                {
                    pTheGame->m_arTiles[i][m_nCastZ].GetPawn()->Damage(DAMAGE);
                }
            }
        }
        
        m_nHit = 1;
    }
#endif

#if defined(SM_CLIENT)
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, (LIFETIME - fTime)/LIFETIME);
#endif

    if (fTime > LIFETIME)
    {
        m_nExpired = 1;
    }
}

void ActLaser::OnCreate(void* pGame,
                        int   nIndex,
                        int   nCaster)
{
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined(SM_CLIENT)
    Mage* pMage = 0;

    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(m_nCastX, m_nCastZ);
    m_pMaterial = new DiffuseMaterial();
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    pTheGame->m_arActMatters[nIndex].SetMaterial(m_pMaterial);
    pTheGame->m_arActMatters[nIndex].SetMesh(reinterpret_cast<Mesh*>(g_pActLaserMesh));
    pTheGame->m_arActMatters[nIndex].SetTexture(g_pActLaserTex);
    pTheGame->m_arActMatters[nIndex].SetPosition(m_nCastX * TILE_WIDTH,
                                                 OFFSET_Y,
                                                 m_nCastZ * TILE_HEIGHT);

    pTheGame->m_arActMatters[nIndex].SetRotation(0.0f, 
                                                 nCaster ? -90.0f : 90.0f,
                                                 0.0f);

    g_pActLaserSound->Play();
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    Mage* pMage          = pTheGame->GetMage(nCaster);

    pMage->GetPosition(m_nCastX, m_nCastZ);
#endif
}

void ActLaser::OnDestroy()
{
#if defined (SM_CLIENT)
    Activation::OnDestroy();
    delete m_pMaterial;
#endif
}