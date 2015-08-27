#include "MatchQueue.h"
#include "Session.h"
#include "ServerGame.h"
#include "Log.h"
#include "Constants.h"

#include <string.h>

MatchQueue::MatchQueue()
{
    m_arGames    = 0;
    memset(m_arSessions, 0, MATCH_QUEUE_SIZE * sizeof(void*));
}

MatchQueue::~MatchQueue()
{

}

void MatchQueue::Update()
{
    int i        = 0;
    int nNewGame = 0;

    // Iterate through sessions and create games for every two
    for (i = 0; i < MATCH_QUEUE_SIZE - 1; i++)
    {
        if (m_arSessions[i]   != 0 &&
            m_arSessions[i+1] != 0)
        {
            nNewGame = StartGame(m_arSessions[i], m_arSessions[i+1]);
            if (nNewGame != 0)
            {
                // The game was successfully started, so remove sessions from
                //  the queue and then shift the queue up.
                m_arSessions[i]   = 0;
                m_arSessions[i+1] = 0;
                ShiftQueue();
                i--;
            }
        }
    }
}

int MatchQueue::AddSession(void* pSession)
{
    int i = 0;

    for (i = 0; i < MATCH_QUEUE_SIZE; i++)
    {
        if (m_arSessions[i] == pSession)
        {
            // Session is already in queue.
            return 0;
        }
        if(m_arSessions[i] == 0)
        {
            // A session slot is available in the match queue.
            m_arSessions[i] = pSession;
            return 1;
        }
    }

    // Queue is full (which is pretty much impossible).
    return 0;
}

void MatchQueue::RemoveSession(void* pSession)
{
    int i = 0;

    for (i = 0; i < MATCH_QUEUE_SIZE; i++)
    {
        if (m_arSessions[i] == pSession)
        {
            m_arSessions[i] = 0;
            ShiftQueue();
            return;
        }
    }
}

void MatchQueue::ShiftQueue()
{
    int i    = 0;
    int nPos = 0;

    for (i = 0; i < MATCH_QUEUE_SIZE; i++)
    {
        if (m_arSessions[i] != 0 &&
            i != nPos)
        {
            // Move the session to the back of the queue
            m_arSessions[nPos] = m_arSessions[i];

            // Increment the new index to shift to
            nPos++;

            // Erase the pointer at index i
            m_arSessions[i] = 0;
        }
    }
}

void MatchQueue::SetGameArray(void* arGames)
{
    m_arGames = arGames;
}

int MatchQueue::StartGame(void* pSession1,
                          void* pSession2)
{
    int i = 0;
    ServerGame* arGames = reinterpret_cast<ServerGame*>(m_arGames);

    if (m_arGames == 0)
    {
        LogError("Cannot start game because match queue does not have game array.");
        return 0;
    }

    for (i = 0; i < MATCH_QUEUE_SIZE; i++)
    {
        if (arGames[i].GetGameState() == GAME_STATE_INACTIVE)
        {
            arGames[i].SetSessions(pSession1, pSession2);
            return 1;
        }
    }

    // No inactive game could be found.
    return 0;
}