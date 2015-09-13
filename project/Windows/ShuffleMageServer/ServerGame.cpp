#include "ServerGame.h"
#include "Constants.h"
#include "Log.h"
#include "Session.h"
#include "CardFactory.h"
#include <stdlib.h>
#include <time.h>

// Response Messages
#include "MsgResQueue.h"

// Game Messages
#include "MsgPosition.h"
#include "MsgCard.h"
#include "MsgDraw.h"

static MsgResQueue      s_msgResQueue;
static MsgPosition      s_msgPosition;
static MsgCard          s_msgCard;
static MsgDraw          s_msgDraw;

ServerGame::ServerGame()
{
    int i = 0;
    int j = 0;

    m_nGameState = SERVER_GAME_STATE_INACTIVE;

    m_arSessions[SESSION_1] = 0;
    m_arSessions[SESSION_2] = 0;

    m_arPlayerData[SESSION_1] = 0;
    m_arPlayerData[SESSION_2] = 0;

    m_arMages[SESSION_1].SetSide(SIDE_1);
    m_arMages[SESSION_2].SetSide(SIDE_2);

    for (i = 0; i < GRID_HEIGHT; i++)
    {
        for (j = 0; j < GRID_WIDTH; j++)
        {
            if (j < GRID_WIDTH / 2)
            {
                m_arTiles[j][i].SetOwner(SIDE_1);
            }
            else
            {
                m_arTiles[j][i].SetOwner(SIDE_2);
            }
        }
    }

    // Set Game/Tiles for mages
    m_arMages[MAGE_1].SetGame(this);
    m_arMages[MAGE_2].SetGame(this);

    // Clear hands
    memset(m_arHands, 0, NUM_MAGES * HAND_SIZE * sizeof(int));

    // Clear activations
    memset(m_arActivations, 0, MAX_ACTIVATIONS * sizeof(Activation*));
 
}

ServerGame::~ServerGame()
{

}

int ServerGame::IsActive()
{
    if (m_nGameState == SERVER_GAME_STATE_ACTIVE  ||
        m_nGameState == SERVER_GAME_STATE_OVER    ||
        m_nGameState == SERVER_GAME_STATE_WAITING)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ServerGame::Update()
{
    int i = 0;

    // Update all activations
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (m_arActivations[i] != 0)
        {
            m_arActivations[i]->Update();

            if (m_arActivations[i]->m_nExpired != 0)
            {
                m_arActivations[i]->OnDestroy();
                delete m_arActivations[i];
                m_arActivations[i] = 0;
            }
        }
    }
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

        // Set the state to
        m_nGameState = SERVER_GAME_STATE_WAITING;

        // Send message to first player
        s_msgResQueue.m_nSuccess = QUEUE_STATUS_MATCH_FOUND;
        s_msgResQueue.m_nSide    = SIDE_1;
        Send(s_msgResQueue, SESSION_1);

        // Send message to second player
        s_msgResQueue.m_nSuccess = QUEUE_STATUS_MATCH_FOUND;
        s_msgResQueue.m_nSide = SIDE_2;
        Send(s_msgResQueue, SESSION_2);

        // Setup decks
        m_arDecks[SESSION_1].Set(m_arPlayerData[SESSION_1]->m_arDeck1);
        m_arDecks[SESSION_2].Set(m_arPlayerData[SESSION_2]->m_arDeck1);

        // Shuffle decks
        srand(static_cast<unsigned int>(time(0)));
        m_arDecks[SESSION_1].Shuffle();
        m_arDecks[SESSION_2].Shuffle();

        RefreshHand(SESSION_1);
        RefreshHand(SESSION_2);
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

int ServerGame::GetGameState()
{
    return m_nGameState;
}

void ServerGame::RefreshHand(int nSession)
{
    int i      = 0;
    int nCount = 0;

    s_msgDraw.Clear();

    for (i = 0; i < HAND_SIZE; i++)
    {
        if (m_arHands[nSession][i] == 0)
        {
            m_arHands[nSession][i] = m_arDecks[nSession].Draw();

            if (m_arHands[nSession][i] == -1)
            {
                m_arHands[nSession][i] = 0;
                break;
            }

            s_msgDraw.m_arCards[nCount] = m_arHands[nSession][i];
            nCount++;
        }
    }

    Send(s_msgDraw, nSession);
}

char* ServerGame::ProcessMessage(char* pBuffer,
                                 int   nLimit)
{
    int nMsgID = 0;

    nMsgID = reinterpret_cast<int*>(pBuffer)[0];

    switch(nMsgID)
    {
    case MSG_POSITION:
        s_msgPosition.Read(pBuffer);
        UpdatePosition(s_msgPosition.m_nPlayer,
                       s_msgPosition.m_nX,
                       s_msgPosition.m_nZ);
        pBuffer += s_msgPosition.Size() + HEADER_SIZE;
        break;
    case MSG_CARD:
        s_msgCard.Read(pBuffer);
        UseCard(s_msgCard.m_nCard,
                s_msgCard.m_nCaster);
        pBuffer += s_msgCard.Size() + HEADER_SIZE;
        break;
    default:
        pBuffer = 0;
        break;
    }

    return pBuffer;
}

void ServerGame::UpdatePosition(int nPlayer,
                                int nX,
                                int nZ)
{
    if (nPlayer == SESSION_1)
    {
        m_arMages[SESSION_1].UpdatePosition(nX, nZ);

        s_msgPosition.m_nPlayer = MAGE_1;
        s_msgPosition.m_nX      = nX;
        s_msgPosition.m_nZ      = nZ;

        Send(s_msgPosition, SESSION_2);
    }
    else if (nPlayer == SESSION_2)
    {
        m_arMages[SESSION_2].UpdatePosition(nX, nZ);

        s_msgPosition.m_nPlayer = MAGE_2;
        s_msgPosition.m_nX      = nX;
        s_msgPosition.m_nZ      = nZ;

        Send(s_msgPosition, SESSION_1);
    }
}

void ServerGame::UseCard(int nCard,
                         int nCaster)
{
    int   i     = 0;
    Card* pCard = 0;

    if (nCaster >= MAGE_1 &&
        nCaster <= MAGE_2)
    {
        for (i = 0; i < HAND_SIZE; i++)
        {
            if (m_arHands[nCaster][i] == nCard)
            {
                // Card exists in hand, next check if the caster has
                // enough mana to stop cheaters and desync issues.

                pCard = InstantiateCard(nCard);
                if (pCard == 0)
                {
                    // Card could not be instantiated
                    LogError("Card could not be instantiated from ID in ServerGame::UseCard()");
                    return;
                }

                if (m_arMages[nCaster].GetMana() >= pCard->GetManaCost())
                {
                    LogDebug("Card activated on Server.");

                    // Activate in ServerGame.
                    pCard->Cast(this, nCaster);

                    // Send MsgCard to other client
                    s_msgCard.Clear();
                    s_msgCard.m_nCard   = nCard;
                    s_msgCard.m_nCaster = nCaster;
                    Send(s_msgCard, (nCaster == MAGE_1) ? MAGE_2 : MAGE_1);
                    
                    delete pCard;
                    pCard = 0;
                    return;
                }
            }
        }
    }
    else
    {
        LogError("Invalid caster in ServerGame::UseCard()");
    }
}

Mage* ServerGame::GetMage(int nIndex)
{
    if (nIndex >= MAGE_1 && 
        nIndex <= NUM_MAGES)
    {
        return &(m_arMages[nIndex]);
    }
    else
    {
        return 0;
    }
}