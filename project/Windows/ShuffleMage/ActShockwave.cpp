#include "ActShockwave.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 40
#define ROT_SPEED_Y 200.0f
#define HIT_TIME 0.25f

ActShockwave::ActShockwave()
{
    m_nType = CARD_SHOCKWAVE;

    m_nCastX = 0;
    m_nCastZ = 0;

    m_nWave2Processing = 0;

    m_arTarget[0] = 0;
    m_arTarget[1] = 0;

    m_arHit[0] = 0;
    m_arHit[1] = 0;

    m_arMaterials[0] = 0;
    m_arMaterials[1] = 0;

    m_arMatter[0] = 0;
    m_arMatter[1] = 0;
}

ActShockwave::~ActShockwave()
{

}

void ActShockwave::OnCreate(void* pGame,
                            int   nIndex,
                            int   nCaster)
{
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    Mage* pMage = pTheGame->GetMage(nCaster);
    pMage->GetPosition(m_nCastX, m_nCastZ);

    // Using 2 waves
    for (int i = 0; i < 2; i++)
    {
        m_arTarget[i] = nCaster ? m_nCastX - (i+1) : m_nCastX + (i+1);

        if (m_arTarget[i] >= GRID_WIDTH ||
            m_arTarget[i] < 0)
        {
            m_arTarget[i] = -1;
        }

#if defined (SM_CLIENT)
        m_arMaterials[i] = new FullbrightMaterial();
        m_arMaterials[i]->SetColor(1.0f, 1.0f, 1.0f, 0.0f);

        m_arMatter[i] = new Matter();
        m_arMatter[i]->SetMesh(g_pActShockwaveMesh);
        m_arMatter[i]->SetTexture(g_pActShockwaveTex);
        m_arMatter[i]->SetMaterial(m_arMaterials[i]);
        if (m_arTarget[i] == -1)
        {
            m_arMatter[i]->SetPosition(-9001.0f, 0.0f, 0.0f);
        }
        else
        {
            m_arMatter[i]->SetPosition(static_cast<float>(m_arTarget[i]) * TILE_WIDTH,
                                       0.0f,
                                       static_cast<float>(m_nCastZ) * TILE_HEIGHT);
        }
        
        pTheGame->GetScene()->AddMatter(m_arMatter[i]);
#endif
    }

    // Kick off timer 1
    m_arWaveTimers[0].Start();
}

void ActShockwave::Update()
{
    float fTime  = 0.0f;
    float fTime1 = 0.0f;
    float fTime2 = 0.0f;

    Activation::Update();

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    // Get activation time, used to flag when to start
    // processing the second wave.
    fTime = m_timer.Time();

    // Get the wave time for wave 1
    m_arWaveTimers[0].Stop();
    fTime1 = m_arWaveTimers[0].Time();

    // Check if wave 2 processing should be started
    if (fTime >= HIT_TIME &&
        m_nWave2Processing == 0)
    {
        m_arWaveTimers[1].Start();
        m_nWave2Processing = 1;
    }

    // Update Wave 1
    UpdateWave(0, fTime1);

    // Check if second wave needs to be processed.
    if (m_nWave2Processing != 0)
    {
        m_arWaveTimers[1].Stop();
        fTime2 = m_arWaveTimers[1].Time();

        UpdateWave(1, fTime2);
    }

    // Check if there are no more targets
    if (m_arTarget[0] == -1 &&
        m_arTarget[1] == -1)
    {
        m_nExpired = 1;
    }

}

void ActShockwave::UpdateWave(int nWave, float fWaveTime)
{
    if (m_arTarget[nWave] == -1)
    {
        return;
    }
#if defined (SM_SERVER)

    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);

    // Process first wave
    if (fWaveTime >= HIT_TIME && 
        m_arHit[nWave] == 0)
    {
        if(pTheGame->m_arTiles[m_arTarget[nWave]][m_nCastZ].GetPawn() != 0)
        {
            pTheGame->m_arTiles[m_arTarget[nWave]][m_nCastZ].GetPawn()->Damage(DAMAGE);
        }

        m_arHit[nWave] = 1;
    }
#else
    if (fWaveTime < HIT_TIME)
    {
        m_arMaterials[nWave]->SetColor(1.0f, 1.0f, 1.0f, fWaveTime/HIT_TIME);
    }
    else
    {
        m_arMaterials[nWave]->SetColor(1.0f, 1.0f, 1.0f, 1.0f - (fWaveTime - HIT_TIME)/HIT_TIME);
    }

    m_arMatter[nWave]->SetRotation(0.0f, fWaveTime * ROT_SPEED_Y, 0.0f);

#endif

    // Pick a new target if it's time
    if (fWaveTime > HIT_TIME*2.0f)
    {
        // Restart timer
        m_arWaveTimers[nWave].Start();

        // Reset hit counter
        m_arHit[nWave] = 0;

        // Update the target
        m_arTarget[nWave] += (m_nCaster ? -2 : 2);

        if (m_arTarget[nWave] >= GRID_WIDTH ||
            m_arTarget[nWave] < 0)
        {
            m_arTarget[nWave] = -1;
        }

#if defined (SM_CLIENT)
        if(m_arTarget[nWave] != -1)
        {
            m_arMatter[nWave]->SetPosition(static_cast<float>(m_arTarget[nWave]) * TILE_WIDTH,
                                           0.0f,
                                           static_cast<float>(m_nCastZ) * TILE_HEIGHT);
        }
        else
        {
            m_arMatter[nWave]->SetPosition(-9001.0f, 0.0f, 0.0f);
        }
#endif
    }
}

void ActShockwave::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    for (int i = 0; i < 2; i++)
    {
        reinterpret_cast<Game*>(m_pGame)->GetScene()->RemoveMatter(m_arMatter[i]);
        
        delete m_arMaterials[i];
        m_arMaterials[i] = 0;

        delete m_arMatter[i];
        m_arMatter[i] = 0;
    }
#endif
}
