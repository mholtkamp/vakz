#include "CardSwipe.h"

#define CARD_SWIPE_TEXTURE g_pCardSwipeTex
#define CARD_SWIPE_NAME "SWIPE"
#define CARD_SWIPE_DESCRIPTION "Damages enemies across 3 vertical tiles"
#define CARD_SWIPE_MANA 30
#define CARD_SWIPE_RARITY RARITY_COMMON


CardSwipe::CardSwipe()
{

}

CardSwipe::~CardSwipe()
{

}

Texture* CardSwipe::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_SWIPE_TEXTURE;
#else
    return 0;
#endif
}

const char* CardSwipe::GetName()
{
    return CARD_SWIPE_NAME;
}

const char* CardSwipe::GetDescription()
{
    return CARD_SWIPE_DESCRIPTION;
}

int CardSwipe::GetID()
{
    return CARD_SWIPE;
}

int CardSwipe::GetManaCost()
{
    return CARD_SWIPE_MANA;
}

int CardSwipe::GetRarity()
{
    return CARD_SWIPE_RARITY;
}

void CardSwipe::Cast(void* pGame, int nCaster)
{
    LogDebug("Implement CardSwipe::Cast()!");
}
