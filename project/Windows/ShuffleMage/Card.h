#ifndef CARD_H
#define CARD_H

#include "Texture.h"
#include "Resources.h"
#include "Log.h"

enum CardIDEnum
{
    CARD_BULLET = 0
};

class Card
{
public:

    Card(){};
    virtual ~Card(){};


    virtual Texture* GetTexture()  = 0;
    virtual char* GetName()        = 0;
    virtual int GetID()            = 0;
    virtual int GetManaCost()      = 0;
    virtual int GetRarity()        = 0;
    virtual void Cast(void* pGame) = 0;
};

#endif