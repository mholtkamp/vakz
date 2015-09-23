#include "Mage.h"
#include "Constants.h"
#include "Resources.h"
#include "Log.h"

Mage::Mage()
{
    m_fMoveTime = DEFAULT_MOVE_TIME;

    m_nHealth = DEFAULT_MAGE_HEALTH;
    m_nMana   = DEFAULT_MAGE_MANA;
    m_nDrawCharge = DEFAULT_DRAW_CHARGE;

    m_nDrawRegenRate = DEFAULT_DRAW_CHARGE_REGEN_RATE;
    m_nManaRegenRate = DEFAULT_MAGE_MANA_REGEN_RATE;

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
    m_fMoveTime = DEFAULT_MOVE_TIME;

    m_nHealth = DEFAULT_MAGE_HEALTH;
    m_nMana   = DEFAULT_MAGE_MANA;
    m_nDrawCharge = DEFAULT_DRAW_CHARGE;

    m_nDrawRegenRate = DEFAULT_DRAW_CHARGE_REGEN_RATE;
    m_nManaRegenRate = DEFAULT_MAGE_MANA_REGEN_RATE;

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

int  Mage::GetMana()
{
    return m_nMana;
}

void Mage::SetMana(int nMana)
{
    m_nMana = nMana;
}

void Mage::Replenish(int nMana)
{
    m_nMana += nMana;
    if (m_nMana > DEFAULT_MAGE_MANA)
    {
        m_nMana = DEFAULT_MAGE_MANA;
    }
}

void Mage::Drain(int nMana)
{
    m_nMana -= nMana;
    if (m_nMana < 0)
    {
        m_nMana = 0;
    }
}

void Mage::RegenMana()
{
    m_nMana += m_nManaRegenRate;

    if (m_nMana >= MAX_MAGE_MANA)
    {
        m_nMana = MAX_MAGE_MANA;
    }
}

void Mage::RegenDrawCharge()
{
    m_nDrawCharge += m_nDrawRegenRate;

    if (m_nDrawCharge >= MAX_DRAW_CHARGE)
    {
        m_nDrawCharge = MAX_DRAW_CHARGE;
    }
}

void Mage::SetManaRegenRate(int nManaRegenRate)
{
    m_nManaRegenRate = nManaRegenRate;
}

void Mage::SetDrawChargeRegenRate(int nDrawChargeRegenRate)
{
    m_nDrawRegenRate = nDrawChargeRegenRate;
}

int Mage::GetDrawCharge()
{
    return m_nDrawCharge;
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

void Mage::PlayCastAnimation()
{
#if defined SM_CLIENT
    m_matter.PlayAnimationOnce("Cast");
#endif
}

float Mage::GetMoveTime()
{
    return m_fMoveTime;
}