#ifndef CARD_BULLET_H
#define CARD_BULLET_H

#include "Constants.h"

#include "Card.h"

class CardBullet : public Card
{
public:

    CardBullet();
    ~CardBullet();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif