#ifndef HUD_H
#define HUD_H

#include "Constants.h"
#include "Scene.h"
#include "Quad.h"
#include "Text.h"

class HUD
{
public:

    HUD();
    ~HUD();

    void Update();

    void Register(Scene* pScene);
    void SetEnable(int nEnable);
    void SetGame(void* pGame);


private:

    Quad m_arHandCards[HAND_SIZE];

    Quad m_quadDrawBar;
    Quad m_quadManaBar;
    Quad m_quadHandBack;

    Quad m_quadHealth;
    Text m_textHealth;

    Quad m_quadEnemyHealth;
    Text m_textEnemyText;

    Quad m_quadCast;
    Quad m_quadRotate;

};

#endif