#include "VSocket.h"
#include "Session.h"
#include "MatchQueue.h"
#include "ServerGame.h"

#define MAX_SESSIONS 64
#define MAX_GAMES 32
#define SERVER_PORT 2000

int main()
{
    Socket::Initialize();
    Session arSessions[MAX_SESSIONS];
    ServerGame arGames[MAX_GAMES];
    MatchQueue matchQueue;

    matchQueue.SetGameArray(arGames);
    Session::SetMatchQueue(&matchQueue);

    Socket serverSocket;
    
    
    serverSocket.Open(Socket::TCP, SERVER_PORT);

    while (1)
    {
        Sleep(5);

        // First, Check for new connections.
        Socket* pNewSocket = 0;
        pNewSocket = serverSocket.Accept();

        if (pNewSocket != 0)
        {
            // Find first non-active session
            for (int i = 0; i < MAX_SESSIONS; i++)
            {
                if (arSessions[i].IsActive() == 0)
                {
                    arSessions[i].Activate(pNewSocket);
                    break;
                }
            }
        }

        // Secondly, update all active sessions
        for (int i = 0;i < MAX_SESSIONS; i++)
        {
            if (arSessions[i].IsActive() != 0)
            {
                arSessions[i].Update();
            }
        }

        // Next, update the match queue to create new games
        matchQueue.Update();
        
        // Update all games
        for (int i = 0; i < MAX_GAMES; i++)
        {
            if (arGames[i].IsActive() != 0)
            {
                arGames[i].Update();
            }
        }
    }
    return 0;
}
