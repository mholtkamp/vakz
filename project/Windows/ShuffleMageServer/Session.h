#ifndef SESSION_H
#define SESSION_H

#define MSG_BUFFER_SIZE 0xffff

#include "PlayerData.h"
#include "VSocket.h"

// Message Includes
#include "Message.h"
#include "MsgLogin.h"
#include "MsgRegister.h"

class Session
{

public:

    Session();

    ~Session();

    int IsActive();

    void Update();

    void Activate(Socket* pSocket);

    // Commands
    void Register();

private:

    char* ProcessMessage(char* pBuffer,
                         int   nLimit);

    int m_nActive;
    int m_nLoggedIn;
    int m_nInGame;

    PlayerData m_playerdata;

    Socket* m_pSocket;

    void* m_pGame;

    static char s_arMsgBuffer[MSG_BUFFER_SIZE];


    // Message
    MsgLogin    m_msgLogin;
    MsgRegister m_msgRegister;
};

#endif
