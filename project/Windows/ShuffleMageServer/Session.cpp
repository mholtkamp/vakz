#include "Session.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>

#include "Message.h"
#include "Constants.h"

#define DATABASE_PATH "C:/Users/mholt_000/ShuffleMage/"
char Session::s_arMsgBuffer[MSG_BUFFER_SIZE] = {0};
void* Session::s_pMatchQueue = 0;

MsgLogin          s_msgLogin;
MsgRegister       s_msgRegister;

MsgResLogin       s_msgResLogin;
MsgResRegister    s_msgResRegister;

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

    nMsgID = reinterpret_cast<int*>(pBuffer)[0];

    if (nMsgID < 100)
    {
        switch (nMsgID)
        {
        case MSG_KEEPALIVE:
            LogDebug("KeepAlive received.");
            pBuffer += MSG_KEEPALIVE_SIZE + HEADER_SIZE;
            break;
        case MSG_LOGIN:
            s_msgLogin.Read(pBuffer);
            Login();
            pBuffer += s_msgLogin.Size() + HEADER_SIZE;
            break;
        case MSG_REGISTER:
        {
            s_msgRegister.Read(pBuffer);
            Register();
            pBuffer += s_msgRegister.Size() + HEADER_SIZE;
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


void Session::Activate(Socket* pSocket)
{
    m_pSocket = pSocket;
    m_nActive = 1;
}

void Session::Send(Message& msg)
{
    msg.Write(s_arMsgBuffer);
    if (m_pSocket != 0)
    {
        m_pSocket->Send(s_arMsgBuffer, msg.Size() + HEADER_SIZE);
    }
}

PlayerData* Session::GetPlayerData()
{
    return &m_playerdata;
}

void Session::SetMatchQueue(void* pMatchQueue)
{

}

void Session::Login()
{
    FILE* pFile = 0;
    char arPath[128] = {0};
    memcpy(arPath, DATABASE_PATH, strlen(DATABASE_PATH));
    memcpy(arPath + strlen(DATABASE_PATH), s_msgLogin.m_arUser, USER_BUFFER_SIZE);
    pFile = fopen(arPath, "rb");

    if (pFile == 0)
    {
        // File not found, report a user not found error.
        LogDebug("User not found.");
        s_msgResLogin.Clear();
        s_msgResLogin.m_nSuccess = LOGIN_STATUS_USER_NOT_FOUND;
        Send(s_msgResLogin);
    }
    else
    {
        fread(&m_playerdata, sizeof(m_playerdata), 1, pFile);
        fclose(pFile);
        if (strcmp(s_msgLogin.m_arPass, m_playerdata.m_arPass) == 0)
        {
            // Passwords match, login successful!
            LogDebug("User has successfully logged in!");
            s_msgResLogin.m_nSuccess     = LOGIN_STATUS_OK;
            s_msgResLogin.m_nPlayerID    = m_playerdata.m_nPlayerID;
            s_msgResLogin.m_nGold        = m_playerdata.m_nGold;
            memcpy(s_msgResLogin.m_arCollection, m_playerdata.m_arCollection, sizeof(m_playerdata.m_arCollection));
            memcpy(s_msgResLogin.m_arDeck1, m_playerdata.m_arDeck1, sizeof(m_playerdata.m_arDeck1));
            memcpy(s_msgResLogin.m_arDeck2, m_playerdata.m_arDeck2, sizeof(m_playerdata.m_arDeck2));
            memcpy(s_msgResLogin.m_arDeck3, m_playerdata.m_arDeck3, sizeof(m_playerdata.m_arDeck3));
            Send(s_msgResLogin);
        }
        else
        {
            // Passwords do not match. Reject login attempt.
            LogDebug("User attempted to log in with invalid password.");
            s_msgResLogin.Clear();
            s_msgResLogin.m_nSuccess = LOGIN_STATUS_INVALID_PASS;
            Send(s_msgResLogin);
        }
    }
}

void Session::Register()
{
    FILE* pFile = 0;
    char arPath[128] = {0};
    memcpy(arPath, DATABASE_PATH, strlen(DATABASE_PATH));
    memcpy(arPath + strlen(DATABASE_PATH), s_msgRegister.m_arUser, USER_BUFFER_SIZE);
    pFile = fopen(arPath, "rb");

    if (pFile == 0)
    {
        // Okay, register the user!
        pFile = fopen(arPath, "wb");

        if (pFile == 0)
        {
            LogError("Error creating new user file.");
            s_msgResRegister.Clear();
            s_msgResRegister.m_nSuccess = LOGIN_STATUS_REGISTRATION_FAILED;
            Send(s_msgResRegister);
        }
        else
        {
            // Generate a new player PlayerData struct
            m_playerdata.GenerateNewPlayer();
            memcpy(m_playerdata.m_arUser, s_msgRegister.m_arUser, USER_BUFFER_SIZE);
            memcpy(m_playerdata.m_arPass, s_msgRegister.m_arPass, PASS_BUFFER_SIZE);

            // Write the player data to file
            fwrite(&m_playerdata, sizeof(m_playerdata), 1, pFile);
            fclose(pFile);

            // Generate the response message
            s_msgResRegister.m_nSuccess = LOGIN_STATUS_OK;
            s_msgResRegister.m_nGold    = m_playerdata.m_nGold;
            memcpy(s_msgResRegister.m_arCollection, m_playerdata.m_arCollection, sizeof(m_playerdata.m_arCollection));
            memcpy(s_msgResRegister.m_arDeck1, m_playerdata.m_arDeck1, sizeof(m_playerdata.m_arDeck1));
            memcpy(s_msgResRegister.m_arDeck2, m_playerdata.m_arDeck2, sizeof(m_playerdata.m_arDeck2));
            memcpy(s_msgResRegister.m_arDeck3, m_playerdata.m_arDeck3, sizeof(m_playerdata.m_arDeck3));
            Send(s_msgResRegister);
        }
    }
    else
    {
        fclose(pFile);
        // Return an error message. Make sure it has the proper status message.
        s_msgResRegister.Clear();
        s_msgResRegister.m_nSuccess = LOGIN_STATUS_REGISTRATION_FAILED;
        Send(s_msgResRegister);
    }
}
