#include "ActAreaGrab.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 20
#define OWNERSHIP_DURATION 20.0f
#define DROP_START_HEIGHT 5.0f
#define DROP_SEPARATION 3.0f
#define DROP_SPEED 10.0f
#define DROP_HIT_Y 0.5f
#define WAVE_POS_Y 0.2f
#define WAVE_DURATION 0.7f
#define WAVE_START_SCALE 0.5f

ActAreaGrab::ActAreaGrab()
{
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        m_arTargetX[i]   = 0;
        m_arTargetZ[i]   = 0;
        m_arPosY[i]      = 0.0f;
        m_arWaveScale[i] = 1.0f;
        m_arWaveAlpha[i] = 1.0f;
        m_arHit[i]       = 0;
        m_arHitTimes[i]  = 0.0f;
    }

    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;
}

ActAreaGrab::~ActAreaGrab()
{
    
}

void ActAreaGrab::OnCreate(void* pGame,
                           int   nIndex,
                           int   nCaster)
{
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

    int i    = 0;
    int j    = 0;
    int nCol = 0;

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    // Find the applicable column to drop on
    for (i = (nCaster == 0) ? 0 : (GRID_WIDTH - 1); (i < GRID_WIDTH) && (i >= 0);)
    {
        if (pTheGame->m_arTiles[i][0].GetOwner() != nCaster ||
            pTheGame->m_arTiles[i][1].GetOwner() != nCaster ||
            pTheGame->m_arTiles[i][2].GetOwner() != nCaster)
        {
            // At least one unowned tile is in this column, so this
            // will be the column that the areagrab drops will fall on
            nCol = i;
            break;
        }

        if (nCaster == 0)
        {
            i++;
        }
        else
        {
            i--;
        }
    }

    // Mark the target cells
    m_arTargetX[0] = nCol;
    m_arTargetX[1] = nCol;
    m_arTargetX[2] = nCol;

    m_arTargetZ[0] = (nCaster == 0) ? 2 : 0;
    m_arTargetZ[1] = 1;
    m_arTargetZ[2] = (nCaster == 0) ? 0 : 2;

#if defined (SM_CLIENT)
    // Choose color based on caster
    if (nCaster == 0)
    {
        m_arColor[0] = 1.0f;
        m_arColor[1] = 0.1f;
        m_arColor[2] = 0.1f;
        m_arColor[3] = 1.0f;
    }
    else
    {
        m_arColor[0] = 0.1f;
        m_arColor[1] = 0.1f;
        m_arColor[2] = 1.0f;
        m_arColor[3] = 1.0f;
    }

    // Create the matters to drop
    for (i = 0; i < 3; i++)
    {
        m_arMaterials[i] = new FullbrightMaterial();
        m_arMaterials[i]->SetColor(m_arColor[0],
                                   m_arColor[1],
                                   m_arColor[2],
                                   m_arColor[3]);

        m_arMatters[i] = new Matter();
        m_arMatters[i]->SetMesh(g_pActAreaGrabDropMesh);
        m_arMatters[i]->SetMaterial(m_arMaterials[i]);
        m_arMatters[i]->SetTexture(g_pActAreaGrabDropTex);
        m_arMatters[i]->SetPosition(m_arTargetX[i] * TILE_WIDTH,
                                    DROP_START_HEIGHT + DROP_SEPARATION*i,
                                    m_arTargetZ[i] * TILE_HEIGHT);
        pTheGame->GetScene()->AddMatter(m_arMatters[i]);
    }
#endif

    // Setup vertical position for server and client
    for (i = 0; i < 3; i++)
    {
        m_arPosY[i] = DROP_START_HEIGHT + DROP_SEPARATION*i;
    }

    // Kick off frame timer
    m_timerDelta.Start();
}

void ActAreaGrab::Update()
{
    int   i          = 0;
    float fTime      = 0;
    float fDeltaTime = 0;
    Activation::Update();

    fTime = m_timer.Time();
    m_timerDelta.Stop();
    fDeltaTime = m_timerDelta.Time();

#if defined SM_CLIENT
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    for (i = 0; i < 3; i++)
    {
        if(m_arHit[i] == 0)
        {
            m_arPosY[i] -= DROP_SPEED*fDeltaTime;
#if defined (SM_CLIENT)
            m_arMatters[i]->SetPosition(m_arTargetX[i] * TILE_WIDTH,
                                        m_arPosY[i],
                                        m_arTargetZ[i] * TILE_HEIGHT);
#endif
            if (m_arPosY[i] < DROP_HIT_Y)
            {
                m_arHit[i] = 1;
                m_arHitTimes[i] = fTime;

#if defined (SM_CLIENT)
                // Convert drops into waves
                m_arMatters[i]->SetMesh(g_pPlaneXZ);
                m_arMatters[i]->SetPosition(m_arTargetX[i] * TILE_WIDTH,
                                            WAVE_POS_Y,
                                            m_arTargetZ[i] * TILE_HEIGHT);
                m_arMatters[i]->SetTexture(g_pActAreaGrabWaveTex);
                m_arMatters[i]->SetScale(WAVE_START_SCALE, 1.0f, WAVE_START_SCALE);
#endif
            }
        }
        else
        {
            // Drop already hit, expand the waves and reduce alpha values
#if defined (SM_CLIENT)
            if(fTime - m_arHitTimes[i] <= WAVE_DURATION)
            {
                m_arMaterials[i]->SetColor(m_arColor[0],
                                           m_arColor[1],
                                           m_arColor[2],
                                           (WAVE_DURATION - (fTime - m_arHitTimes[i]))/WAVE_DURATION);
                m_arMatters[i]->SetScale((fTime - m_arHitTimes[i])/WAVE_DURATION + WAVE_START_SCALE,
                                         1.0f,
                                         (fTime - m_arHitTimes[i])/WAVE_DURATION + WAVE_START_SCALE);
            }
            else
            {
                m_arMaterials[i]->SetColor(m_arColor[0],
                                           m_arColor[1],
                                           m_arColor[2],
                                           0.0f);
            }
#else
            // Server needs to change tile ownership
            pTheGame->m_arTiles[m_arTargetX[i]][m_arTargetZ[i]].SetOwner(m_nCaster);
#endif
        }
    }

    if (fTime >= OWNERSHIP_DURATION)
    {
        m_nExpired = 1;
    }

    m_timerDelta.Start();
}

void ActAreaGrab::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        delete m_arMaterials[i];
        m_arMaterials[i] = 0;

        reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveMatter(m_arMatters[i]);
        delete m_arMatters[i];
        m_arMatters[i] = 0;
    }
#endif
}