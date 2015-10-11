#include "ActSpears.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 50
#define SPEED_X 12.0f
#define SPAWN_X_OFFSET 4.0f
#define HIT_X_OFFSET 0.3f
#define FADE_TIME 0.8f

ActSpears::ActSpears()
{
    m_nType = CARD_SPEARS;

    m_fPosX = 0.0f;
    m_nHit  = 0;
}

ActSpears::~ActSpears()
{

}

void ActSpears::OnCreate(void* pGame,
                         int   nIndex,
                         int   nCaster)
{
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(pGame);
#endif

    // Red side casted, spears on blue
    if(nCaster == 0)
    {
        m_fPosX = (GRID_WIDTH - 1) * TILE_WIDTH + SPAWN_X_OFFSET;
    }
    else
    {
        m_fPosX = -SPAWN_X_OFFSET;
    }
    m_fSpawnX = m_fPosX;

#if defined (SM_CLIENT)
    m_pMaterial = new DiffuseMaterial();
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        m_arMatters[i] = new Matter();
        m_arMatters[i]->SetMesh(g_pActSpearsMesh);
        m_arMatters[i]->SetTexture(g_pActSpearsTex);
        m_arMatters[i]->SetMaterial(m_pMaterial);

        m_arMatters[i]->SetRotation(0.0f, nCaster ? 90.0f : -90.0f, 0.0f);
        m_arMatters[i]->SetPosition(m_fPosX,
                                    MAGE_Y_OFFSET,
                                    TILE_HEIGHT * i);
        pTheGame->GetScene()->AddMatter(m_arMatters[i]);
    }
#endif

}

void ActSpears::Update()
{
    float fTime = 0;
    Activation::Update();

    fTime = m_timer.Time();

    if (m_nHit == 0)
    {
        m_fPosX = m_fSpawnX + (m_nCaster ? fTime*SPEED_X : -fTime*SPEED_X);

        if (m_nCaster == 0 &&
            m_fPosX   <= (GRID_WIDTH - 1) * TILE_WIDTH)
        {
            m_nHit = 1;
            m_timer.Start();
            CheckHit();
        }
        else if (m_nCaster == 1 &&
                 m_fPosX   >= 0.0f)
        {
            m_nHit = 1;
            m_timer.Start();
            CheckHit();
        }
    }
    else
    {
#if defined (SM_SERVER)
        m_nExpired = 1;
#else
        if (fTime >= FADE_TIME)
        {
            m_nExpired = 1;
        }
        else
        {
            float fFadeVal = (FADE_TIME - fTime)/FADE_TIME;

            m_pMaterial->SetColor(1.0f, fFadeVal, fFadeVal, fFadeVal);
        }
#endif
    }

#if defined (SM_CLIENT)

    // Update matter positions
    for (int i = 0; i < GRID_HEIGHT; i++)
    {
        m_arMatters[i]->SetPosition(m_fPosX, MAGE_Y_OFFSET, TILE_HEIGHT*i);
    }
#endif
}

void ActSpears::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    for (int i = 0; i < 3; i++)
    {
        reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveMatter(m_arMatters[i]);
        delete m_arMatters[i];
        m_arMatters[i] = 0;
    }
#endif
}

void ActSpears::CheckHit()
{
#if defined (SM_SERVER)
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    Pawn* pPawn = 0;
    int nX = 0;
    int nZ = 0;

    if (m_nCaster == SIDE_1)
    {
        for (int i = 0; i < GRID_HEIGHT; i++)
        {
            pPawn = pTheGame->m_arTiles[GRID_WIDTH - 1][i].GetPawn();

            if (pPawn != 0)
            {
                pPawn->Damage(DAMAGE);
                pPawn->GetPosition(nX,nZ);
                pPawn->SetPosition(nX - 1, nZ);
            }
        }
    }
    else
    {
        for (int i = 0; i < GRID_HEIGHT; i++)
        {
            pPawn = pTheGame->m_arTiles[0][i].GetPawn();

            if (pPawn != 0)
            {
                pPawn->Damage(DAMAGE);
                pPawn->GetPosition(nX,nZ);
                pPawn->SetPosition(nX + 1, nZ);
            }
        }
    }
#endif

}
