#include "VSocket.h"
#include "Log.h"
#include <stdio.h>

Socket::Socket()
{
    m_nType = TYPE_SERVER;
}

Socket::~Socket()
{

}

void Socket::Initialize()
{

#if defined (WINDOWS)
    WSADATA wsadata;

    int nError = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (nError)
    {
        LogError("Error initializing winsock library.");
        return;
    }

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return;
    }
#elif defined (ANDROID)

#endif
}

void Socket::Shutdown()
{

}

void Socket::SetBlocking(int nEnable)
{
    unsigned long lEnable = static_cast<unsigned long>(!nEnable);

    if(ioctlsocket(m_sock, FIONBIO, &lEnable) == SOCKET_ERROR)
    {
        LogError("Socket could not be set to blocking/nonblocking.");
    }
}

void Socket::Connect(int         nProtocol,
                     const char* pIPAddress,
                     int         nPort)
{
    SOCKADDR_IN sinTarget;

    m_nType = TYPE_CLIENT;

    sinTarget.sin_family = AF_INET;
    sinTarget.sin_port   = htons(nPort);

    sinTarget.sin_addr.s_addr = inet_addr (pIPAddress);
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_sock == INVALID_SOCKET)
    {
        LogError("Could not create socket in Socket::Connect().");
        return;
    }

    // Attempt to connect to address/port
    if (connect(m_sock, (SOCKADDR*) &sinTarget, sizeof(sinTarget)) == SOCKET_ERROR)
    {
        LogError("Could not connect to IP/Port.");
        return;
    }

    // Set the default block mode to non-blocking
    SetBlocking(0);
}

void Socket::Open(int nProtocol,
                  int nPort)
{
    SOCKADDR_IN sinTarget;

    m_nType = TYPE_SERVER;

    sinTarget.sin_family = AF_INET;
    sinTarget.sin_addr.s_addr = htonl (INADDR_ANY);
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (m_sock == INVALID_SOCKET)
    {
        LogError("Failed to create socket in Socket::Open().");
        return;
    }

    if (bind (m_sock, (LPSOCKADDR) &sinTarget, sizeof(sinTarget)) == SOCKET_ERROR)
    {
        LogError("Socket could not be bound to specified port.");
        printf("Error code: %d\n", WSAGetLastError());
        return;
    }
    
    // Set the default block mode to non-blocking
    SetBlocking(0);
}

Socket* Socket::Accept()
{
    Socket* retSock = 0;
    SOCKET newSock = 0;

    if (m_nType == TYPE_SERVER)
    {
        if (listen(m_sock, 1) == SOCKET_ERROR)
        {
            // No connections
            return 0;
        }

        newSock = accept(m_sock, NULL, NULL);

        if (newSock == INVALID_SOCKET)
        {
            // No incoming connections, so return 0.
            return 0;
        }

        retSock = new Socket();
        retSock->SetSocketHandle(newSock);
        LogDebug("Client connected!");

        return retSock;
    }
    else
    {
        LogError("Cannot accept connection on client socket.");
        return 0;
    }
}

void Socket::Close()
{
    if (m_sock != 0)
    {
        closesocket(m_sock);
    }
}

void Socket::SetSocketHandle(int nSocketHandle)
{
    m_sock = nSocketHandle;
}

void Socket::Send(char* pBuffer,
                  int   nLength)
{
    send(m_sock, pBuffer, nLength, 0);
}

int Socket::Receive(char* pBuffer,
                    int   nLength)
{
    return recv(m_sock, pBuffer, nLength, 0);
}
