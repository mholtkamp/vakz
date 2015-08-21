#ifndef VSOCKET_H
#define VSOCKET_H

#if defined(WINDOWS)
#include <winsock.h>
#elif defined(ANDROID)
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

class Socket
{

public:

    Socket();

    ~Socket();

    static void Initialize();

    static void Shutdown();

    void SetBlocking(int nEnable);

    void Connect(int         nProtocol,
                 const char* pIPAddress,
                 int         nPort);

    void Open(int nProtocol,
              int nPort);

    Socket* Accept();

    void Send(char* pBuffer,
              int   nLength);

    int Receive(char* pBuffer,
                 int   nLength);

    void Close();

    void SetSocketHandle(int nSocketHandle);

    enum SocketEnum
    {
        TYPE_SERVER = 1,
        TYPE_CLIENT = 2,

        TCP = 1,
        UDP = 2
    };

private:

    int m_nType;

    int m_nBound;
    int m_nConnected;

#if defined (WINDOWS)
    SOCKET m_sock;
#elif defined(ANDROID)
    int m_sock;
#endif

};


#endif
