#include "MatchQueue.h"
#include "Session.h"
#include "ServerGame.h"

#include <string.h>

MatchQueue::MatchQueue()
{
    m_arGames    = 0;
    memset(m_arSessions, 0, MAX_SESSIONS * sizeof(void*));
}

MatchQueue::~MatchQueue()
{

}

void MatchQueue::Update()
{

}

int MatchQueue::AddSession(void* pSession)
{
    int i = 0;

    for (i = 0; i < MAX_SESSIONS; i++)
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

    for (i = 0; i < MAX_SESSIONS; i++)
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

    for (i = 0; i < MAX_SESSIONS; i++)
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
