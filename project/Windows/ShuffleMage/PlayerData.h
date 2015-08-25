#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <string.h>

#define PLAYER_NAME_SIZE 15
#define DECK_SIZE 30
#define COLLECTION_SIZE 512

class PlayerData
{
public:
    char arName[PLAYER_NAME_SIZE + 1];
    int  nPlayerID;
    int  nGold;
    int  arCollection[COLLECTION_SIZE];
    int  arDeck1[DECK_SIZE];
    int  arDeck2[DECK_SIZE];
    int  arDeck3[DECK_SIZE];
    int  nLastLogon;

    void Clear()
    {
        memset(arName, 0, PLAYER_NAME_SIZE + 1);
        nPlayerID = 0;
        nGold     = 0;
        memset(arCollection, 0, COLLECTION_SIZE * sizeof(int));
        memset(arDeck1, 0, DECK_SIZE * sizeof(int));
        memset(arDeck2, 0, DECK_SIZE * sizeof(int));
        memset(arDeck3, 0, DECK_SIZE * sizeof(int));
        nLastLogon = 0;

    }

    PlayerData()
    {
        Clear();
    }
};


#endif