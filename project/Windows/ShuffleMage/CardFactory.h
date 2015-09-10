#ifndef CARD_FACTORY_H
#define CARD_FACTORY_H

#include "Card.h"

#include "CardAreaGrab.h"
#include "CardBomb.h"
#include "CardBubble.h"
#include "CardBullet.h"
#include "CardHeal.h"
#include "CardInvis.h"
#include "CardLaser.h"
#include "CardShockwave.h"
#include "CardSpears.h"
#include "CardSwipe.h"

Card* InstantiateCard(int nCardID);

#endif