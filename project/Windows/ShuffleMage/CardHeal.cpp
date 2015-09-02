#include "CardHeal.h"

#define CARD_HEAL_TEXTURE g_pCardHealTex
#define CARD_HEAL_NAME "HEAL"
#define CARD_HEAL_DESCRIPTION "Restores mage's health"
#define CARD_HEAL_MANA 30
#define CARD_HEAL_RARITY RARITY_COMMON


CardHeal::CardHeal()
{

}

CardHeal::~CardHeal()
{

}

Texture* CardHeal::GetTexture()
{
    return CARD_HEAL_TEXTURE;
}

char* CardHeal::GetName()
{
    return CARD_HEAL_NAME;
}

char* CardHeal::GetDescription()
{
    return CARD_HEAL_DESCRIPTION;
}

int CardHeal::GetID()
{
    return CARD_HEAL;
}

int CardHeal::GetManaCost()
{
    return CARD_HEAL_MANA;
}

int CardHeal::GetRarity()
{
    return CARD_HEAL_RARITY;
}

void CardHeal::Cast(void* pGame)
{
    LogDebug("Implement CardHeal::Cast()!");
}