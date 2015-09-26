#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include "PlayerData.h"
#include "Message.h"
#include "Mage.h"
#include "Tile.h"
#include "Deck.h"

enum ServerGameEnum
{
    SESSION_1     = 0,
    SESSION_2     = 1,
    SESSION_COUNT = 2,
    SESSION_ALL   = SESSION_COUNT
};

class ServerGame
{

public:

    ServerGame();
    ~ServerGame();

    int IsActive();

    void Update();

    int GetGameState();

    void SetSessions(void* pSession1,
                     void* pSession2);

    void Send(Message& msg,
              void* pSession);

    void Send(Message& msg,
              int nSession);

    char* ProcessMessage(char* pBuffer,
                         int   nLimit);

    void RefreshHand(int nSession);

    Mage* GetMage(int nIndex);

    // Message Actions
    void UpdatePosition(int nPlayer,
                        int nX,
                        int nZ);

    void UpdateHealth(int nPlayer);

    void UseCard(int nCard,
                 int nCaster);

    void UpdateStatus(int nPlayer,
                      int nStatus,
                      int nAfflicted);

    enum ServerGameEnum
    {
        MAGE_1    = 0,
        MAGE_2    = 1,
        NUM_MAGES = 2
    };

    Tile m_arTiles[GRID_WIDTH][GRID_HEIGHT];

    // Activations
    Activation* m_arActivations[MAX_ACTIVATIONS];

private:

    Mage m_arMages[NUM_MAGES];

    Deck m_arDecks[NUM_MAGES];

    int m_arHands[NUM_MAGES][HAND_SIZE];

    int m_nGameState;

    void* m_arSessions[SESSION_COUNT];

    PlayerData* m_arPlayerData[SESSION_COUNT];
};


#endif
