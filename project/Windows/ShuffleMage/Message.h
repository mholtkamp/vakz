#ifndef MESSAGE_H
#define MESSAGE_H

#define HEADER_SIZE 4

enum MessageIDs
{
    MSG_KEEPALIVE  = 1,
    MSG_LOGIN      = 2,
    MSG_RLOGIN     = 3,
    MSG_QUEUE      = 4,
    MSG_RQUEUE     = 5,
    MSG_BUY        = 6,
    MSG_RBUY       = 7,
    MSG_DECK       = 8,
    MSG_RDECK      = 9,
    MSG_REGISTER   = 10,
    MSG_RREGISTER  = 11,
    MSG_DISCONNECT = 12
};

enum MessageSizes
{
    MSG_KEEPALIVE_SIZE = 0,
    MSG_LOGIN_SIZE     = 32,
};

class Message
{
public:

    Message();
    ~Message();

    virtual void Read(char* pBuffer);

    virtual void Write(char* pBuffer);

    virtual void Clear();

    int m_nID;

};



#endif