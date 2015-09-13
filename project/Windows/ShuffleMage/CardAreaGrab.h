#ifndef CARD_AREAGRAB_H
#define CARD_AREAGRAB_H

#include "Constants.h"

#include "Card.h"

class CardAreaGrab : public Card
{
public:

    CardAreaGrab();
    ~CardAreaGrab();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame, int nCaster);
};

#endif