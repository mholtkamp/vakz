#include "ActBubble.h"
#include "Resources.h"
#include "Constants.h"
#include <math.h>

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 20
#define SPEED_X 20.0f
#define SHOT_BUBBLE_SCALE 0.4f
#define STUN_DURATION 3.5f
#define STUN_BUBBLE_SCALE 1.0f
#define PULSE_ANGULAR_SPEED 5.0f

ActBubble::ActBubble()
{
    m_nType = CARD_BUBBLE;

    m_fPosX = 0;
    m_fPosY = 0;
    m_fPosZ = 0;

    m_nHit     = 0;
    m_nTargetX = 0;
    m_nTargetZ = 0;

    m_pMatter   = 0;
    m_pMaterial = 0;
}

ActBubble::~ActBubble()
{

}

void ActBubble::OnCreate(void* pGame,
                         int   nIndex,
                         int   nCaster)
{
    int nCastX = 0;
    int nCastZ = 0;
    Activation::OnCreate(pGame, nIndex, nCaster);

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    Mage* pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(nCastX, nCastZ);

    m_fPosX = nCastX * TILE_WIDTH;
    m_fPosY = MAGE_Y_OFFSET;
    m_fPosZ = nCastZ * TILE_HEIGHT;

    m_nTargetX = (nCaster ? nCastX - 1 : nCastX + 1);
    m_nTargetZ = nCastZ;

#if defined (SM_CLIENT)
    m_pMaterial = new DiffuseMaterial();
    m_pMaterial->SetColor(0.0f, 0.2f, 0.8f, 0.5f);

    m_pMatter = new Matter();
    m_pMatter->SetMaterial(m_pMaterial);
    m_pMatter->SetMesh(g_pSphere);
    m_pMatter->SetPosition(m_fPosX,
                           m_fPosY,
                           m_fPosZ);
    m_pMatter->SetScale(SHOT_BUBBLE_SCALE,
                        SHOT_BUBBLE_SCALE,
                        SHOT_BUBBLE_SCALE);
    pTheGame->GetScene()->AddActor(m_pMatter);
#endif
}

void ActBubble::Update()
{
    float fTime = 0.0f;
    float fScaleFactor = 1.0f;
    Pawn* pTargetPawn = 0;

    Activation::Update();
    fTime = m_timer.Time();

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    if (m_nHit == 0)
    {
        if (m_nCaster == SIDE_1)
        {
            m_fPosX += SPEED_X * fTime;

            if (m_nTargetX < GRID_WIDTH)
            {   
                // Bubble has reached target
                if (m_fPosX >= (m_nTargetX * TILE_WIDTH))
                {
                    pTargetPawn = pTheGame->m_arTiles[m_nTargetX][m_nTargetZ].GetPawn();
                    if (pTargetPawn != 0)
                    {
                        // Apply damage if server
#if defined (SM_SERVER)
                       pTargetPawn->Damage(DAMAGE);

                       if (pTargetPawn == pTheGame->GetMage(!m_nCaster))
                       {
                            DestroyPreviousHits();
                            reinterpret_cast<Mage*>(pTargetPawn)->SetStunned(1);
                       }
#endif
                        m_nHit = 1;

                        m_fPosX = m_nTargetX * TILE_WIDTH;
                    }

                    m_nTargetX++;
                }
            }
        }
        else
        {
            m_fPosX -= SPEED_X * fTime;
            
            if (m_nTargetX >= 0)
            {
                // Bubble has reached target
                if (m_fPosX <= (m_nTargetX * TILE_WIDTH))
                {
                    pTargetPawn = pTheGame->m_arTiles[m_nTargetX][m_nTargetZ].GetPawn();
                    if (pTargetPawn != 0)
                    {
                        // Apply damage if server
#if defined (SM_SERVER)
                       pTargetPawn->Damage(DAMAGE);

                       if (pTargetPawn == pTheGame->GetMage(!m_nCaster))
                       {
                            DestroyPreviousHits();
                            reinterpret_cast<Mage*>(pTargetPawn)->SetStunned(1);
                       }
#endif
                        m_nHit = 1;

                        m_fPosX = m_nTargetX * TILE_WIDTH;
                    }
                    m_nTargetX--;
                }
            }
        }

#if defined (SM_CLIENT)
        m_pMatter->SetPosition(m_fPosX, m_fPosY, m_fPosZ);
#endif

        // Check if the bubble shot is offscreen
        if (m_fPosX >= 15.0f ||
            m_fPosX < -10.0f)
        {
            m_nExpired = 1;
        }

        m_timer.Start();
    }
    else
    {
        // Do some weird pulsating stuff
#if defined (SM_CLIENT)
        fScaleFactor = STUN_BUBBLE_SCALE + cosf(fTime * PULSE_ANGULAR_SPEED)/5.0f;
        m_pMatter->SetScale(fScaleFactor, fScaleFactor, fScaleFactor);
#endif

        if (fTime >= STUN_DURATION)
        {
            m_nExpired = 1;

#if defined (SM_SERVER)
            pTheGame->GetMage(!m_nCaster)->SetStunned(0);
#endif
        }
    }

    
}


void ActBubble::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveMatter(m_pMatter);

    delete m_pMaterial;
    m_pMaterial = 0;

    delete m_pMatter;
    m_pMatter = 0;
#endif
}

void ActBubble::DestroyPreviousHits()
{
    int i = 0;

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif


    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (pTheGame->m_arActivations[i] != 0 &&
            pTheGame->m_arActivations[i]->m_nType == m_nType)
        {
            // Duplicate has been found, but check if it has hit before destroying it.
            if (reinterpret_cast<ActBubble*>(pTheGame->m_arActivations[i])->m_nHit != 0)
            {
                pTheGame->m_arActivations[i]->OnDestroy();
                pTheGame->m_arActivations[i] = 0;
            }
        }
    }
}