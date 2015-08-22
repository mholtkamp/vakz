#ifndef SESSION_H
#define SESSION_H

#define MSG_BUFFER_SIZE 0xffff

#include "PlayerData.h"
#include "VSocket.h"

class Session
{

public:

    Session();

    ~Session();

    int IsActive();

    void Update();

private:

    void ProcessMessage(char* pBuffer);

    int m_nActive;
    int m_nLoggedIn;
    int m_nInGame;

    PlayerData m_playerdata;

    Socket* m_pSocket;

    void* m_pGame;

    static char s_arMsgBuffer[MSG_BUFFER_SIZE];
};

#endif
