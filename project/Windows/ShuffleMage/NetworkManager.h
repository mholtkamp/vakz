#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "PlayerData.h"
#include "VSocket.h"
#include "Timer.h"

#define MSG_BUFFER_SIZE 0xffff

class NetworkManager
{

public:

    NetworkManager();

    ~NetworkManager();

    void Connect();

    void Disconnect();

    void Update();

    void ProcResRegister();

    void* m_pMenu;
    void* m_pGame;

    PlayerData m_player;

private:

    Socket* m_pSocket;

    Timer m_timKeepAlive;

    static char s_arMsgBuffer[MSG_BUFFER_SIZE];

};

#endif
