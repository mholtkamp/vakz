#include "Mage.h"

#define DEFAULT_PLAYER_HEALTH 500

Mage::Mage()
{
    m_nHealth = DEFAULT_PLAYER_HEALTH;

    m_nConfused   = 0;
    m_nPoisoned   = 0;
    m_nFloating   = 0;
    m_nInvisible  = 0;
    m_nInvincible = 0;
    m_nStunned    = 0;
    m_nRooted     = 0;
}

Mage::Mage(int nSide) : Pawn(nSide)
{
    Mage();
}

Mage::~Mage()
{

}

int Mage::IsConfused()
{
    return m_nConfused;
}
int Mage::IsPoisoned()
{
    return m_nPoisoned;
}
int Mage::IsFloating()
{
    return m_nFloating;
}
int Mage::IsInvisible()
{
    return m_nInvisible;
}
int Mage::IsInvincible()
{
    return m_nInvincible;
}
int Mage::IsStunned()
{
    return m_nStunned;
}
int Mage::IsRooted()
{
    return m_nRooted;
}


void Mage::SetConfused(int nConfused)
{
    m_nConfused = nConfused;
}
void Mage::SetPoisoned(int nPoisoned)
{
    m_nPoisoned = nPoisoned;
}
void Mage::SetFloating(int nFloating)
{
    m_nFloating = nFloating;
}
void Mage::SetInvisible(int nInvisible)
{
    m_nInvisible = nInvisible;
}
void Mage::SetInvincible(int nInvincible)
{
    m_nInvincible = nInvincible;
}
void Mage::SetStunned(int nStunned)
{
    m_nStunned = nStunned;
}
void Mage::SetRooted(int nRooted)
{
    m_nRooted = nRooted;
}
