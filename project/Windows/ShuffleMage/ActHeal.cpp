#include "ActHeal.h"
#include "Resources.h"

#if defined SM_SERVER
#include "ServerGame.h"
#else
#include "Game.h"
#endif

#define LIFETIME 0.7f
#define HEAL_AMOUNT 20
#define SPEED_Y 1.0f
#define OFFSET_Y 1.0f

ActHeal::ActHeal()
{
    m_nType = CARD_HEAL;

    m_fPosX = 0.0f;
    m_fPosY = 0.0f;
    m_fPosZ = 0.0f;

    m_pMatter   = 0;
    m_pMaterial = 0;
}

ActHeal::~ActHeal()
{

}

void ActHeal::OnCreate(void* pGame,
                       int   nIndex,
                       int   nCaster)
{
    Activation::OnCreate(pGame,
                         nIndex,
                         nCaster);

#if defined (SM_CLIENT)

    int nCastX;
    int nCastZ;

    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pTheGame->GetMage(nCaster)->GetPosition(nCastX, nCastZ);

    m_fPosX = static_cast<float>(nCastX) * TILE_WIDTH;
    m_fPosY = OFFSET_Y;
    m_fPosZ = static_cast<float>(nCastZ) * TILE_HEIGHT;

    m_pMaterial = new FullbrightMaterial();
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    m_pMatter = new Matter();
    m_pMatter->SetMesh(g_pPlaneXY);
    m_pMatter->SetMaterial(m_pMaterial);
    m_pMatter->SetTexture(g_pActHealTex);
    m_pMatter->SetPosition(m_fPosX,
                           m_fPosY,
                           m_fPosZ);

    pTheGame->GetScene()->AddActor(m_pMatter);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);
    pTheGame->GetMage(nCaster)->Heal(HEAL_AMOUNT);
    m_nExpired = 1;
#endif
}


void ActHeal::OnDestroy()
{
    Activation::OnDestroy();

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pTheGame->GetScene()->RemoveMatter(m_pMatter);
    delete m_pMatter;
    m_pMatter = 0;
    delete m_pMaterial;
    m_pMaterial = 0;
#endif
}

void ActHeal::Update()
{
    Activation::Update();
    float fTime = 0.0f;

    fTime = m_timer.Time();

#if defined (SM_CLIENT)
    m_pMatter->SetPosition(m_fPosX,
                           m_fPosY + fTime*SPEED_Y,
                           m_fPosZ);
    m_pMaterial->SetColor(1.0f,
                          1.0f,
                          1.0f,
                          (LIFETIME - fTime)/LIFETIME);

    if (fTime >= LIFETIME)
    {
        m_nExpired = 1;
    }
#endif
}
