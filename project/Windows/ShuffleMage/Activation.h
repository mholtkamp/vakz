#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Timer.h"

class Activation
{

public:

    Activation();
    virtual ~Activation();

    virtual void Update();
    virtual void OnCreate(void* pGame,
                          int   nIndex,
                          int   nCaster);
    virtual void OnCasterDamage(int nDamage);
    virtual void OnEnemyDamage(int nDamage);
    virtual void OnDestroy();

    int   m_nType;
    int   m_nActivityIndex;
    int   m_nExpired;
    int   m_nCaster;
    void* m_pGame;
    Timer m_timer;


};

#endif