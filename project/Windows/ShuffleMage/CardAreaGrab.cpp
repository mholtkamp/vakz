#include "CardAreaGrab.h"
#include "ActAreaGrab.h"

#define CARD_AREAGRAB_TEXTURE g_pCardAreaGrabTex
#define CARD_AREAGRAB_NAME "AREA GRAB"
#define CARD_AREAGRAB_DESCRIPTION "Steals nearest row of enemy tiles"
#define CARD_AREAGRAB_MANA 50
#define CARD_AREAGRAB_RARITY RARITY_COMMON


CardAreaGrab::CardAreaGrab()
{

}

CardAreaGrab::~CardAreaGrab()
{

}

Texture* CardAreaGrab::GetTexture()
{
#if defined(SM_CLIENT)
    return CARD_AREAGRAB_TEXTURE;
#else
    return 0;
#endif
}

const char* CardAreaGrab::GetName()
{
    return CARD_AREAGRAB_NAME;
}

const char* CardAreaGrab::GetDescription()
{
    return CARD_AREAGRAB_DESCRIPTION;
}

int CardAreaGrab::GetID()
{
    return CARD_AREAGRAB;
}

int CardAreaGrab::GetManaCost()
{
    return CARD_AREAGRAB_MANA;
}

int CardAreaGrab::GetRarity()
{
    return CARD_AREAGRAB_RARITY;
}

void CardAreaGrab::Cast(void* pGame, int nCaster)
{
    Activation* pAct = new ActAreaGrab();
    if (SpawnActivation(pGame, nCaster, pAct) == -1)
    {
        // Couldn't create activation.
        delete pAct;
    }
}
