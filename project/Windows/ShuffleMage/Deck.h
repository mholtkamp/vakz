#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include "Constants.h"

class Deck
{
public:

    Deck();
    ~Deck();
    
    void Draw(int nCount);

    void Shuffle();

    int Remaining();

private:

    Card* m_arCards[DECK_SIZE];
};

#endif