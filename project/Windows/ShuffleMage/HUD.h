#ifndef HUD_H
#define HUD_H

#include "Constants.h"
#include "Scene.h"
#include "Quad.h"
#include "Text.h"
#include "Rect.h"

class HUD
{
public:

    HUD();
    ~HUD();

    void Update();

    void Register(Scene* pScene);
    void SetEnable(int nEnable);
    void SetGame(void* pGame);

    void SetHandTextures(Texture* arHand[5]);

    int IsCastPressed(float fX,
                      float fY);
    int IsRotatePressed(float fX,
                        float fY);


private:

    void* m_pGame;

    Quad m_arHandCards[HAND_SIZE];

    Quad m_quadDrawBar;
    Quad m_quadManaBar;
    Quad m_quadHandBack;

    Quad m_quadHealth;
    Text m_textHealth;

    Quad m_quadEnemyHealth;
    Text m_textEnemyHealth;

    Quad m_quadCast;
    Quad m_quadRotate;
    Rect m_rectCast;
    Rect m_rectRotate;

};

#endif