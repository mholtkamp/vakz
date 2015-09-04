#include "Mage.h"
#include "Constants.h"
#include "Resources.h"
#include "Log.h"

Mage::Mage()
{
    m_nHealth = DEFAULT_MAGE_HEALTH;

    m_nConfused   = 0;
    m_nPoisoned   = 0;
    m_nFloating   = 0;
    m_nInvisible  = 0;
    m_nInvincible = 0;
    m_nStunned    = 0;
    m_nRooted     = 0;

#ifdef SM_CLIENT
    m_matter.SetMesh(g_pMageAnimatedMesh);
    m_matter.SetLoopMode(Matter::LOOP_PING_PONG);
    m_matter.SetAnimation("Idle");
    m_matter.StartAnimation();
    m_matter.SetScale(MAGE_SCALE_XYZ, MAGE_SCALE_XYZ, MAGE_SCALE_XYZ);
#endif
}

Mage::Mage(int nSide) : Pawn(nSide)
{
    m_nHealth = DEFAULT_MAGE_HEALTH;

    m_nConfused   = 0;
    m_nPoisoned   = 0;
    m_nFloating   = 0;
    m_nInvisible  = 0;
    m_nInvincible = 0;
    m_nStunned    = 0;
    m_nRooted     = 0;
    
#ifdef SM_CLIENT
    m_matter.SetMesh(g_pMageAnimatedMesh);
    m_matter.SetLoopMode(Matter::LOOP_PING_PONG);
    m_matter.SetAnimation("Idle");
    m_matter.StartAnimation();
    m_matter.SetScale(MAGE_SCALE_XYZ, MAGE_SCALE_XYZ, MAGE_SCALE_XYZ);
#endif
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

void Mage::SetPosition(int nX,
                       int nZ)
{
    Pawn::SetPosition(nX, nZ);

#ifdef SM_CLIENT
        m_matter.Translate(0.0f, MAGE_Y_OFFSET, 0.0f);
#endif
}

void Mage::UpdatePosition(int nX,
                          int nZ)
{
    Pawn::UpdatePosition(nX, nZ);

#ifdef SM_CLIENT
        m_matter.Translate(0.0f, MAGE_Y_OFFSET, 0.0f);
#endif
}
