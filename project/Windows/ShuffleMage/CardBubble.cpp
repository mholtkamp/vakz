#include "CardBubble.h"
#include "ActBubble.h"

#define CARD_BUBBLE_TEXTURE g_pCardBubbleTex
#define CARD_BUBBLE_NAME "BUBBLE"
#define CARD_BUBBLE_DESCRIPTION "Shoots a bubble that stuns target"
#define CARD_BUBBLE_MANA 30
#define CARD_BUBBLE_RARITY RARITY_COMMON


CardBubble::CardBubble()
{

}

CardBubble::~CardBubble()
{

}

Texture* CardBubble::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_BUBBLE_TEXTURE;
#else
    return 0;
#endif
}

const char* CardBubble::GetName()
{
    return CARD_BUBBLE_NAME;
}

const char* CardBubble::GetDescription()
{
    return CARD_BUBBLE_DESCRIPTION;
}

int CardBubble::GetID()
{
    return CARD_BUBBLE;
}

int CardBubble::GetManaCost()
{
    return CARD_BUBBLE_MANA;
}

int CardBubble::GetRarity()
{
    return CARD_BUBBLE_RARITY;
}

void CardBubble::Cast(void* pGame, int nCaster)
{
    Activation* pAct = new ActBubble();
    if (SpawnActivation(pGame, nCaster, pAct) == -1)
    {
        // Couldn't create activation.
        delete pAct;
    }
}