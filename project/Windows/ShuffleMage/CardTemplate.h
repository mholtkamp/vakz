/*
INSTRUCTIONS: Replace XCARD with cased card name, eg. Bullet, Laser, Invis
              Replace DCARD with the all caps name, eg. BULLET, LASER, INVIS



#include "CardXCARD.h"

#define CARD_DCARD_TEXTURE g_pCardXCARDTex
#define CARD_DCARD_NAME "DCARD"
#define CARD_DCARD_DESCRIPTION "Description"
#define CARD_DCARD_MANA 30
#define CARD_DCARD_RARITY RARITY_COMMON


CardXCARD::CardXCARD()
{

}

CardXCARD::~CardXCARD()
{

}

Texture* CardXCARD::GetTexture()
{
    return CARD_DCARD_TEXTURE;
}

const char* CardXCARD::GetName()
{
    return CARD_DCARD_NAME;
}

const char* CardXCARD::GetDescription()
{
    return CARD_DCARD_DESCRIPTION;
}

int CardXCARD::GetID()
{
    return CARD_DCARD;
}

int CardXCARD::GetManaCost()
{
    return CARD_DCARD_MANA;
}

int CardXCARD::GetRarity()
{
    return CARD_DCARD_RARITY;
}

void CardXCARD::Cast(void* pGame)
{
    LogDebug("Implement CardXCARD::Cast()!");
}

*/