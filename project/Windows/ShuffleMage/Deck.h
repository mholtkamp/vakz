#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include "Constants.h"

class Deck
{
public:

    Deck();
    ~Deck();
    
    int Draw();

    void Shuffle();

    int Remaining();

    void Set(int* arCardIDs);

private:

    int m_nRemaining;
    int m_arCards[DECK_SIZE];
};

#endif