#include "ActSwipe.h"
#include "Resources.h"
#include "Constants.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define LIFETIME 1.0f
#define DAMAGE 40

ActSwipe::ActSwipe()
{
    m_nType = CARD_SWIPE;

    m_nTargetX = 0;
    m_nTargetZ = 0;

    m_pMatter = 0;

    m_nHit = 0;
}

ActSwipe::~ActSwipe()
{

}

void ActSwipe::OnCreate(void* pGame,
                        int   nIndex,
                        int   nCaster)
{
    int nMageX = 0;
    int nMageZ = 0;

    Activation::OnCreate(pGame, nIndex, nCaster);

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    // Figure out target position
    Mage* pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(nMageX, nMageZ);

    m_nTargetX = nMageX + (nCaster ? -1 : 1);
    m_nTargetZ = nMageZ;

#if defined (SM_CLIENT)
    m_pMaterial = new FullbrightMaterial();

    m_pMatter = new Matter();
    m_pMatter->SetMesh(g_pActSwipeMesh);
    m_pMatter->SetTexture(g_pActSwipeTex);
    m_pMatter->SetMaterial(m_pMaterial);
    m_pMatter->SetRotation(0.0f, nCaster ? -90.0f : 90.0f, 0.0f);
    m_pMatter->SetPosition(nMageX * TILE_WIDTH,
                           MAGE_Y_OFFSET,
                           nMageZ * TILE_HEIGHT);
    pTheGame->GetScene()->AddActor(m_pMatter);
#endif
}

void ActSwipe::Update()
{
    float fTime = 0;
    Activation::Update();

    fTime = m_timer.Time();

#if defined (SM_CLIENT)
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, (LIFETIME - fTime)/LIFETIME);
#else
    int i                = 0;
    Pawn* pPawn          = 0;
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);

    if (m_nHit == 0)
    {
        // Perform damage check

        for (i = -1; i < 2; i++)
        {
            if (m_nTargetX     >= 0          &&
                m_nTargetX     <  GRID_WIDTH &&
                m_nTargetZ + i >= 0          &&
                m_nTargetZ + i <  GRID_HEIGHT)
            {
                pPawn = pTheGame->m_arTiles[m_nTargetX][m_nTargetZ + i].GetPawn();
                if (pPawn != 0)
                {
                    pPawn->Damage(DAMAGE);
                }
            }
        }

        m_nHit = 1;
    }
#endif

    if (fTime >= LIFETIME)
    {
        m_nExpired = 1;
    }
}

void ActSwipe::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);

    pTheGame->GetScene()->RemoveMatter(m_pMatter);
    
    delete m_pMaterial;
    m_pMaterial = 0;

    delete m_pMatter;
    m_pMatter = 0;
#endif
}