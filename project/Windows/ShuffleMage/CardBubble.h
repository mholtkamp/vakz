#ifndef CARD_BUBBLE_H
#define CARD_BUBBLE_H

#include "Constants.h"

#include "Card.h"

class CardBubble : public Card
{
public:

    CardBubble();
    ~CardBubble();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif