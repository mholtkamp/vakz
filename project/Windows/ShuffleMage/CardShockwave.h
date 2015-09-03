#ifndef CARD_SHOCKWAVE_H
#define CARD_SHOCKWAVE_H

#include "Constants.h"

#include "Card.h"

class CardShockwave : public Card
{
public:

    CardShockwave();
    ~CardShockwave();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif