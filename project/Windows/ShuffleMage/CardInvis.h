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
    char* GetName();
    char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif