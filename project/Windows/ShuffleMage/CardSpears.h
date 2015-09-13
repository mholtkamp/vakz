#ifndef CARD_SPEARS_H
#define CARD_SPEARS_H

#include "Constants.h"

#include "Card.h"

class CardSpears : public Card
{
public:

    CardSpears();
    ~CardSpears();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif