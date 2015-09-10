#include "Deck.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

Deck::Deck()
{
    m_nRemaining = 0;
    memset(m_arCards, 0, DECK_SIZE * sizeof(int));
}

Deck::~Deck()
{

}

void Deck::Set(int* arCardIDs)
{
    m_nRemaining = DECK_SIZE;
    memcpy(m_arCards, arCardIDs, DECK_SIZE * sizeof(int));
}

void Deck::Shuffle()
{
    int i     = 0;
    int nTemp = 0;
    int nPos  = 0;

    srand(static_cast<unsigned int>(time(0)));

    for (i = 0; i < DECK_SIZE; i++)
    {
        nPos = DECK_SIZE * (rand() % 30);
        nTemp = m_arCards[i];
        m_arCards[i] = m_arCards[nPos];
        m_arCards[nPos] = nTemp;
    }
}

int Deck::Draw()
{
    // If deck is out, return invalid card
    if (m_nRemaining == 0)
    {
        return -1;
    }
    else
    {
        m_nRemaining--;
        return m_arCards[m_nRemaining];
    }

}

int Deck::Remaining()
{
    return m_nRemaining;
}