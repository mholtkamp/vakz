#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <string.h>
#include <stdio.h>

#define PLAYER_USER_SIZE 15
#define PLAYER_PASS_SIZE 15
#define DECK_SIZE 30
#define COLLECTION_SIZE 512

static int s_arStartingCollection[DECK_SIZE] = {1,1,1, 2,2,2, 3,3,3,
                                                4,4,4, 5,5,5, 6,6,6,
                                                7,7,7, 8,8,8, 9,9,9,
                                                10, 10, 10};

class PlayerData
{
public:
    char m_arUser[PLAYER_USER_SIZE + 1];
    char m_arPass[PLAYER_PASS_SIZE + 1];
    int  m_nPlayerID;
    int  m_nGold;
    int  m_arCollection[COLLECTION_SIZE];
    int  m_arDeck1[DECK_SIZE];
    int  m_arDeck2[DECK_SIZE];
    int  m_arDeck3[DECK_SIZE];
    int  m_nLastLogon;

    void Clear()
    {
        memset(m_arUser, 0, PLAYER_USER_SIZE + 1);
        m_nPlayerID = 0;
        m_nGold     = 0;
        memset(m_arCollection, 0, COLLECTION_SIZE * sizeof(int));
        memset(m_arDeck1, 0, DECK_SIZE * sizeof(int));
        memset(m_arDeck2, 0, DECK_SIZE * sizeof(int));
        memset(m_arDeck3, 0, DECK_SIZE * sizeof(int));
        m_nLastLogon = 0;

    }

    PlayerData()
    {
        Clear();
    }

    void PlayerData::GenerateNewPlayer()
    {
        m_nGold = 100;
        memcpy(m_arCollection, s_arStartingCollection, sizeof(s_arStartingCollection));
        memcpy(m_arDeck1, s_arStartingCollection, sizeof(s_arStartingCollection));
        memcpy(m_arDeck2, s_arStartingCollection, sizeof(s_arStartingCollection));
        memcpy(m_arDeck3, s_arStartingCollection, sizeof(s_arStartingCollection));
    }

    void PlayerData::PrintData()
    {
#if defined WINDOWS
        printf("User: %s\n", m_arUser);
        printf("Pass: %s\n", m_arPass);
        printf("Gold: %d\n", m_nGold);

        printf ("Collection: ");
        for (int i = 0; i < COLLECTION_SIZE; i++)
        {
            printf("%d, ", m_arCollection[i]);
        }
        printf("\n");

        printf ("Deck1: ");
        for (int i = 0; i < DECK_SIZE; i++)
        {
            printf("%d, ", m_arDeck1[i]);
        }
        printf("\n");

        printf ("Deck2: ");
        for (int i = 0; i < DECK_SIZE; i++)
        {
            printf("%d, ", m_arDeck2[i]);
        }
        printf("\n");

        printf ("Deck3: ");
        for (int i = 0; i < DECK_SIZE; i++)
        {
            printf("%d, ", m_arDeck3[i]);
        }
        printf("\n");
#elif defined

#endif
    }
};


#endif