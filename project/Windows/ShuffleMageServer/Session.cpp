#include "Session.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>

#include "Message.h"

#define DATABASE_PATH "C:/ShuffleMage/"
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
                pBuffer = ProcessMessage(pBuffer, nSize);
            }
        }
    }
}

char* Session::ProcessMessage(char* pBuffer,
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
            case MSG_REGISTER:
            {
                m_msgRegister.Read(pBuffer);
                Register();
                pBuffer += m_msgRegister.Size() + HEADER_SIZE;
                break;
            }
            default:
                break;
            }

            return pBuffer;
        }
        else
        {
            if(m_pGame != 0)
            {
                // Send to game to process message.
            }
            return pBuffer;
        }
    }
}


void Session::Activate(Socket* pSocket)
{
    m_pSocket = pSocket;
    m_nActive = 1;
}

void Session::Register()
{
    FILE* pFile = 0;
    char arPath[128] = {0};
    memcpy(arPath, DATABASE_PATH, strlen(DATABASE_PATH));
    memcpy(arPath + strlen(DATABASE_PATH), m_msgRegister.m_arUser, USER_BUFFER_SIZE);
    pFile = fopen(arPath, "rb");

    if (pFile == 0)
    {
        // Okay, register the user!
    }
    else
    {
        // Return an error message. Make sure it has the proper status message.
    }
}