#ifndef SESSION_H
#define SESSION_H

#define MSG_BUFFER_SIZE 0xffff
#define RECV_BUFFER_SIZE 0xffff
#define SEND_BUFFER_SIZE 0x1000

#include "PlayerData.h"
#include "VSocket.h"
#include "Message.h"

class Session
{

public:

    Session();

    ~Session();

    int IsActive();

    void Update();

    void Activate(Socket* pSocket);

    void Send(Message& msg);

    PlayerData* GetPlayerData();

    static void SetMatchQueue(void* pMatchQueue);

    // Commands
    void Login();
    void Queue();
    void Register();

    void* m_pGame;

private:

    char* ProcessMessage(char* pBuffer,
                         int   nLimit);

    int m_nActive;
    int m_nLoggedIn;
    int m_nInGame;

    PlayerData m_playerdata;

    Socket* m_pSocket;

    static char s_arRecvBuffer[RECV_BUFFER_SIZE];
    static char s_arSendBuffer[SEND_BUFFER_SIZE];

    static void* s_pMatchQueue;
};

#endif
