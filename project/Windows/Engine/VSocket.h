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

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Socket();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Socket();

    //## **********************************************************************
    //## Initialize
    //##
    //## Should be called before creating an socket objects.
    //## **********************************************************************
    static void Initialize();

    //## **********************************************************************
    //## Shutdown
    //##
    //## Call to clean up resources used by sockets.
    //## **********************************************************************
    static void Shutdown();

    //## **********************************************************************
    //## SetBlocking
    //##
    //## Controls whether this Socket object should block when Receiving 
    //## or Accepting.
    //## **********************************************************************
    void SetBlocking(int nEnable);

    //## **********************************************************************
    //## Connect
    //##
    //## Connects to server socket with given IP Address and port.
    //##
    //## Input:
    //##   nProtocol  - TCP or UDP.
    //##   pIPAddress - null terminated string that contains ip address.
    //##   nPort      - port to connect to.
    //## **********************************************************************
    void Connect(int         nProtocol,
                 const char* pIPAddress,
                 int         nPort);

    //## **********************************************************************
    //## Open
    //##
    //## Opens this socket as a server socket on supplied port.
    //##
    //## Input:
    //##   nProtocol - TCP or UDP.
    //##   nPort     - port to open.
    //## **********************************************************************
    void Open(int nProtocol,
              int nPort);

    //## **********************************************************************
    //## Accept
    //##
    //## Attempts to accept an incoming connection if this socket has been 
    //## opened as a server socket.
    //##
    //## Returns:
    //##   Socket - a new socket that should be used for communicating with
    //##            client.
    //## **********************************************************************
    Socket* Accept();

    //## **********************************************************************
    //## Send
    //## 
    //## Sends message if this Socket has been connected.
    //## 
    //## Input:
    //##   pBuffer - buffer containing data.
    //##   nLength - length of message in bytes.
    //## **********************************************************************
    void Send(char* pBuffer,
              int   nLength);

    //## **********************************************************************
    //## Receive
    //##
    //## Receieves all data that has been sent to this Socket.
    //## 
    //## Input:
    //##   pBuffer - pointer to buffer that will store new data receieved.
    //##   nLength - size of buffer.
    //##
    //## Returns:
    //##   int - bytes reveived.
    //## **********************************************************************
    int Receive(char* pBuffer,
                int   nLength);

    //## **********************************************************************
    //## Closes
    //## 
    //## Closes connection to remote machine.
    //## **********************************************************************
    void Close();

    //## **********************************************************************
    //## SetSocketHandle
    //## 
    //## Sets this Socket's handle to OS socket.
    //## **********************************************************************
    void SetSocketHandle(int nSocketHandle);

    enum SocketEnum
    {
        TYPE_SERVER = 1,
        TYPE_CLIENT = 2,

        TCP = 1,
        UDP = 2
    };

private:

    // Type of socket, TYPE_SERVER or TYPE_CLIENT
    int m_nType;

    // Socket states
    int m_nBound;
    int m_nConnected;

    // Socket handle
#if defined (WINDOWS)
    SOCKET m_sock;
#elif defined(ANDROID)
    int m_sock;
#endif

};


#endif
