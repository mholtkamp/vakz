#include "CardBomb.h"
#include "ActBomb.h"

#define CARD_BOMB_TEXTURE g_pCardBombTex
#define CARD_BOMB_NAME "BOMB"
#define CARD_BOMB_DESCRIPTION "Throws bomb that lands 3 cells ahead"
#define CARD_BOMB_MANA 20
#define CARD_BOMB_RARITY RARITY_COMMON


CardBomb::CardBomb()
{
    
}

CardBomb::~CardBomb()
{

}

Texture* CardBomb::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_BOMB_TEXTURE;
#else
    return 0;
#endif
}

const char* CardBomb::GetName()
{
    return CARD_BOMB_NAME;
}

const char* CardBomb::GetDescription()
{
    return CARD_BOMB_DESCRIPTION;
}

int CardBomb::GetID()
{
    return CARD_BOMB;
}

int CardBomb::GetManaCost()
{
    return CARD_BOMB_MANA;
}

int CardBomb::GetRarity()
{
    return CARD_BOMB_RARITY;
}

void CardBomb::Cast(void* pGame, int nCaster)
{
    Activation* pAct = new ActBomb();
    if (SpawnActivation(pGame, nCaster, pAct) == -1)
    {
        // Couldn't create activation.
        delete pAct;
    }
}