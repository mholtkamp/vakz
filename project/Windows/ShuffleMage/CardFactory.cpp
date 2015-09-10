#include "CardFactory.h"

Card* InstantiateCard(int nCardID)
{
    switch(nCardID)
    {
    case CARD_AREAGRAB:
        return new CardAreaGrab();
        break;
    case CARD_BOMB:
        return new CardBomb();
        break;
    case CARD_BUBBLE:
        return new CardBubble();
        break;
    case CARD_BULLET:
        return new CardBullet();
        break;
    case CARD_HEAL:
        return new CardHeal();
        break;
    case CARD_INVIS:
        return new CardInvis();
        break;
    case CARD_LASER:
        return new CardLaser();
        break;
    case CARD_SHOCKWAVE:
        return new CardShockwave();
        break;
    case CARD_SPEARS:
        return new CardSpears();
        break;
    case CARD_SWIPE:
        return new CardSwipe();
        break;
    default:
        LogError("Unrecognized CardID for Instantiation.");
        return 0;
        break;
    }
}