#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "PlayerData.h"
#include "VSocket.h"
#include "Timer.h"
#include "Message.h"

#define RECV_BUFFER_SIZE 0xffff
#define SEND_BUFFER_SIZE 0x1000

class NetworkManager
{

public:

    NetworkManager();

    ~NetworkManager();

    void Connect();

    void Disconnect();

    void Update();

    char* ProcessMessage(char* pBuffere, int nLimit);

    void Send(Message& msg);

    // Message handling functions
    void ResLogin();
    void ResQueue();
    void ResRegister();

    void* m_pMenu;
    void* m_pGame;

    PlayerData m_player;

private:

    Socket* m_pSocket;

    Timer m_timKeepAlive;

    static char s_arRecvBuffer[RECV_BUFFER_SIZE];
    static char s_arSendBuffer[SEND_BUFFER_SIZE];

};

#endif
