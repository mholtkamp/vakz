#include "ActLaser.h"
#include "Game.h"

#define HIT_TIME 0.1f
#define LIFETIME 0.5f


ActLaser::ActLaser()
{
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

#if defined(SM_SERVER)
    if (fTime > HIT_TIME &&
        m_nHit == 0)
    {
        // Perform hit check and damage calculation

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
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    m_pMaterial = new DiffuseMaterial();
    m_pMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    pTheGame->m_arActMatters[nIndex].SetMaterial(m_pMaterial);
    pTheGame->m_arActMatters[nIndex].SetPosition(0.0f,0.0f,0.0f);
#endif
}

void ActLaser::OnDestroy()
{
#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(m_pGame);
    pTheGame->m_arActMatters[m_nActivityIndex].SetMaterial(0);
    delete m_pMaterial;
#endif
}