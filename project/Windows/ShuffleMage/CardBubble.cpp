#include "CardBubble.h"

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
    return CARD_BUBBLE_TEXTURE;
}

char* CardBubble::GetName()
{
    return CARD_BUBBLE_NAME;
}

char* CardBubble::GetDescription()
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

void CardBubble::Cast(void* pGame)
{
    LogDebug("Implement CardBubble::Cast()!");
}