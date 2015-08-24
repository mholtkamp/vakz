#include "Session.h"
#include "Log.h"

#include "Message.h"

char Session::s_arMsgBuffer[MSG_BUFFER_SIZE] = {0};

Session::Session()
{
    m_nActive   = 0;
    m_nLoggedIn = 0;
    m_nInGame   = 0;

    m_pSocket = 0;
    m_pGame   = 0;
}

Session::~Session()
{
    delete m_pSocket;
}

int Session::IsActive()
{
    return m_nActive;
}

void Session::Update()
{
    int   nSize = 0;
    char* pBuffer = 0;

    if (m_nActive != 0 &&
        m_pSocket != 0)
    {
        
        nSize = m_pSocket->Receive(s_arMsgBuffer, MSG_BUFFER_SIZE);
        if (nSize > 0)
        {
            // Set pointer to beginning of the static buffer.
            // The pointer location will be updated after each message
            // has been processed.
            pBuffer = s_arMsgBuffer;

            while (pBuffer < nSize + s_arMsgBuffer)
            {
                ProcessMessage(pBuffer, nSize);
            }
        }
    }
}

void Session::ProcessMessage(char* pBuffer,
                             int   nLimit)
{
    int nMsgID = 0;

    if (pBuffer != 0 &&
        pBuffer < nLimit + s_arMsgBuffer)
    {
        nMsgID = reinterpret_cast<int*>(pBuffer)[0];

        if (nMsgID < 100)
        {
            switch (nMsgID)
            {
            case MSG_LOGIN:
                //ProcLogin(pBuffer);
                break;
            default:
                break;
            }
        }
        else
        {
            if(m_pGame != 0)
            {
                // Send to game to process message.
            }
        }
    }
}
