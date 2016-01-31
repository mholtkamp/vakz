#include "VSocket.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>
#if defined(ANDROID)
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

//*****************************************************************************
// Constructor
//*****************************************************************************
Socket::Socket()
{
    m_nType = TYPE_SERVER;
    m_nConnected = 0;
}

//*****************************************************************************
// Constructor
//*****************************************************************************
Socket::~Socket()
{

}

//*****************************************************************************
// Constructor
//*****************************************************************************
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
#endif
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::Shutdown()
{

}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::SetBlocking(int nEnable)
{
#if defined(WINDOWS)
    unsigned long lEnable = static_cast<unsigned long>(!nEnable);

    if(ioctlsocket(m_sock, FIONBIO, &lEnable) == SOCKET_ERROR)
    {
        LogError("Socket could not be set to blocking/nonblocking.");
    }
#elif defined(ANDROID)
    int nFlag = !nEnable;
    ioctl(m_sock, FIONBIO, &nFlag);
#endif
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::Connect(int         nProtocol,
                     const char* pIPAddress,
                     int         nPort)
{
#if defined(WINDOWS)
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

#elif defined(ANDROID)
    struct sockaddr_in serv_addr;

    m_nType = TYPE_CLIENT;

    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set the address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(pIPAddress);
    serv_addr.sin_port = htons(nPort);

    if(connect(m_sock,
               (struct sockaddr*) &serv_addr,
               sizeof(serv_addr)) < 0)
    {
        LogError("Error connecting to server.");
        return;
    }

#endif

    // Set the default block mode to non-blocking
    SetBlocking(0);
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::Open(int nProtocol,
                  int nPort)
{
#if defined(WINDOWS)
    SOCKADDR_IN sinTarget;

    m_nType = TYPE_SERVER;

    sinTarget.sin_family = AF_INET;
    sinTarget.sin_addr.s_addr = htonl (INADDR_ANY);
    sinTarget.sin_port = htons(nPort);
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
    
#elif defined (ANDROID)
    struct sockaddr_in serv_addr;

    m_nType = TYPE_SERVER;

    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (m_sock < 0)
    {
        LogError("Error creating socket in Socket::Open().");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(nPort);

    if (bind(m_sock,
            (struct sockaddr*) &serv_addr,
             sizeof(serv_addr)) < 0)
    {
        LogError("Error binding server socket.");
    }
#endif

    // Set the default block mode to non-blocking
    SetBlocking(0);
}

//*****************************************************************************
// Constructor
//*****************************************************************************
Socket* Socket::Accept()
{
#if defined (WINDOWS)
    Socket* retSock = 0;
    SOCKET newSock = 0;

    if (m_nType == TYPE_SERVER)
    {
        if (listen(m_sock, 1) == SOCKET_ERROR)
        {
            // No connections
            return 0;
        }

        newSock = accept(m_sock, 0, 0);

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
#elif defined (ANDROID)
    Socket* retSock = 0;
    int newSock = 0;

    if (m_nType == TYPE_SERVER)
    {
        listen(m_sock, 1);
        newSock = accept(m_sock, 0, 0);

        if (newSock >= 0)
        {
            retSock = new Socket();
            retSock->SetSocketHandle(newSock);
            LogDebug("Client connected!");

            return retSock;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        LogError("Cannot accept connection on client socket.");
        return 0;
    }
#endif
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::Close()
{
    if (m_sock != 0)
    {
#if defined (WINDOWS)
        closesocket(m_sock);
#elif defined (ANDROID)
        shutdown(m_sock, SHUT_RDWR);
#endif
    }
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::SetSocketHandle(int nSocketHandle)
{
    m_sock = nSocketHandle;
}

//*****************************************************************************
// Constructor
//*****************************************************************************
void Socket::Send(char* pBuffer,
                  int   nLength)
{
    send(m_sock, pBuffer, nLength, 0);
}

//*****************************************************************************
// Constructor
//*****************************************************************************
int Socket::Receive(char* pBuffer,
                    int   nLength)
{
    return recv(m_sock, pBuffer, nLength, 0);
}
