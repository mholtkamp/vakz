#ifndef CARD_BOMB_H
#define CARD_BOMB_H

#include "Constants.h"

#include "Card.h"

class CardBomb : public Card
{
public:

    CardBomb();
    ~CardBomb();

    Texture* GetTexture();
    char* GetName();
    char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif