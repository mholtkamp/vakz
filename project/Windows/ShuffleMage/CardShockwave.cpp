#include "CardShockwave.h"

#define CARD_SHOCKWAVE_TEXTURE g_pCardShockwaveTex
#define CARD_SHOCKWAVE_NAME "SHOCKWAVE"
#define CARD_SHOCKWAVE_DESCRIPTION "Creates a shockwave along row, damaging enemies and cracking tiles"
#define CARD_SHOCKWAVE_MANA 40
#define CARD_SHOCKWAVE_RARITY RARITY_COMMON


CardShockwave::CardShockwave()
{

}

CardShockwave::~CardShockwave()
{

}

Texture* CardShockwave::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_SHOCKWAVE_TEXTURE;
#else
    return 0;
#endif
}

const char* CardShockwave::GetName()
{
    return CARD_SHOCKWAVE_NAME;
}

const char* CardShockwave::GetDescription()
{
    return CARD_SHOCKWAVE_DESCRIPTION;
}

int CardShockwave::GetID()
{
    return CARD_SHOCKWAVE;
}

int CardShockwave::GetManaCost()
{
    return CARD_SHOCKWAVE_MANA;
}

int CardShockwave::GetRarity()
{
    return CARD_SHOCKWAVE_RARITY;
}

void CardShockwave::Cast(void* pGame, int nCaster)
{
    LogDebug("Implement CardShockwave::Cast()!");
}