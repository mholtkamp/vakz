#ifndef CARD_HEAL_H
#define CARD_HEAL_H

#include "Constants.h"

#include "Card.h"

class CardHeal : public Card
{
public:

    CardHeal();
    ~CardHeal();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif