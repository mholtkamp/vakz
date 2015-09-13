#include "Activation.h"
#include "Game.h"

Activation::Activation()
{
    m_nActivityIndex = 0;
    m_nExpired       = 0;
    m_nCaster        = 0;
    m_pGame          = 0;
}

Activation::~Activation()
{

}

void Activation::Update()
{
    m_timer.Stop();
}

void Activation::OnCreate(void* pGame,
                          int   nIndex,
                          int   nCaster)
{
    m_pGame           = pGame;
    m_nActivityIndex  = nIndex;
    m_nCaster         = nCaster;
    m_timer.Start();
}

void Activation::OnCasterDamage(int nDamage)
{

}

void Activation::OnEnemyDamage(int nDamage)
{

}

void Activation::OnDestroy()
{
#if defined(SM_CLIENT)
    reinterpret_cast<Game*>(m_pGame)->m_arActMatters[m_nActivityIndex].SetMesh(0);
    reinterpret_cast<Game*>(m_pGame)->m_arActMatters[m_nActivityIndex].SetTexture(0);
    reinterpret_cast<Game*>(m_pGame)->m_arActMatters[m_nActivityIndex].SetMaterial(0);
#endif
}