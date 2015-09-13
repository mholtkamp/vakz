#include "CardBomb.h"

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
    return CARD_BOMB_TEXTURE;
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
    LogDebug("Implement CardBomb::Cast()!");
}