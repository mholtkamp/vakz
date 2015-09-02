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
    char* GetName();
    char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif