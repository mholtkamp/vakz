#ifndef MAGE_H
#define MAGE_H

#include "Pawn.h"

class Mage : public Pawn
{
public:

    Mage(int nSide);
    ~Mage();

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