#include "ActBubble.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define DAMAGE 20
#define SPEED_X 3.0f
#define STUN_DURATION 3.5f

ActBubble::ActBubble()
{
    m_nType = CARD_BUBBLE;

    m_fPosX = 0;
    m_fPosY = 0;
    m_fPosZ = 0;

    m_nHit = 0;

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
    m_pMaterial = new DiffuseMaterial();
    m_pMaterial->SetColor(0.0f, 0.2f, 0.8f, 0.5f);

    m_pMatter = new Matter();
    m_pMatter->SetMaterial(m_pMaterial);
    // m_pMatter->SetMesh(g_pSphere);
#else

#endif
}
