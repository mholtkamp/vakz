#ifndef CARD_H
#define CARD_H

#include "Texture.h"
#include "Resources.h"
#include "Log.h"
#include "Constants.h"

enum CardIDEnum
{
    CARD_BULLET    = 0,
    CARD_LASER     = 1,
    CARD_HEAL      = 2,
    CARD_INVIS     = 3,
    CARD_SHOCKWAVE = 4,
    CARD_BOMB      = 5,
    CARD_SPEARS    = 6,
    CARD_BUBBLE    = 7,
    CARD_SWIPE     = 8,
    CARD_AREAGRAB  = 9
};

class Card
{
public:

    Card(){};
    virtual ~Card(){};


    virtual Texture* GetTexture()  = 0;
    virtual char* GetName()        = 0;
    virtual char* GetDescription() = 0;
    virtual int GetID()            = 0;
    virtual int GetManaCost()      = 0;
    virtual int GetRarity()        = 0;
    virtual void Cast(void* pGame) = 0;
};

#endif