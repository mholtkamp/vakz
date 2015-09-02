#ifndef CARD_LASER_H
#define CARD_LASER_H

#include "Card.h"

class CardLaser : public Card
{

    CardLaser();
    ~CardLaser();

    Texture* GetTexture();
    char* GetName();
    char* GetDescription();
    int GetID();
    int GetManaCost();
    int GetRarity();
    void Cast(void* pGame);
};

#endif