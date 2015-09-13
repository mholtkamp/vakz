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
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif