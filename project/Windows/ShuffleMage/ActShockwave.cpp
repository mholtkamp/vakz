#include "ActShockwave.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 40
#define ROT_SPEED_Y 70.0f
#define HIT_TIME 0.6f

ActShockwave::ActShockwave()
{
    m_nType = CARD_SHOCKWAVE;

    m_nCastX = 0;
    m_nCastZ = 0;

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
        m_arMaterials[i] = new DiffuseMaterial();
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
            m_arMatter[i]->SetPosition(static_cast<float>(m_arTarget[i]),
                                       0.0f,
                                       static_cast<float>(m_nCastZ));
        }
        
        pTheGame->GetScene()->AddMatter(m_arMatter[i]);
#endif
    }
}

void ActShockwave::Update()
{
    Activation::Update();
}
