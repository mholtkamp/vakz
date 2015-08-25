#include "VSocket.h"
#include "Session.h"

#define MAX_SESSIONS 64
#define MAX_GAMES 32
#define SERVER_PORT 2000

int main()
{
    Socket::Initialize();
    Session arSessions[MAX_SESSIONS];

    Socket serverSocket;

    serverSocket.Open(Socket::TCP, SERVER_PORT);

    while (1)
    {

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
    }
    return 0;
}