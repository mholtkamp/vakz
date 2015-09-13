#ifndef CARD_H
#define CARD_H

#include "Texture.h"
#include "Resources.h"
#include "Log.h"
#include "Activation.h"
#include "Constants.h"

enum CardIDEnum
{
    CARD_BULLET    = 1,
    CARD_LASER     = 2,
    CARD_HEAL      = 3,
    CARD_INVIS     = 4,
    CARD_SHOCKWAVE = 5,
    CARD_BOMB      = 6,
    CARD_SPEARS    = 7,
    CARD_BUBBLE    = 8,
    CARD_SWIPE     = 9,
    CARD_AREAGRAB  = 10
};

class Card
{
public:

    Card(){};
    virtual ~Card(){};


    virtual Texture* GetTexture()  = 0;
    virtual const char* GetName()        = 0;
    virtual const char* GetDescription() = 0;
    virtual int GetID()            = 0;
    virtual int GetManaCost()      = 0;
    virtual int GetRarity()        = 0;
    virtual void Cast(void* pGame, int nCaster) = 0;

protected:

    int SpawnActivation(void*       pGame,
                        int         nCaster,
                        Activation* pActivation);
};

#endif