#ifndef MATCH_QUEUE_H
#define MATCH_QUEUE_H

#include "ServerConstants.h"

class MatchQueue
{
public:

    MatchQueue();
    ~MatchQueue();

    void Update();

    int AddSession(void* pSession);

    void RemoveSession(void* pSession);

    void ShiftQueue();

    void SetGameArray(void* arGames);

    int StartGame(void* pSession1,
                  void* pSession2);

private:
    
    // Pointer to Main's array of games
    void* m_arGames;

    // Array of session pointers that are currently in queue.
    void* m_arSessions[MATCH_QUEUE_SIZE];
};

#endif