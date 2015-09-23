#ifndef MAGE_H
#define MAGE_H

#include "Pawn.h"

class Mage : public Pawn
{
public:

    Mage();
    Mage(int nSide);
    ~Mage();

    void SetPosition(int nX,
                     int nZ);

    void UpdatePosition(int nX,
                        int nZ);

    void PlayCastAnimation();

    int  GetMana();
    void SetMana(int nMana);
    void Replenish(int nMana);
    void Drain(int nMana);

    void RegenMana();
    void RegenDrawCharge();

    void SetManaRegenRate(int nManaRegenRate);
    void SetDrawChargeRegenRate(int nDrawChargeRegenRate);

    int GetDrawCharge();

    float GetMoveTime();

    //void SetSide(int nSide);

    // Status Getters
    int IsConfused();
    int IsPoisoned();
    int IsFloating();
    int IsInvisible();
    int IsInvincible();
    int IsStunned();
    int IsRooted();

    // Status Setters
    void SetConfused(int nConfused);
    void SetPoisoned(int nPoisoned);
    void SetFloating(int nFloating);
    void SetInvisible(int nInvisible);
    void SetInvincible(int nInvincible);
    void SetStunned(int nStunned);
    void SetRooted(int nRooted);

private:

    float m_fMoveTime;

    int m_nMana;
    int m_nDrawCharge;

    int m_nManaRegenRate;
    int m_nDrawRegenRate;

    // Statuses
    int m_nConfused;
    int m_nPoisoned;
    int m_nFloating;
    int m_nInvisible;
    int m_nInvincible;
    int m_nStunned;
    int m_nRooted;
};

#endif