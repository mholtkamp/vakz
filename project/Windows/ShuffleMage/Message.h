#ifndef MESSAGE_H
#define MESSAGE_H

#include "Constants.h"

#define HEADER_SIZE 4

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
    MSG_DISCONNECT    = 12,



    MSG_POSITION      = 101,
    MSG_CARD          = 102,
    MSG_HEALTH        = 103,
    MSG_DRAW          = 104,
    MSG_READY         = 105,
    MSG_START         = 106,
    MSG_STATUS        = 107,
    MSG_TILE          = 108,
    MSG_END           = 109
};

enum MessageSizes
{
    MSG_KEEPALIVE_SIZE = 0,
    MSG_LOGIN_SIZE     = 32,
    MSG_QUEUE_SIZE     = 4,
    MSG_REGISTER_SIZE  = 32,
    
    MSG_POSITION_SIZE  = 12,
    MSG_CARD_SIZE      = 8,
    MSG_HEALTH_SIZE    = 8,
    MSG_DRAW_SIZE      = HAND_SIZE * sizeof(int),
    MSG_STATUS_SIZE    = 12,
    MSG_TILE_SIZE      = 16,

    MSG_RES_LOGIN_SIZE    = 2424,
    MSG_RES_QUEUE_SIZE    = 8,
    MSG_RES_REGISTER_SIZE = 2424

};

class Message
{
public:

    Message();
    ~Message();

    virtual void Read(char* pBuffer) = 0;

    virtual void Write(char* pBuffer) = 0;

    virtual int Size() = 0;

    virtual void Clear() = 0;

    int m_nID;

};

#endif