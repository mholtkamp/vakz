#include "CardBullet.h"

#define CARD_BULLET_TEXTURE g_pCardBulletTex
#define CARD_BULLET_NAME "BULLET"
#define CARD_BULLET_DESCRIPTION "Fires bullet that damages first enemy it hits"
#define CARD_BULLET_MANA 30
#define CARD_BULLET_RARITY RARITY_COMMON


CardBullet::CardBullet()
{

}

CardBullet::~CardBullet()
{

}

Texture* CardBullet::GetTexture()
{
    return CARD_BULLET_TEXTURE;
}

const char* CardBullet::GetName()
{
    return CARD_BULLET_NAME;
}

const char* CardBullet::GetDescription()
{
    return CARD_BULLET_DESCRIPTION;
}

int CardBullet::GetID()
{
    return CARD_BULLET;
}

int CardBullet::GetManaCost()
{
    return CARD_BULLET_MANA;
}

int CardBullet::GetRarity()
{
    return RARITY_COMMON;
}

void CardBullet::Cast(void* pGame, int nCaster)
{
    LogDebug("Implement CardBullet::Cast()!");
}
