#include "HUD.h"
#include "Resources.h"
#include <stdio.h>

#define HAND_BACK_X -0.4f
#define HAND_BACK_Y 0.6f
#define HAND_BACK_WIDTH 0.8f
#define HAND_BACK_HEIGHT 0.3f
#define HAND_CARD_X_OFFSET 0.031f
#define HAND_CARD_Y_OFFSET 0.016f
#define HAND_CARD_WIDTH 0.12f
#define HAND_CARD_HEIGHT 0.12f

HUD::HUD()
{
    int i = 0;

    m_pGame = 0;

    // Not using constants for these values because...
    // This should really be the only place these colors
    // are referenced. Can't think of any other time that
    // something will need these color values.
    m_quadHandBack.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_quadDrawBar.SetColor(0.6f, 0.8f, 0.6f, 1.0f);
    m_quadManaBar.SetColor(0.3f, 0.3f, 0.6f, 1.0f);
    
    m_quadHealth.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_quadHealth.SetBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_quadEnemyHealth.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_quadEnemyHealth.SetBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_textHealth.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_textEnemyHealth.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    

    // Next load textures for quads that need them
    m_quadCast.SetTexture(g_pCastTex);
    m_quadRotate.SetTexture(g_pRotateTex);

    // Next, set position values
    m_quadHandBack.SetBox(HAND_BACK_X,
                          HAND_BACK_Y,
                          HAND_BACK_WIDTH,
                          HAND_BACK_HEIGHT);
    m_quadDrawBar.SetBox(HAND_BACK_X,
                         HAND_BACK_Y + HAND_BACK_HEIGHT * 0.75f,
                         HAND_BACK_WIDTH,
                         HAND_BACK_HEIGHT * 0.25f);
    m_quadManaBar.SetBox(HAND_BACK_X,
                         HAND_BACK_Y,
                         HAND_BACK_WIDTH,
                         HAND_BACK_HEIGHT * 0.25f);
    
    for (i = 0; i < HAND_SIZE; i++)
    {
        m_arHandCards[i].SetBox(HAND_BACK_X + i*(HAND_CARD_X_OFFSET + HAND_CARD_WIDTH) + HAND_CARD_X_OFFSET,
                                HAND_BACK_Y + HAND_CARD_Y_OFFSET + 0.25f*HAND_BACK_HEIGHT,
                                HAND_CARD_WIDTH,
                                HAND_CARD_HEIGHT);
    }

    m_quadHealth.SetBox(-0.9f, 0.75f, 0.3f, 0.14f);
    m_quadEnemyHealth.SetBox(0.6f, 0.75f, 0.3f, 0.14f);
    m_textHealth.SetPosition(-0.86f, 0.79f);
    m_textEnemyHealth.SetPosition(0.64f, 0.79f);

    m_rectCast.m_fX      = 0.35f;
    m_rectCast.m_fY      = -0.9f;
    m_rectCast.m_fWidth  = 0.25f;
    m_rectCast.m_fHeight = 0.25f;

    m_rectRotate.m_fX      = 0.68f;
    m_rectRotate.m_fY      = -0.9f;
    m_rectRotate.m_fWidth  = 0.25f;
    m_rectRotate.m_fHeight = 0.25f;

    m_quadCast.SetBox(m_rectCast.m_fX, 
                      m_rectCast.m_fY,
                      m_rectCast.m_fWidth,
                      m_rectCast.m_fHeight);
    m_quadRotate.SetBox(m_rectRotate.m_fX, 
                        m_rectRotate.m_fY,
                        m_rectRotate.m_fWidth,
                        m_rectRotate.m_fHeight);

    // Set default text strings, just so we can see something
    m_textHealth.SetText("500");
    m_textEnemyHealth.SetText("500");
    m_textHealth.SetScale(1.4f, 1.2f);
    m_textEnemyHealth.SetScale(1.4f, 1.2f);

    // Enable borders where needed.
    m_quadHealth.EnableBorder(1);
    m_quadEnemyHealth.EnableBorder(1);
}

HUD::~HUD()
{

}

void HUD::Register(Scene* pScene)
{
    int i = 0;

    pScene->AddGlyph(&m_quadHandBack);

    for (i = 0; i < HAND_SIZE; i++)
    {
        pScene->AddGlyph(&m_arHandCards[i]);
    }
    
    
    pScene->AddGlyph(&m_quadDrawBar);
    pScene->AddGlyph(&m_quadManaBar);

    pScene->AddGlyph(&m_quadHealth);
    pScene->AddGlyph(&m_quadEnemyHealth);
    pScene->AddGlyph(&m_textHealth);
    pScene->AddGlyph(&m_textEnemyHealth);

    pScene->AddGlyph(&m_quadCast);
    pScene->AddGlyph(&m_quadRotate);
}

void HUD::SetEnable(int nEnable)
{
    int i = 0;

    for (i = 0; i < HAND_SIZE; i++)
    {
        m_arHandCards[i].SetVisible(nEnable);
    }

    m_quadDrawBar.SetVisible(nEnable);
    m_quadManaBar.SetVisible(nEnable);
    m_quadHandBack.SetVisible(nEnable);

    m_quadHealth.SetVisible(nEnable);
    m_quadEnemyHealth.SetVisible(nEnable);
    m_quadCast.SetVisible(nEnable);
    m_quadRotate.SetVisible(nEnable);

    m_textHealth.SetVisible(nEnable);
    m_textEnemyHealth.SetVisible(nEnable);
}

void HUD::SetGame(void* pGame)
{
    m_pGame = pGame;
}

int HUD::IsCastPressed(float fX, float fY)
{
    return m_rectCast.Contains(fX, fY);
}

int HUD::IsRotatePressed(float fX, float fY)
{
    return m_rectRotate.Contains(fX, fY);
}

void HUD::SetHandTextures(Card** arHand)
{
    int i = 0;

    for (i = 0; i < HAND_SIZE; i++)
    {
        if (arHand[i] != 0)
        {
            m_arHandCards[i].SetTexture(arHand[i]->GetTexture());
            m_arHandCards[i].SetVisible(1);
        }
        else
        {
            m_arHandCards[i].SetTexture(0);
            m_arHandCards[i].SetVisible(0);
        }
    }
}

void HUD::SetHealth(int nHealth)
{
    char arString[8] = {0};
    sprintf(arString, "%d", nHealth);
    m_textHealth.SetText(arString);
}

void HUD::SetEnemyHealth(int nHealth)
{
    char arString[8] = {0};
    sprintf(arString, "%d", nHealth);
    m_textEnemyHealth.SetText(arString);
}

void HUD::SetMana(int nMana)
{
    m_quadManaBar.SetWidth((static_cast<float>(nMana)/DEFAULT_MAGE_MANA) * HAND_BACK_WIDTH);
}

void HUD::SetDrawCharge(int nDrawCharge)
{
    m_quadDrawBar.SetWidth((static_cast<float>(nDrawCharge)/DEFAULT_MAGE_MANA) * HAND_BACK_WIDTH);
}