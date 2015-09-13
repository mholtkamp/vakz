#include "CardInvis.h"

#define CARD_INVIS_TEXTURE g_pCardInvisTex
#define CARD_INVIS_NAME "INVIS"
#define CARD_INVIS_DESCRIPTION "Temporarily makes mage invisible"
#define CARD_INVIS_MANA 60
#define CARD_INVIS_RARITY RARITY_COMMON


CardInvis::CardInvis()
{

}

CardInvis::~CardInvis()
{

}

Texture* CardInvis::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_INVIS_TEXTURE;
#else
    return 0;
#endif
}

const char* CardInvis::GetName()
{
    return CARD_INVIS_NAME;
}

const char* CardInvis::GetDescription()
{
    return CARD_INVIS_DESCRIPTION;
}

int CardInvis::GetID()
{
    return CARD_INVIS;
}

int CardInvis::GetManaCost()
{
    return CARD_INVIS_MANA;
}

int CardInvis::GetRarity()
{
    return CARD_INVIS_RARITY;
}

void CardInvis::Cast(void* pGame, int nCaster)
{
    LogDebug("Implement CardInvis::Cast()!");
}