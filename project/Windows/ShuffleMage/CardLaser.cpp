#include "CardLaser.h"
#include "ActLaser.h"

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
#if defined(SM_CLIENT)
    return CARD_LASER_TEXTURE;
#else
    return 0;
#endif
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

void CardLaser::Cast(void* pGame, int nCaster)
{
    Activation* pAct = new ActLaser();
    if (SpawnActivation(pGame, nCaster, pAct) == -1)
    {
        // Couldn't create activation.
        delete pAct;
    }
}
