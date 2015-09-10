#ifndef CARD_LASER_H
#define CARD_LASER_H

#include "Card.h"

class CardLaser : public Card
{
public:
    CardLaser();
    ~CardLaser();

    Texture* GetTexture();
    const char* GetName();
    const char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif