#ifndef MESSAGE_H
#define MESSAGE_H

#define HEADER_SIZE 4


#define USER_BUFFER_SIZE 16
#define PASS_BUFFER_SIZE 16
#define DECK_SIZE        30
#define COLLECTION_SIZE  512

enum MessageIDs
{
    MSG_KEEPALIVE     = 1,
    MSG_LOGIN         = 2,
    MSG_QUEUE         = 3,
    MSG_BUY           = 4,
    MSG_DECK          = 5,
    MSG_REGISTER      = 6,
    MSG_RES_LOGIN     = 7,
    MSG_RES_QUEUE     = 8,
    MSG_RES_BUY       = 9,
    MSG_RES_DECK      = 10,
    MSG_RES_REGISTER  = 11,
    MSG_DISCONNECT    = 12
};

enum MessageSizes
{
    MSG_KEEPALIVE_SIZE = 0,
    MSG_LOGIN_SIZE     = 32,
    MSG_REGISTER_SIZE  = 32,

    MSG_RES_LOGIN_SIZE    = 2420,
    MSG_RES_REGISTER_SIZE = 2420

};

class Message
{
public:

    Message();
    ~Message();

    virtual void Read(char* pBuffer) = 0;

    virtual void Write(char* pBuffer) = 0;

    virtual void Clear();

    int m_nID;

};



#endif