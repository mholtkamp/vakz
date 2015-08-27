#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include "PlayerData.h"
#include "Message.h"

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

    void Update();

    int GetGameState();

    void SetSessions(void* pSession1,
                     void* pSession2);

    void Send(Message& msg,
              void* pSession);

    void Send(Message& msg,
              int nSession);

private:

    int m_nGameState;

    void* m_arSessions[SESSION_COUNT];

    PlayerData* m_arPlayerData[SESSION_COUNT];
};


#endif
