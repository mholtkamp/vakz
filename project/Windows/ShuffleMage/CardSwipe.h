#ifndef CARD_SWIPE_H
#define CARD_SWIPE_H

#include "Constants.h"

#include "Card.h"

class CardSwipe : public Card
{
public:

    CardSwipe();
    ~CardSwipe();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif