#ifndef CARD_INVIS_H
#define CARD_INVIS_H

#include "Constants.h"

#include "Card.h"

class CardInvis : public Card
{
public:

    CardInvis();
    ~CardInvis();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif