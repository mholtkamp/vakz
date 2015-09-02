#include "CardSpears.h"

#define CARD_SPEARS_TEXTURE g_pCardSpearsTex
#define CARD_SPEARS_NAME "SPEARS"
#define CARD_SPEARS_DESCRIPTION "Stabs enemies on back row and pushes them forward"
#define CARD_SPEARS_MANA 40
#define CARD_SPEARS_RARITY RARITY_COMMON


CardSpears::CardSpears()
{

}

CardSpears::~CardSpears()
{

}

Texture* CardSpears::GetTexture()
{
    return CARD_SPEARS_TEXTURE;
}

char* CardSpears::GetName()
{
    return CARD_SPEARS_NAME;
}

char* CardSpears::GetDescription()
{
    return CARD_SPEARS_DESCRIPTION;
}

int CardSpears::GetID()
{
    return CARD_SPEARS;
}

int CardSpears::GetManaCost()
{
    return CARD_SPEARS_MANA;
}

int CardSpears::GetRarity()
{
    return CARD_SPEARS_RARITY;
}

void CardSpears::Cast(void* pGame)
{
    LogDebug("Implement CardSpears::Cast()!");
}