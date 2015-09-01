#include "NetworkManager.h"
#include "Message.h"
#include "Log.h"
#include "Menu.h"
#include "Game.h"
#include "Constants.h"

// Session Response messages
#include "MsgResLogin.h"
#include "MsgResQueue.h"
#include "MsgResRegister.h"

// Game Response Messages
#include "MsgPosition.h"


#define MASTER_SEREVER_IP "192.168.2.3"
#define SERVER_PORT 2000
#define KEEP_ALIVE_PERIOD 20.0f

char NetworkManager::s_arMsgBuffer[MSG_BUFFER_SIZE] = {0};

static MsgResLogin        s_msgResLogin;
static MsgResQueue        s_msgResQueue;
static MsgResRegister     s_msgResRegister;

static MsgPosition        s_msgPosition;

// Declaration of functions in Main.cpp
void SetGameState(int nState);
void SetGame(void* pNewGame);

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
    int   i        = 0;
    int   nSize    = 0;
    char* pBuffer  = 0;
    
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

        // Read any messages from the server
        pBuffer = s_arMsgBuffer;

        nSize = m_pSocket->Receive(s_arMsgBuffer, sizeof(s_arMsgBuffer));

        // There are messages to read!
        if (nSize > 0)
        {
            while (pBuffer < nSize + s_arMsgBuffer)
            {
                pBuffer = ProcessMessage(pBuffer, nSize);
            }
        }
    }
}

char* NetworkManager::ProcessMessage(char* pBuffer,
                                     int   nLimit)
{
    int nMsgID = 0;

    nMsgID = reinterpret_cast<int*>(pBuffer)[0];

    if (nMsgID > 0 &&
        nMsgID < 100)
    {
        if (m_pMenu == 0)
        {
            LogError("Network manager has no menu!");
            return 0;
        }

        switch (nMsgID)
        {
        case MSG_RES_LOGIN:
            s_msgResLogin.Read(pBuffer);
            ResLogin();
            pBuffer += s_msgResLogin.Size() + HEADER_SIZE;
            break;
        case MSG_RES_QUEUE:
            s_msgResQueue.Read(pBuffer);
            ResQueue();
            pBuffer += s_msgResQueue.Size() + HEADER_SIZE;
            break;
        case MSG_RES_REGISTER:
            s_msgResRegister.Read(pBuffer);
            ResRegister();
            pBuffer += s_msgResRegister.Size() + HEADER_SIZE;
            break;
        default:
            LogError("Unknown Session message received.");
            break;
        }

        return pBuffer;
    }
    else if (nMsgID > 100 &&
             nMsgID < 200)
    {
        if(m_pGame != 0)
        {
            switch (nMsgID)
            {
            case MSG_POSITION:
                s_msgPosition.Read(pBuffer);
                reinterpret_cast<Game*>(m_pGame)->UpdatePosition(s_msgPosition.m_nPlayer,
                                                                 s_msgPosition.m_nX,
                                                                 s_msgPosition.m_nZ);
                pBuffer += s_msgPosition.Size() + HEADER_SIZE;
                break;
            default:
                LogError("Unknown Game message received.");
                pBuffer = 0;
                break;
            }
        }
        return pBuffer;
    }
    else
    {
        LogError("Unkown message received.");
        pBuffer = 0;
        return pBuffer;
    }
}

void NetworkManager::ResLogin()
{
    reinterpret_cast<Menu*>(m_pMenu)->SetLoginStatus(s_msgResLogin.m_nSuccess);
    m_player.m_nPlayerID = s_msgResLogin.m_nPlayerID;
    m_player.m_nGold     = s_msgResLogin.m_nGold;
    memcpy(m_player.m_arCollection, s_msgResLogin.m_arCollection, COLLECTION_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck1, s_msgResLogin.m_arDeck1, DECK_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck2, s_msgResLogin.m_arDeck2, DECK_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck3, s_msgResLogin.m_arDeck3, DECK_SIZE * sizeof(int));

    // Set the player data in the menu, the menu will add the user/pass to the playerdata
    reinterpret_cast<Menu*>(m_pMenu)->SetPlayerData(&m_player);
}

void NetworkManager::ResQueue()
{
    if (s_msgResQueue.m_nSuccess == QUEUE_STATUS_ERROR)
    {
        LogError("Error joining queue.");
    }
    else if (s_msgResQueue.m_nSuccess == QUEUE_STATUS_FULL)
    {
        LogError("Could not join queue because it is full.");
    }
    else if (s_msgResQueue.m_nSuccess == QUEUE_STATUS_MATCH_FOUND)
    {
        LogDebug("Match has been found!");
        if (m_pGame == 0)
        {
            m_pGame = new Game(s_msgResQueue.m_nSide);
            reinterpret_cast<Game*>(m_pGame)->m_pNetworkManager = this;
            reinterpret_cast<Game*>(m_pGame)->RegisterScene();
            SetGameState(GAME_STATE_GAME);
            SetGame(m_pGame);
        }
        
    }
    else
    {
        LogDebug("Unknown queue status received in NetworkManager::ResQueue().");
    }
}

void NetworkManager::ResRegister()
{
    reinterpret_cast<Menu*>(m_pMenu)->SetLoginStatus(s_msgResRegister.m_nSuccess);
    m_player.m_nPlayerID = s_msgResRegister.m_nPlayerID;
    m_player.m_nGold     = s_msgResRegister.m_nGold;
    memcpy(m_player.m_arCollection, s_msgResRegister.m_arCollection, COLLECTION_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck1, s_msgResRegister.m_arDeck1, DECK_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck2, s_msgResRegister.m_arDeck2, DECK_SIZE * sizeof(int));
    memcpy(m_player.m_arDeck3, s_msgResRegister.m_arDeck3, DECK_SIZE * sizeof(int));

    // Set the player data in the menu, the menu will add the user/pass to the playerdata
    reinterpret_cast<Menu*>(m_pMenu)->SetPlayerData(&m_player);
}

void NetworkManager::Send(Message& msg)
{
    msg.Write(s_arMsgBuffer);
    if (m_pSocket != 0)
    {
        m_pSocket->Send(s_arMsgBuffer, msg.Size() + HEADER_SIZE);
    }
}