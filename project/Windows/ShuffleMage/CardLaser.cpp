#include "CardLaser.h"

#define CARD_LASER_TEXTURE g_pCardLaserTex
#define CARD_LASER_NAME "LASER"
#define CARD_LASER_DESCRIPTION "Fires laser that damages all enemies on the same row"
#define CARD_LASER_MANA 30
#define CARD_LASER_RARITY RARITY_COMMON


CardLaser::CardLaser()
{

}

CardLaser::~CardLaser()
{

}

Texture* CardLaser::GetTexture()
{
    return CARD_LASER_TEXTURE;
}

const char* CardLaser::GetName()
{
    return CARD_LASER_NAME;
}

const char* CardLaser::GetDescription()
{
    return CARD_LASER_DESCRIPTION;
}

int CardLaser::GetID()
{
    return CARD_LASER;
}

int CardLaser::GetManaCost()
{
    return CARD_LASER_MANA;
}

int CardLaser::GetRarity()
{
    return CARD_LASER_RARITY;
}

void CardLaser::Cast(void* pGame)
{
    LogDebug("Implement CardLaser::Cast()!");
}
