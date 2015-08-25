#include "NetworkManager.h"
#include "Message.h"

#define MASTER_SEREVER_IP "192.168.2.3"
#define SERVER_PORT 2000
#define KEEP_ALIVE_PERIOD 20.0f

char NetworkManager::s_arMsgBuffer[MSG_BUFFER_SIZE] = {0};

NetworkManager::NetworkManager()
{
    m_pMenu = 0;
    m_pGame = 0;
    
    m_pSocket = 0;
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Connect()
{
    if (m_pSocket == 0)
    {
        m_pSocket = new Socket();
        m_pSocket->Connect(Socket::TCP, MASTER_SEREVER_IP, SERVER_PORT);

        // Start keepalive timer
        m_timKeepAlive.Start();
    }
}

void NetworkManager::Disconnect()
{
    if (m_pSocket != 0)
    {
        m_pSocket->Close();
        delete m_pSocket;
        m_pSocket = 0;
    }
}

void NetworkManager::Update()
{
    if (m_pSocket != 0)
    {
        m_timKeepAlive.Stop();

        if ( m_timKeepAlive.Time() > KEEP_ALIVE_PERIOD)
        {
            // Send keepalive message
            reinterpret_cast<int*>(s_arMsgBuffer)[0] = MSG_KEEPALIVE;
            m_pSocket->Send(s_arMsgBuffer, HEADER_SIZE);

            m_timKeepAlive.Start();
        }
    }
}

void NetworkManager::ProcResRegister()
{

}

void NetworkManager::Send(Message& msg)
{
    msg.Write(s_arMsgBuffer);
    if (m_pSocket != 0)
    {
        m_pSocket->Send(s_arMsgBuffer, msg.Size());
    }
}