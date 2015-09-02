#include "CardBullet.h"

#define CARD_BULLET_TEXTURE g_pCardBulletTex
#define CARD_BULLET_NAME "BULLET"
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

char* CardBullet::GetName()
{
    return CARD_BULLET_NAME;
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

void CardBullet::Cast(void* pGame)
{
    LogDebug("Implement CardBullet::Cast()!");
}
