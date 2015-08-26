#include "ServerGame.h"
#include "Constants.h"
#include "Log.h"
#include "Session.h"

ServerGame::ServerGame()
{
    m_nGameState = GAME_STATE_INACTIVE;

    m_arSessions[SESSION_1] = 0;
    m_arSessions[SESSION_2] = 0;

    m_arPlayerData[SESSION_1] = 0;
    m_arPlayerData[SESSION_2] = 0;
}

ServerGame::~ServerGame()
{

}

void ServerGame::Update()
{

}

void ServerGame::SetSessions(void* pSession1,
                             void* pSession2)
{
    if (pSession1 != 0 &&
        pSession2 != 0)
    {
        m_arSessions[SESSION_1] = pSession1;
        m_arSessions[SESSION_2] = pSession2;

        m_arPlayerData[SESSION_1] = reinterpret_cast<Session*>(m_arSessions[SESSION_1])->GetPlayerData();
        m_arPlayerData[SESSION_2] = reinterpret_cast<Session*>(m_arSessions[SESSION_2])->GetPlayerData();
    }
    else
    {
        LogError("Null session received in ServerGame::SetSessions().");
    }
}

void ServerGame::Send(Message& msg, void* pSession)
{
    if (pSession != 0)
    {
        reinterpret_cast<Session*>(pSession)->Send(msg);
    }
}

void ServerGame::Send(Message& msg, int nSession)
{
    Session** pSessions = reinterpret_cast<Session**>(m_arSessions);

    if (nSession == SESSION_ALL      &&
        m_arSessions[SESSION_1] != 0 &&
        m_arSessions[SESSION_2] != 0)
    {
        pSessions[SESSION_1]->Send(msg);
        pSessions[SESSION_2]->Send(msg);
    }
    else if (nSession == SESSION_1  &&
            m_arSessions[SESSION_1] != 0)
    {
        pSessions[SESSION_1]->Send(msg);
    }
    else if (nSession == SESSION_2   &&
            m_arSessions[SESSION_2] != 0)
    {
        pSessions[SESSION_2]->Send(msg);
    }
    else
    {
        LogError("Invalid session index to send message to in ServerGame::Send().");
    }
}