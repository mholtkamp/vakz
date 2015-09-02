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
    return CARD_SHOCKWAVE_TEXTURE;
}

char* CardShockwave::GetName()
{
    return CARD_SHOCKWAVE_NAME;
}

char* CardShockwave::GetDescription()
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

void CardShockwave::Cast(void* pGame)
{
    LogDebug("Implement CardShockwave::Cast()!");
}