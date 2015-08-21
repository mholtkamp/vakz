#include "VSocket.h"
#include "Log.h"

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

void Socket::Connect(int         nProtocol,
                     const char* pIPAddress,
                     int         nPort)
{
    SOCKADDR_IN sinTarget;

    sinTarget.sin_family = AF_INET;
    sinTarget.sin_port   = htons(nPort);

    m_nType = TYPE_CLIENT;
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
}

void Socket::Open(int nProtocol,
                  int nPort)
{
    SOCKADDR_IN sinTarget;

    m_nType = TYPE_SERVER;
    sinTarget.sin_addr.s_addr = htonl (INADDR_ANY);
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_sock == INVALID_SOCKET)
    {
        LogError("Failed to create socket in Socket::Open().");
        return;
    }

    if (bind (m_sock, (LPSOCKADDR) &sinTarget, sizeof(sinTarget)) == SOCKET_ERROR)
    {
        LogError("Socket could not be bound to specified port.");
        return;
    }
}

Socket* Socket::Accept()
{
    Socket* retSock = 0;
    SOCKET newSock = 0;

    if (m_nType == TYPE_SERVER)
    {
        if (listen(m_sock, 1) == SOCKET_ERROR)
        {
            LogError("Cannot listen on server socket.");
            return;
        }

        newSock = accept(m_sock, NULL, NULL);

        if (newSock == INVALID_SOCKET)
        {
            LogError("Failed to accept connection on server socket.");
            return;
        }

        retSock = new Socket();
        retSock->SetSocketHandle(newSock);
        LogDebug("Client connected!");

        return retSock;
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
